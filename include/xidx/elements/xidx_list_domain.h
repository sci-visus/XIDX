#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include "xidx_domain.h"

namespace xidx{

class ListDomain : public HyperSlabDomain{

public:
  
  ListDomain(std::string _name) : HyperSlabDomain(_name) {};
  
  int AddDomainItem(double phy, int32_t log){
    physical.text += string_format("%f ", phy);
    logical.text  += string_format("%d ", log);
  }
};

}
#endif
