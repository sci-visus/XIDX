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
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    data_items = d->data_items;
  }
  
  int setDomain(uint32_t dims, double* phy_hyperslab){
    assert(data_items.size() >= 1);
    DataItem& physical = data_items[0];
    
    physical.dimensions = ToIndexVector(string_format("%d", dims));
    
    for(int i=0; i< dims; i++){
      physical.text += std::to_string(phy_hyperslab[i]) +" ";
    }
    
    return 0;
  }
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace() override{
    values_vector.resize(count);
    
    for(int i=0; i< count; i++){
      values_vector[i] = start + i*step;
    }
    return values_vector;
  };
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    type = DomainType::HYPER_SLAB_DOMAIN_TYPE;
    xmlNodePtr domain_ptr = Domain::Serialize(parent);

    return domain_ptr;
  };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    assert(data_items.size() >= 1);
    SetParent(_parent);
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
    
    assert(physical.dimensions[0]==3);
    
    std::string s = physical.text;
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
    count = stoi(tokens[2]);

    return 0;
  };
  
  virtual std::string GetClassName() const override { return "HyperSlabDomain"; };
  
private:
  double start = 0;
  double step  = 0;
  int    count = 0;
  
  IndexSpace<PHY_TYPE> values_vector;


};
  

}
#endif
