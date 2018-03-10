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
  
  int AddDomainItem(double phy, int32_t log){
    physical.text += string_format("%f ", phy);
    logical.text  += string_format("%d ", log);
  }
};

}
#endif
