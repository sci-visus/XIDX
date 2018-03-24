#if 0

#ifndef XIDX_PHYLOG_HYPERSLAB_DOMAIN_H_
#define XIDX_PHYLOG_HYPERSLAB_DOMAIN_H_

#include "xidx_domain.h"

namespace xidx{
  
class PhyLogHyperSlabDomain : public HyperSlabDomain{

public:
  
  PhyLogHyperSlabDomain(const HyperSlabDomain* c): HyperSlabDomain(c->name){
    type = DomainType::PHYLOG_HYPER_SLAB_DOMAIN_TYPE;
    name = c->name;
    data_items = c->data_items;
    
    if(data_items.size() < 2)
      data_items.push_back(DataItem("Logical", this));
  };
  
  PhyLogHyperSlabDomain(std::string _name) : HyperSlabDomain(_name) {
    type = DomainType::PHYLOG_HYPER_SLAB_DOMAIN_TYPE;
    
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
    
    phy_vector.resize(dims);
    log_vector.resize(dims);
    for(int i=0; i< dims; i++){
      phy_vector[i] = phy_hyperslab[i];
      log_vector[i] = log_hyperslab[i];
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
    
    int length = stoi(physical.dimensions);
    
    if(physical.format_type == FormatType::XML_FORMAT && physical.format_type == FormatType::XML_FORMAT){
      
      std::stringstream stream_log(logical.text);
      std::stringstream stream_phy(physical.text);
      
      phy_vector.resize(length);
      log_vector.resize(length);
      for(int i=0; i< length; i++){
        stream_phy >> phy_vector[i];
        stream_log >> log_vector[i];
      }
    }
    else{
      fprintf(stderr, "Deserialization of data items for type != XML is not implemented yet!\n");
    }
    
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
  
//  virtual std::vector<INDEX_TYPE>::const_iterator GetLogicalIterator() override{
//    INDEX_TYPE count = log_vector[2]; // last elem of hyperslab element is count
//    log_indices.resize(count);
//    
//    for(INDEX_TYPE i=0; i < count; i++)
//      log_indices[i] = log_vector[0] + i*log_vector[1];
//    
//    return log_indices.begin();
//  }
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace() override{
    INDEX_TYPE count = phy_vector[2]; // last elem of hyperslab element is count
    phy_indices.resize(count);
    
    for(INDEX_TYPE i=0; i < count; i++)
      phy_indices[i] = phy_vector[0] + i*phy_vector[1];
    
    return phy_indices;
  }
  
  virtual std::string GetClassName() override { return "PhyLogHyperSlabDomain"; };

protected:
  std::vector<INDEX_TYPE> log_vector;
  std::vector<PHY_TYPE> phy_vector;
  
  std::vector<INDEX_TYPE> log_indices;
  std::vector<PHY_TYPE> phy_indices;
};

}
#endif

#endif
