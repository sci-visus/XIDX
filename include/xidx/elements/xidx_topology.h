#ifndef XIDX_TOPOLOGY_H_
#define XIDX_TOPOLOGY_H_

#include "xidx_parsable.h"
#include "xidx_attribute.h"
#include "xidx_dataitem.h"

namespace xidx{

class Topology : public xidx::Parsable{

public:

  std::vector<Attribute> attributes;
  std::vector<DataItem> items;
  TopologyType topologyType;
  std::string dimensions;
  std::string order;
  std::string nodesPerElement;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr topology_node = xmlNewChild(parent, NULL, BAD_CAST "Topology", NULL);

    xmlNewProp(topology_node, BAD_CAST "TopologyType", BAD_CAST ToString(topologyType));
    xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());
    
    for(auto item: items)
      xmlNodePtr item_node = item.Serialize(topology_node, item.text.c_str());

    return topology_node;
  };
  
  int Deserialize(xmlNodePtr node){
    if(!xidx::IsNodeName(node,"Topology"))
      return -1;

    const char* topo_type = xidx::GetProp(node, "TopologyType");

    for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
      if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
          topologyType = static_cast<TopologyType>(t);

    dimensions = xidx::GetProp(node, "Dimensions");

    return 0;
  };

};

}

#endif
