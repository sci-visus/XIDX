#ifndef XIDX_TOPOLOGY_H_
#define XIDX_TOPOLOGY_H_

#include "xidx/xidx.h"

namespace xidx{

class Topology : public Parsable{

public:

  std::vector<Attribute> attributes;
  std::vector<DataItem> items;
  TopologyType type;
  std::vector<INDEX_TYPE> dimensions;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{

    xmlNodePtr topology_node = xmlNewChild(parent, NULL, BAD_CAST "Topology", NULL);

    xmlNewProp(topology_node, BAD_CAST "Type", BAD_CAST ToString(type));
    xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST ToString(dimensions).c_str());
    
    for(auto item: items)
      xmlNodePtr item_node = item.Serialize(topology_node);

    return topology_node;
  };
  
  int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    if(!IsNodeName(node,"Topology"))
      return -1;
    
    SetParent(_parent);

    const char* topo_type = GetProp(node, "Type");

    for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
      if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
          type = static_cast<TopologyType>(t);

    dimensions = ToIndexVector(GetProp(node, "Dimensions"));

    return 0;
  };
  
  virtual std::string GetClassName() const override { return "Topology"; };

};

}

#endif
