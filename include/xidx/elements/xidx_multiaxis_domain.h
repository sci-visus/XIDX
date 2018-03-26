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

template<typename T>
class MultiAxisDomain : public Domain{

public:
  
  MultiAxisDomain(std::string _name) : Domain(_name) {
    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
  };

  MultiAxisDomain(std::string _name, int count, const char* axis_names...) : Domain(_name) {
    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    va_list args;
    va_start(args, axis_names);

    lists.resize(count);
    for (int i = 0; i < count; ++i) {
      std::string arg_name = std::string(va_arg(args, const char*));
      lists[i]= ListDomain<T>(arg_name, this);
    }
    
    va_end(args);
  };
  
  MultiAxisDomain(const MultiAxisDomain* d) : Domain(d->name){
    SetParent(d->GetParent());
    lists = d->lists;
    data_items = d->data_items;
  }
  
  int SetAxis(int index, ListDomain<T>& list){
    assert(index < list.data_items.size());
    
    lists[index] = list;
    lists[index].parent = this;
    lists[index].type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    return 0;
  }
  
  int AddAxis(ListDomain<T>& list){
    lists.push_back(list);
    lists.back().SetParent(this);
    lists.back().type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    xmlNodePtr domain_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(domain_node, BAD_CAST "Type", BAD_CAST ToString(type));
    data_items.clear();
    
    int items_count = 0;
    for(auto& l: lists){
      DataItem itemt(this);
      itemt.name = l.name;
      data_items.push_back(itemt);
      
      for(int i=0; i< l.data_items.size(); i++){        
        data_items[items_count] = l.data_items[i];      
      }
      
      items_count++;
    }
    
    for(auto item: data_items)
      item.Serialize(domain_node);
    
    return parent;
  }
  
  virtual const ListDomain<PHY_TYPE>& GetAxis(int index){
    return lists[index];
  };
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace(int index){
    return lists[index].GetLinearizedIndexSpace();
  };
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace(){
    // TODO NOT IMPLEMENTED
    fprintf(stderr, "GetLinearizedIndexSpace() for MultiAxisDomain not implemented please\
            use GetLinearizedIndexSpace(int index)\n");
    assert(false);
    return lists[0].GetLinearizedIndexSpace();
  };

  
  int GetNumberOfAxis(){ return lists.size(); }
  
  virtual std::string GetClassName() const override { return "MultiAxisDomain"; };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    Domain::Deserialize(node, _parent);
    SetParent(_parent);
    
    assert(GetParent()!=nullptr);
    
    int count = data_items.size();
    
    lists.clear();
    
    for(int di=0; di < count; di++){
      const auto& item = data_items[di];
      
      assert(item.dimensions.size()>0);
      
      int length = item.dimensions[0];
      
      if(item.format_type == FormatType::XML_FORMAT){
        std::stringstream stream_data(item.text);
        
        ListDomain<T> list(item.name);
        list.data_items[0] = item;
        list.values_vector.resize(length);
        for(int i=0; i< length; i++){
          stream_data >> list.values_vector[i];
        }
      
        lists.push_back(list);
      }
      else{
        fprintf(stderr, "Deserialization of data items for type != XML is not implemented yet!\n");
      }
    }
    
    return 0;
  }
  
private:
  std::vector<ListDomain<T>> lists;
};

}
#endif
