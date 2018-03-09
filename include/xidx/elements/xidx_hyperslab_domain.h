#ifndef XIDX_HYPERSLAB_DOMAIN_H_
#define XIDX_HYPERSLAB_DOMAIN_H_

#include <cmath>
#include "xidx_domain.h"

namespace xidx{

class HyperSlabDomain : public Domain{

public:
  DataItem physical;
  DataItem logical;
  
  HyperSlabDomain(std::string _name) : Domain(_name) {
  
    physical.name = "Physical";
    physical.formatType = FormatType::XML_FORMAT;
    physical.numberType = NumberType::FLOAT_NUMBER_TYPE;
    physical.bit_precision = "64";
    
    logical.name = "Physical";
    logical.formatType = FormatType::XML_FORMAT;
    logical.numberType = NumberType::FLOAT_NUMBER_TYPE;
    logical.bit_precision = "64";
  }
  
  int setDomain(uint32_t dims, double* phy_hyperslab, int32_t* log_hyperslab){
    physical.dimensions = string_format("%d", dims);
    logical.dimensions  = string_format("%d", dims);
    
    for(int i=0; i< dims; i++){
      physical.text += string_format("%f ", phy_hyperslab[i]);
      logical.text  += string_format("%d ", log_hyperslab[i]);
    }
    
    return 0;
  }
  
  xmlNodePtr Serialize(xmlNode* parent){
    Domain::Serialize(parent);

    physical.Serialize(parent);
    logical.Serialize(parent);

    return parent;
  };
  
  int Deserialize(xmlNodePtr node){
    Domain::Deserialize(node);
    
    for (xmlNode* cur_node = node; cur_node; cur_node = cur_node->next) {
      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (inner_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(inner_node, "Physical")){
            physical.Deserialize(inner_node);
          }
          else if(IsNodeName(inner_node, "Logical")){
            physical.Deserialize(inner_node);
          }
        }
      }
    }

    return 0;
  };

};

}
#endif
