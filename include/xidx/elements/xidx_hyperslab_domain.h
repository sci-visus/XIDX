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

#ifndef XIDX_HYPERSLAB_DOMAIN_H_
#define XIDX_HYPERSLAB_DOMAIN_H_

#include "xidx/xidx.h"
#include "xidx_parse_utils.h"

namespace xidx{

class HyperSlabDomain : public ListDomain<double>{

public:
  HyperSlabDomain(std::string _name) : ListDomain(_name){
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    
    std::shared_ptr<DataItem> physical = data_items[0];
    
    physical->format_type = DataItem::FormatType::XML_FORMAT;
    physical->number_type = XidxDataType::NumberType::FLOAT_NUMBER_TYPE;
    physical->bit_precision = "64";
  }
  
  HyperSlabDomain(const HyperSlabDomain* d) : ListDomain(d->name){
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    data_items = d->data_items;
  }
  
  int setDomain(uint32_t dims, double* phy_hyperslab){
    assert(data_items.size() >= 1);
    std::shared_ptr<DataItem> physical = data_items[0];
    
    physical->dimensions = toIndexVector(string_format("%d", dims));
    
    for(int i=0; i< dims; i++){
      physical->text += std::to_string(phy_hyperslab[i]) +" ";
    }
    
    trim(physical->text);
    
    return 0;
  }
  
  virtual const IndexSpace& getLinearizedIndexSpace() override{
    values_vector.resize(count);
    
    for(int i=0; i< count; i++){
      values_vector[i] = start + i*step;
    }
    return values_vector;
  };
  
  virtual xmlNodePtr serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    xmlNodePtr domain_ptr = Domain::serialize(parent);

    return domain_ptr;
  };
  
  virtual int deserialize(xmlNodePtr node, Parsable* _parent) override{
    assert(data_items.size() >= 1);
    setParent(_parent);
    std::shared_ptr<DataItem> physical = data_items[0];
    
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      //for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          
          if(isNodeName(cur_node, "DataItem")){
            physical->deserialize(cur_node, this);
          }
        }
      //}
    }
    
    assert(physical->dimensions[0]==3);
    
    std::string s = physical->text;
    std::string delimiter = " ";
    
    size_t pos = 0;
    std::string tokens[3];
    int c=0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
      tokens[c] = s.substr(0, pos);
      
      s.erase(0, pos + delimiter.length());
      c++;
    }
    
    start = stof(tokens[0]);
    step  = stof(tokens[1]);
    count = stoi(s);

    return 0;
  };
  
  virtual std::string getClassName() const override { return "HyperSlabDomain"; };

  //TODO swig does not allsee these inherited function so rewrite
  Domain::DomainType getType() { return type; }
  std::vector<std::shared_ptr<Attribute>> getAttributes() const{ return attributes; }

private:
  double start = 0;
  double step  = 0;
  int    count = 0;
  
};
  

}
#endif
