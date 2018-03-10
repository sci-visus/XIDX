#ifndef XIDX_DOMAIN_H_
#define XIDX_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class Domain : public Parsable{

public:
  
  Domain(){};
  
  Domain(const Domain& c) {
    name = c.name;
  };
  
  Domain(std::string _name) { name=_name; };
  
  DomainType type;
  std::vector<std::shared_ptr<DataItem> > data_items;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    //Parsable::Serialize(parent);

    xmlNodePtr curr_grid_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(curr_grid_node, BAD_CAST "DomainType", BAD_CAST ToString(type));

    for(auto item: data_items)
      xmlNodePtr item_node = item->Serialize(curr_grid_node);

    return parent;
  };
  
  int Deserialize(xmlNodePtr node){
    //Parsable::Deserialize(node); // TODO use the parent class to serialize name??

    const char* domain_type = GetProp(node, "DomainType");

    for(int t=DomainType::SINGLE_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
      if (strcmp(domain_type, ToString(static_cast<DomainType>(t)))==0)
          type = static_cast<DomainType>(t);

    return 0;
  };

};

}
#endif
