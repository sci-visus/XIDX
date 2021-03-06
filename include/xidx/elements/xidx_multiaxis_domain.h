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

#ifndef XIDX_MULTIAXIS_DOMAIN_H_
#define XIDX_MULTIAXIS_DOMAIN_H_

#include <sstream>
#include "xidx/xidx.h"

namespace xidx{
  
class MultiAxisDomain : public Domain{

public:
  
  MultiAxisDomain(std::string _name) : Domain(_name) {
    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
  };

  /// swig 3.0 does not support var args
//  MultiAxisDomain(std::string _name, int count, const char* axis_names...) : Domain(_name) {
//    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
//    
//    va_list args;
//    va_start(args, axis_names);
//
//    for (int i = 0; i < count; ++i) {
//      std::string arg_name = std::string(va_arg(args, const char*));
//      
//      if(i > axis.size()){
//        axis.push_back(Variable(arg_name));
//      }
//      else
//        axis[i] = Variable(arg_name);
//    }
//    
//    va_end(args);
//  };
  
  MultiAxisDomain(const MultiAxisDomain* d) : Domain(d->name){
    setParent(d->getParent());
    axis = d->axis;
    data_items = d->data_items;
  }
  
  int SetAxis(int index, Axis& _axis){
    assert(index < axis.size());
    
    axis[index] = _axis;
    axis[index].setParent(this);
    
    return 0;
  }
  
  int addAxis(Axis &_axis){
    axis.push_back(_axis);
    axis.back().setParent(this);
    
    return 0;
  }
  
  virtual xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{
    xmlNodePtr domain_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(domain_node, BAD_CAST "Type", BAD_CAST toString(type));
    data_items.clear();
    
//    int items_count = 0;
    for(auto& l: axis){
      l.serialize(domain_node);
//      std::shared_ptr<DataItem> itemt(new DataItem(this));
//      itemt->name = l.name;
//      data_items.push_back(itemt);
//      
//      for(auto di: l.getDataItems()){
//        data_items[items_count] = di;
//      }
//      
//      items_count++;
    }
    
//    for(auto item: data_items)
//      item->serialize(domain_node);
//    
    return parent;
  }
  
  virtual const Axis& getAxis(int index){
    return axis[index];
  };
  
  virtual const IndexSpace& getLinearizedIndexSpace(int index){
    assert(axis[index].getDataItems().size() > 0);
  
    return axis[index].getDataItems()[0]->getValues();
  };
  
  virtual const IndexSpace& getLinearizedIndexSpace() override{
    // TODO NOT IMPLEMENTED
    fprintf(stderr, "getLinearizedIndexSpace() for MultiAxisDomain not implemented please\
            use getLinearizedIndexSpace(int index)\n");
    assert(false);
    
    return getLinearizedIndexSpace(0);
  };

  int getNumberOfAxis(){ return axis.size(); }
  
  virtual std::string getClassName() const override { return "MultiAxisDomain"; };
  
  virtual int deserialize(xmlNodePtr node, Parsable *_parent) override{
    Domain::deserialize(node, _parent);
    setParent(_parent);
    
    assert(getParent()!=nullptr);
    
    int data_items_count=0;
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      
      if (cur_node->type == XML_ELEMENT_NODE) {
        
        if(isNodeName(cur_node, "Variable")){
          if(axis.size() > data_items_count){
            Axis& a = axis[data_items_count];
            a.deserialize(cur_node, this);
          }
          else{
            Axis a(this);
            a.deserialize(cur_node, this);
            axis.push_back(a);
          }
        
          data_items_count++;
        }
        else if(isNodeName(cur_node, "Variable")){
          
        }
      }
      
    }

//    
//    int count = data_items.size();
//    
//    axis.clear();
//    
//    for(int di=0; di < count; di++){
//      const auto& item = data_items[di];
//      
//      assert(item->dimensions.size()>0);
//      
//      int length = item->dimensions[0];
//      
//      if(item->format_type == FormatType::XML_FORMAT){
//        std::stringstream stream_data(item->text);
//        
//        Variable a(item->name);
//        
//        ListDomain<T> list(item->name);
//        list.data_items[0] = item;
//        list.values_vector.resize(length);
//        for(int i=0; i< length; i++){
//          stream_data >> list.values_vector[i];
//        }
//      
//        axis.push_back(list);
//      }
//      else{
//        fprintf(stderr, "Deserialization of data items for type != XML is not implemented yet!\n");
//      }
//    }
    
    return 0;
  }
  
private:
  std::vector<Axis> axis;
};

}
#endif
