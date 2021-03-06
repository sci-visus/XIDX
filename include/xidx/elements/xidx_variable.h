/*
 * Copyright (c) 2017 University of Utah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XIDX_VARIABLE_H_
#define XIDX_VARIABLE_H_

#include <algorithm>

#include "xidx/xidx.h"

namespace xidx{

class Group;
class DataItem;

class Variable : public Parsable{
public:
  std::string name;
  
  enum CenterType{
    NODE_CENTER = 0,
    CELL_CENTER = 1,
    GRID_CENTER = 2,
    FACE_CENTER = 3,
    EDGE_CENTER = 4
  };
  
  static inline const char* toString(CenterType v)
  {
    switch (v)
    {
      case NODE_CENTER:   return "Node";
      case CELL_CENTER:   return "Cell";
      case GRID_CENTER:   return "Grid";
      case FACE_CENTER:   return "Face";
      case EDGE_CENTER:   return "Edge";
      default:            return "[Unknown]";
    }
  }
  
  class defaults{
  public:
    static const CenterType VARIABLE_CENTER_TYPE = CenterType::CELL_CENTER;
  };
  
private:
  std::vector<std::shared_ptr<Attribute>> attributes;
  std::vector<std::shared_ptr<DataItem> > data_items;
  
public:
  CenterType center_type;

  Variable(std::string _name){
    name = _name;
    center_type = defaults::VARIABLE_CENTER_TYPE;
  }
  
  Variable(Parsable* _parent){
    setParent(_parent);
  }
  
//  Variable(const Variable* v){
//    setParent(v->getParent());
//    attributes = v->attributes;
//    center_type = v->center_type;
//    data_items = v->data_items;
//  }
//
//  Variable(const Variable& v){
//    setParent(v.getParent());
//    attributes = v.attributes;
//    center_type = v.center_type;
//    data_items = v.data_items;
//  }
  
  // int get_n_components(){
  //   size_t found=data.dimensions.find_last_of(" \\");
  //   return stoi(data.dimensions.substr(found));
  // }

  // std::string get_dtype_str(){
  //   std::string numberType = toString(data.numberType); 
  //   std::transform(numberType.begin(), numberType.end(), numberType.begin(), ::tolower);
  //   int n_bytes = stoi(data.precision)*8;
  //   return string_format("%d*%s%d",get_n_components(),numberType.c_str(),n_bytes);
  // }

  virtual xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{
    xmlNodePtr variable_node = xmlNewChild(parent, NULL, BAD_CAST "Variable", NULL);
    xmlNewProp(variable_node, BAD_CAST "Name", BAD_CAST name.c_str());
    //if(center_type != defaults::VARIABLE_CENTER_TYPE)
    xmlNewProp(variable_node, BAD_CAST "Center", BAD_CAST toString(center_type));

    for(auto item: data_items)
      xmlNodePtr data_node = item->serialize(variable_node);

    for(auto& curr_att : attributes){
      xmlNodePtr info_node = curr_att->serialize(variable_node);
    }

    return variable_node;
  };

  // int set_raw_data(std::vector<uint32_t>& dims, char* raw_data, char* filepath){
  //   std::stringstream ss;
  //   for(int i=0; i< dims.size(); i++)
  //     if(i < dims.size()-1)
  //       ss << dims[i] << " ";
  //     else
  //       ss << dims[i];

  //   set_raw_data(ss.str(), raw_data, filepath);

  //   return 0;
  // }

  // int set_raw_data(std::string dims, char* raw_data, char* filepath){
  //   data.formatType = FormatType::BINARY_FORMAT;
  //   data.dimensions = dims;

  //   std::vector<int> idims;
  //   std::istringstream f(dims);

  //   std::string s;
  //   while (getline(f, s, ' ')) {
  //     idims.push_back(atoi(s.c_str()));
  //   }

  //   size_t size = 1;
  //   for(auto d:idims)
  //     size *= d;

  //   std::stringstream ss;
  //   ss << name << ".raw";

  //   std::ofstream out;
  //   out.open(ss.str().c_str(), std::ios::out | std::ios::binary);
  //   out.write(raw_data, size);
  //   out.close();

  //   data.text = ss.str();

  //   return 0;
  // }

  // int get_raw_data(char* raw_data, size_t size){
  //   std::ifstream out;
  //   out.open(data.text.c_str(), std::ios::in | std::ios::binary);
  //   out.read(raw_data, size);
  //   out.close();

  //   return 0;
  // }
  
  virtual int addValues(std::vector<double> vals){
    int stride = vals.size();
    
    if(data_items.size()==0){
      data_items.push_back(std::make_shared<DataItem>(this));
      data_items[0]->format_type = DataItem::FormatType::XML_FORMAT;
    }
    
    for(auto v:vals)
      data_items[0]->addValue(v, stride);
    
    return 0;
  }
  
  virtual int addValue(double v){
    if(data_items.size()==0){
      data_items.push_back(std::make_shared<DataItem>(this));
      data_items[0]->format_type = DataItem::FormatType::XML_FORMAT;
    }
    
    data_items[0]->addValue(v);
    
    return 0;
  }
  
  const std::vector<double>& getValues(int axis = 0) const{
    if(data_items.size() > axis){
      return data_items[axis]->getValues();
    }
    else
      fprintf(stderr, "Axis %d does not exist", axis);
    
    return std::vector<double>();
  }
  
  virtual size_t getVolume() const{
    size_t total = 1;
    for(auto& item: this->data_items)
        total *= item->getVolume();
    return total;
  }
  
  virtual int addAttribute(std::string name, std::string value){
    std::shared_ptr<Attribute> att(new Attribute(name, value));
    attributes.push_back(att);
    return 0;
  }
  
  virtual int deserialize(xmlNodePtr node, Parsable *_parent) override{
    if(!isNodeName(node,"Variable"))
      return -1;

    setParent(_parent);
    
    assert(getParent()!=nullptr);
    
    name = xidx::getProp(node, "Name");

    const char* center_type_value = getProp(node, "Center");
    if(center_type_value != NULL){
      for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
        if (strcmp(center_type_value, toString(static_cast<CenterType>(t)))==0)
            center_type = static_cast<CenterType>(t);
    }
    else
      center_type = defaults::VARIABLE_CENTER_TYPE;

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(isNodeName(inner_node, "Attribute")){
        std::shared_ptr<Attribute> att(new Attribute);
        att->deserialize(inner_node, this);
        attributes.push_back(att);
      }
      if(isNodeName(inner_node, "DataItem")){
        std::shared_ptr<DataItem> ditem(new DataItem(this));
        ditem->deserialize(inner_node, ditem->getParent());
        data_items.push_back(ditem);
      }
    }

    return 0;
  };
  
  virtual std::vector<std::shared_ptr<Attribute>> getAttributes() const { return attributes; }
  
  virtual int addAttribute(const std::shared_ptr<Attribute>& att){ attributes.push_back(att); return 0; }
  
  virtual int addAttribute(const std::vector<std::shared_ptr<Attribute>>& atts){
    attributes.insert(attributes.end(), atts.begin(), atts.end());
    return 0;
  }
  
  virtual std::vector<std::shared_ptr<DataItem> > getDataItems() const { return data_items; }
  
  virtual int addDataItem(const std::shared_ptr<DataItem>& di){ data_items.push_back(di); return 0; }
  
  virtual int addDataItem(const std::vector<std::shared_ptr<DataItem> >& dis){
    data_items.insert(data_items.end(), dis.begin(), dis.end());
    return 0;
  }
  
  virtual std::string getClassName() const override { return "Variable"; };

};
}

#endif
