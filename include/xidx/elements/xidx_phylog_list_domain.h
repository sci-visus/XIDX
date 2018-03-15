#ifndef XIDX_PHYLOG_LIST_DOMAIN_H_
#define XIDX_PHYLOG_LIST_DOMAIN_H_

#include <sstream>
#include "xidx/xidx.h"

namespace xidx{
  
class PhyLogListDomain : public ListDomain<double>{

public:
  
  PhyLogListDomain(std::string _name) : ListDomain("Physical") {
    type = DomainType::PHYLOG_LIST_DOMAIN_TYPE;
    
    data_items.push_back(DataItem("Logical", this));
  };
  
  PhyLogListDomain(const ListDomain* c) : ListDomain("Physical"){
    type = DomainType::PHYLOG_LIST_DOMAIN_TYPE;
    
    name = c->name;
    data_items = c->data_items;
    
    if(data_items.size() < 2)
      data_items.push_back(DataItem("Logical", this));
  };
  
  int AddDomainItem(int32_t log, double phy){
    log_vector.push_back(log);
    phy_vector.push_back(phy);
  
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() == 2);
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    physical.text="";
    logical.text="";
    
    for(auto phy: phy_vector)
      physical.text+=std::to_string(phy)+" ";
     
    for(auto log: log_vector)
      logical.text+=std::to_string(log)+" ";
    
    physical.dimensions=std::to_string(phy_vector.size());
    logical.dimensions=std::to_string(log_vector.size());
    
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
      
    return domain_node;
  }
  
  virtual int Deserialize(xmlNodePtr node){
    //Parsable::Deserialize(node); // TODO use the parent class to serialize name??
    
    Domain::Deserialize(node);
    
    DataItem& physical = data_items[0];
    DataItem& logical = data_items[1];
    
    int length = stoi(physical.dimensions);
    
    std::stringstream stream_log(logical.text);
    std::stringstream stream_phy(physical.text);
    
    phy_vector.resize(length);
    log_vector.resize(length);
    for(int i=0; i< length; i++){
      stream_phy >> phy_vector[i];
      stream_log >> log_vector[i];
    }
    
    return 0;
  };

  
  virtual std::string GetClassName() override { return "PhyLogListDomain"; };
  
private:
  std::vector<int> log_vector;
  std::vector<double> phy_vector;
};

}
#endif
