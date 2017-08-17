#ifndef IDX_METADATA_TOPOLOGY_H_
#define IDX_METADATA_TOPOLOGY_H_

#include "idx_metadata_parsable.h"
#include "idx_metadata_information.h"
#include "idx_metadata_dataitem.h"

namespace idx_metadata{

class Topology : public idx_metadata::Parsable{

public:

  std::vector<Information> information;
  std::vector<DataItem> items;
  TopologyType topologyType;
  std::string dimensions;
  std::string order;
  std::string nodesPerElement;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){

    xmlNodePtr topology_node = xmlNewChild(parent, NULL, BAD_CAST "Topology", NULL);

    xmlNewProp(topology_node, BAD_CAST "TopologyType", BAD_CAST ToString(topologyType));
    xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());
    
    for(auto item: items)
      xmlNodePtr item_node = item.objToXML(topology_node, item.text.c_str());

    return topology_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"Topology"))
      return -1;

    const char* topo_type = idx_metadata::getProp(node, "TopologyType");

    for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
      if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
          topologyType = static_cast<TopologyType>(t);

    dimensions = idx_metadata::getProp(node, "Dimensions");

    return 0;
  };

};

}

#endif