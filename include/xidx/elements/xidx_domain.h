#ifndef XIDX_DOMAIN_H_
#define XIDX_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class Domain : public Parsable{

public:
  
  Domain(){};
  
  Domain(const Domain& c) {
    SetParent(c.GetParent());
    name = c.name;
  };
  
  Domain(std::string _name) { name=_name; };
  
  DomainType type;
  std::vector<DataItem > data_items;

  int AddDataItem(DataItem& item){
    data_items.push_back(item);
    return 0;
  }
  
  virtual int AddDataItem(std::string name, Parsable* parent){
    data_items.push_back(DataItem(name, parent));
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    //Parsable::Serialize(parent);

    xmlNodePtr domain_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(domain_node, BAD_CAST "Type", BAD_CAST ToString(type));

    for(auto item: data_items)
      xmlNodePtr item_node = item.Serialize(domain_node);

    return domain_node;
  };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    //Parsable::Deserialize(node); // TODO use the parent class to serialize name??

    SetParent(_parent);
    
    assert(this->GetParent()!=nullptr);
    
    const char* domain_type = GetProp(node, "Type");

    for(int t=DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
      if (strcmp(domain_type, ToString(static_cast<DomainType>(t)))==0)
          type = static_cast<DomainType>(t);
    
    for(int t=DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
      if (strcmp(domain_type, ToString(static_cast<DomainType>(t)))==0)
        type = static_cast<DomainType>(t);
    
    int data_items_count=0;
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      
      if (cur_node->type == XML_ELEMENT_NODE) {
        
        if(IsNodeName(cur_node, "DataItem")){
          if(data_items.size() > data_items_count){
            DataItem& d = data_items[data_items_count];
            d.Deserialize(cur_node, this);
          }
          else{
            DataItem d(this);
            d.Deserialize(cur_node, this);
            data_items.push_back(d);
          }
          
          data_items_count++;
        }
      }
      
    }

    return 0;
  };
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace() = 0;
  
  virtual std::string GetClassName() const override { return "Domain"; };

};

}
#endif
