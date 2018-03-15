#ifndef XIDX_MULTIAXIS_DOMAIN_H_
#define XIDX_MULTIAXIS_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

template<typename T>
class MultiAxisDomain : public Domain{

public:
  
  MultiAxisDomain(std::string _name) : Domain(_name) {
    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
  };

  MultiAxisDomain(std::string _name, int count, const char* axis_names...) : Domain(_name) {
    type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    va_list args;
    va_start(args, axis_names);

    lists.resize(count);
    for (int i = 0; i < count; ++i) {
      std::string arg_name = std::string(va_arg(args, const char*));
      lists[i]= ListDomain<T>(arg_name, this);
    }
    
    va_end(args);
  };
  
  MultiAxisDomain(const MultiAxisDomain* d) : Domain(d->name){
    lists = d->lists;
  }
  
  int SetAxis(int index, ListDomain<T>& list){
    assert(index < list.data_items.size());
    
    lists[index] = list;
    lists[index].parent = this;
    lists[index].type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    return 0;
  }
  
  int AddAxis(ListDomain<T>& list){
    lists.push_back(list);
    lists.back().parent = this;
    lists.back().type = DomainType::MULTIAXIS_DOMAIN_TYPE;
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    xmlNodePtr domain_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(domain_node, BAD_CAST "Type", BAD_CAST ToString(type));
    
    for(auto& l: lists){
      
      for(int i=0; i< l.data_items.size(); i++){
        if(data_items.size() >= l.data_items.size())
          data_items[i] = l.data_items[i];
        else {
          DataItem item(this);
          data_items.push_back(item);
        }
        
        DataItem& item = data_items[i];
        
        for(auto phy: l.values_vector)
          item.text+=std::to_string(phy)+" ";
          
        item.dimensions=std::to_string(l.values_vector.size());
        data_items[i] = item;
      }
    }
    
    for(auto item: data_items)
      item.Serialize(domain_node);
    
    return parent;
  }
  
  virtual std::string GetClassName() override { return "MultiAxisDomain"; };
  
private:
  std::vector<ListDomain<T>> lists;
};

}
#endif
