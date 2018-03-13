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
//      data_items.push_back(DataItem(arg_name, this));
      printf("adding %s\n", arg_name.c_str());
    }
    
    va_end(args);
  };
  
//  MultiAxisDomain(const Domain* c) : Domain(c){
//    type = DomainType::DOUBLE_LIST_DOMAIN_TYPE;
//    
//    name = c->name;
//    data_items = c->data_items;
//  };
  
  int SetAxis(int index, ListDomain<T>& list){
    assert(index < list.data_items.size());
    
    lists[index] = list;
    //data_items[index] = list.data_items[0];
    return 0;
  }
  
  int AddAxis(ListDomain<T>& list){
    lists.push_back(list);
    //data_items.push_back(list.data_items[0]);
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    for(auto& l: lists){
     xmlNodePtr list_node = l.Serialize(parent);
    }
    
    return parent;
  }
  
private:
  std::vector<ListDomain<T>> lists;
};

}
#endif
