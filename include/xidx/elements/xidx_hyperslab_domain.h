#ifndef XIDX_HYPERSLAB_DOMAIN_H_
#define XIDX_HYPERSLAB_DOMAIN_H_

#include "xidx/xidx.h"
#include "xidx_parse_utils.h"

namespace xidx{

class HyperSlabDomain : public ListDomain<double>{

public:
  HyperSlabDomain(std::string _name) : ListDomain(_name){
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    
    DataItem& physical = data_items[0];
    
    physical.format_type = FormatType::XML_FORMAT;
    physical.number_type = NumberType::FLOAT_NUMBER_TYPE;
    physical.bit_precision = "64";
  }
  
  HyperSlabDomain(const HyperSlabDomain* d) : ListDomain(d->name){
    data_items = d->data_items;
  }
  
  int setDomain(uint32_t dims, double* phy_hyperslab){
    assert(data_items.size() >= 1);
    DataItem& physical = data_items[0];
    
    physical.dimensions = string_format("%d", dims);
    
    for(int i=0; i< dims; i++){
      physical.text += string_format("%f ", phy_hyperslab[i]);
    }
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    
    xmlNodePtr domain_ptr = Domain::Serialize(parent);

    return domain_ptr;
  };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    assert(data_items.size() >= 1);
    parent = _parent;
    DataItem& physical = data_items[0];
    
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      //for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(cur_node, "DataItem")){
            physical.Deserialize(cur_node, this);
          }
        }
      //}
    }

    return 0;
  };

  virtual std::string GetClassName() override { return "HyperSlabDomain"; };
};

}
#endif
