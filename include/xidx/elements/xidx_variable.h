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
  
namespace defaults{
  const CenterType VARIABLE_CENTER_TYPE = CenterType::CELL_CENTER;
}

class Variable : public Parsable{

private:
  std::vector<Attribute> attributes;
  std::vector<std::shared_ptr<DataItem> > data_items;
  
public:
  CenterType center_type;

  Variable(Parsable* _parent){
    SetParent(_parent);
  }
  
//  Variable(const Variable* v){
//    SetParent(v->GetParent());
//    attributes = v->attributes;
//    center_type = v->center_type;
//    data_items = v->data_items;
//  }
//  
//  Variable(const Variable& v){
//    parent = v.parent;
//    attributes = v.attributes;
//    center_type = v.center_type;
//    data_items = v.data_items;
//  }
  
  // int get_n_components(){
  //   size_t found=data.dimensions.find_last_of(" \\");
  //   return stoi(data.dimensions.substr(found));
  // }

  // std::string get_dtype_str(){
  //   std::string numberType = ToString(data.numberType); 
  //   std::transform(numberType.begin(), numberType.end(), numberType.begin(), ::tolower);
  //   int n_bytes = stoi(data.precision)*8;
  //   return string_format("%d*%s%d",get_n_components(),numberType.c_str(),n_bytes);
  // }

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr variable_node = xmlNewChild(parent, NULL, BAD_CAST "Variable", NULL);
    xmlNewProp(variable_node, BAD_CAST "Name", BAD_CAST name.c_str());
    //if(center_type != defaults::VARIABLE_CENTER_TYPE)
    xmlNewProp(variable_node, BAD_CAST "Center", BAD_CAST ToString(center_type));

    for(auto item: data_items)
      xmlNodePtr data_node = item->Serialize(variable_node);

    for(auto& curr_att : attributes){
      xmlNodePtr info_node = curr_att.Serialize(variable_node);
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

  int AddAttribute(std::string name, std::string value){
    Attribute att(name, value);
    attributes.push_back(att);
    return 0;
  }
  
  int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    if(!IsNodeName(node,"Variable"))
      return -1;

    SetParent(_parent);
    
    assert(GetParent()!=nullptr);
    
    name = GetProp(node, "Name");

    const char* center_type_value = GetProp(node, "Center");
    if(center_type_value != NULL){
      for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
        if (strcmp(center_type_value, ToString(static_cast<CenterType>(t)))==0)
            center_type = static_cast<CenterType>(t);
    }
    else
      center_type = defaults::VARIABLE_CENTER_TYPE;

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(IsNodeName(inner_node, "Attribute")){
        Attribute att;
        att.Deserialize(inner_node, this);
        attributes.push_back(att);
      }
      if(IsNodeName(inner_node, "DataItem")){
        std::shared_ptr<DataItem> ditem(new DataItem(this));
        ditem->Deserialize(inner_node, ditem->GetParent());
        data_items.push_back(ditem);
      }
    }

    return 0;
  };
  
  const std::vector<Attribute>& GetAttributes() const { return attributes; }
  
  int AddAttribute(const Attribute& att){ attributes.push_back(att); return 0; }
  
  int AddAttribute(const std::vector<Attribute>& atts){
    attributes.insert(attributes.end(), atts.begin(), atts.end());
    return 0;
  }
  
  const std::vector<std::shared_ptr<DataItem> >& GetDataItems() const { return data_items; }
  
  int AddDataItem(const std::shared_ptr<DataItem>& di){ data_items.push_back(di); return 0; }
  
  int AddDataItem(const std::vector<std::shared_ptr<DataItem> >& dis){
    data_items.insert(data_items.end(), dis.begin(), dis.end());
    return 0;
  }
  
  virtual std::string GetClassName() const override { return "Variable"; };

};
}

#endif
