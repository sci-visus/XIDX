#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

template<typename T>
class ListDomain : public Domain{

public:
  
  ListDomain(std::string _name) : Domain(_name) {
    data_items.push_back(DataItem(name, this));
  };
  
  ListDomain(const ListDomain& c) : Domain(c){
    name = c.name;
    data_items = c.data_items;
    values_vector = c.values_vector;
  };
  
  int AddDomainItem(T phy){
    values_vector.push_back(phy);
  
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    DataItem& physical = data_items[0];
    
    for(auto phy: values_vector)
      physical.text+=std::to_string(phy)+" ";
    
    physical.dimensions=std::to_string(values_vector.size());
    
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
      
    return domain_node;
  }
  
private:
  std::vector<T> values_vector;

};

}
#endif
