#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class ListDomain : public HyperSlabDomain{

public:
  
  ListDomain(std::string _name) : HyperSlabDomain(_name) { };
  
  ListDomain(const ListDomain* c) : HyperSlabDomain(c){
    name = c->name;
    data_items = c->data_items;
  };
  
  int AddDomainItem(double phy){
    phy_vector.push_back(phy);
  
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    DataItem& physical = data_items[0];
    
    for(auto phy: phy_vector)
      physical.text+=std::to_string(phy)+" ";
    
    physical.dimensions=std::to_string(phy_vector.size());
    
    xmlNodePtr domain_node = HyperSlabDomain::Serialize(parent, text);
      
    return domain_node;
  }
  
private:
  std::vector<double> phy_vector;

};

}
#endif
