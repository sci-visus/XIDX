#ifndef XIDX_DOUBLE_HYPERSLAB_DOMAIN_H_
#define XIDX_DOUBLE_HYPERSLAB_DOMAIN_H_

#include "xidx_domain.h"

namespace xidx{
  
class DoubleHyperSlabDomain : public HyperSlabDomain{

public:
  
  DoubleHyperSlabDomain(const HyperSlabDomain* c): HyperSlabDomain(c->name){
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    name = c->name;
    data_items = c->data_items;
    data_items.push_back(DataItem("Logical", this));
  };
  
  DoubleHyperSlabDomain(std::string _name) : HyperSlabDomain(_name) {
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    
    data_items.push_back(DataItem("Logical", this));
    
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    physical.format_type = FormatType::XML_FORMAT;
    physical.number_type = NumberType::FLOAT_NUMBER_TYPE;
    physical.bit_precision = "64";
    
    logical.format_type = FormatType::XML_FORMAT;
    logical.number_type = NumberType::FLOAT_NUMBER_TYPE;
    logical.bit_precision = "64";
  }
  
  int setDomain(uint32_t dims, double* phy_hyperslab, int32_t* log_hyperslab){
    assert(data_items.size() == 2);
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    physical.dimensions = string_format("%d", dims);
    logical.dimensions  = string_format("%d", dims);
    
    for(int i=0; i< dims; i++){
      physical.text += string_format("%f ", phy_hyperslab[i]);
      logical.text  += string_format("%d ", log_hyperslab[i]);
    }
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() == 2);
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    xmlNodePtr domain_ptr = Domain::Serialize(parent);

    physical.Serialize(domain_ptr);
    logical.Serialize(domain_ptr);

    return domain_ptr;
  };
  
  virtual int Deserialize(xmlNodePtr node) override{
    assert(data_items.size() == 2);
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    for (xmlNode* cur_node = node; cur_node; cur_node = cur_node->next) {
      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (inner_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(inner_node, "Physical")){
            physical.Deserialize(inner_node);
          }
          else if(IsNodeName(inner_node, "Logical")){
            logical.Deserialize(inner_node);
          }
        }
      }
    }

    return 0;
  };

};

}
#endif
