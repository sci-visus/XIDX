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

#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include <sstream>
#include "xidx/xidx.h"

namespace xidx{

template<typename T>
class ListDomain : public Domain{

public:
  IndexSpace values_vector;
  
  ListDomain(std::string _name) : Domain(_name) {
    type = Domain::LIST_DOMAIN_TYPE;
    data_items.push_back(std::make_shared<DataItem>(new DataItem(name, this)));
  };
  
  ListDomain(std::string _name, std::shared_ptr<DataItem> item) : Domain(_name) {
    type = Domain::LIST_DOMAIN_TYPE;
    data_items.push_back(item);
  };
  
//  ListDomain(const ListDomain& c) : Domain(c){
//    type = LIST_DOMAIN_TYPE;
//    name = c.name;
//    data_items = c.data_items;
//    values_vector = c.values_vector;
//  };
  
  ListDomain(const ListDomain* d) : Domain(d->name){
    type = LIST_DOMAIN_TYPE;
    data_items = d->data_items;
    values_vector = d->values_vector;
  }
  
  ListDomain(std::shared_ptr<ListDomain<double>> d) : Domain(d->name){
    type = LIST_DOMAIN_TYPE;
    data_items = d->data_items;
    values_vector = d->values_vector;
  }
  
  int AddDomainItem(T phy){
    values_vector.push_back(phy);
  
    return 0;
  }
  
  virtual const IndexSpace& GetLinearizedIndexSpace() override{
    return values_vector;
  };
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    auto physical = data_items[0];
    physical->text="";
    physical->dimensions.clear();
    physical->dimensions.push_back(values_vector.size());
    
    if(!std::is_same<T, DataSource>::value){
      for(auto phy: values_vector)
        physical->text+=std::to_string(phy)+" ";
    }
    
    trim(physical->text);
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
      
    return domain_node;
  }
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    Domain::Deserialize(node, _parent);
    
    SetParent(_parent);
  
    assert(GetParent()!=nullptr);
      
    int count = data_items.size();
  
    if(count == 1){
      auto& item = data_items[0];
      
      assert(item->dimensions.size()>0);
      
      size_t length = item->dimensions[0];

      values_vector.resize(length);
      
      std::stringstream stream_data(item->text);
      
      for(int i=0; i< length; i++){
        stream_data >> values_vector[i];
      }
      
    }
    else{
      assert(false);
      fprintf(stderr, "too many data items in list domain\n");
    }
    
    return 0;
  }
  
  virtual std::string GetClassName() const override { return "ListDomain"; };
  
};

}
#endif
