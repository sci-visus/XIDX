#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class ListDomain : public HyperSlabDomain{

public:
  
  ListDomain(std::string _name) : HyperSlabDomain(_name) {};
  
  ListDomain(const ListDomain* c) : HyperSlabDomain(c){
    name = c->name;
    physical = c->physical;
    logical = c->logical;
  };
  
  int AddDomainItem(int32_t log, double phy){
    
    log_vector.push_back(log);
    phy_vector.push_back(phy);
    printf("adding to list domain len %lu\n", log_vector.size());
  
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
     
    for(auto phy: phy_vector)
      physical.text+=std::to_string(phy)+" ";
     
    for(auto log: log_vector)
      logical.text+=std::to_string(log)+" ";
    
    physical.dimensions=std::to_string(phy_vector.size());
    logical.dimensions=std::to_string(log_vector.size());
    
    xmlNodePtr domain_node = HyperSlabDomain::Serialize(parent, text);
      
    return domain_node;
  }
  
private:
  std::vector<int> log_vector;
  std::vector<double> phy_vector;

};

}
#endif
