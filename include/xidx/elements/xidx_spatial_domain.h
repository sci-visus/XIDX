#ifndef XIDX_SPATIAL_DOMAIN_H_
#define XIDX_SPATIAL_DOMAIN_H_

#include "xidx_domain.h"
#include "xidx_topology.h"
#include "xidx_geometry.h"

namespace xidx{

class SpatialDomain : public Domain{

public:
  
  SpatialDomain(std::string _name) : Domain(_name){};
  
  Topology topology;
  Geometry geometry;

  xmlNodePtr Serialize(xmlNode* parent){
    Domain::Serialize(parent);

    xmlNodePtr topology_node = topology.Serialize(curr_grid_node);

    xmlNodePtr geometry_node = geometry.Serialize(curr_grid_node);

    return parent;
  };
  
  int SetTopology(TopologyType type, uint32_t dims){
    topology.dimesions = string_format("%d", dims);
    topology.type = type;
  }
  
  int SetTopology(TopologyType type, int n_dims, uint32_t* dims){
    for(int i=0; i< n_dims; i++)
      topology.dimesions = string_format("%d ", dims[i]);
    
    topology.type = type;
  }
  
  int Deserialize(xmlNodePtr node){
    Domain::Deserialize(node);

    for (xmlNode* cur_node = node; cur_node; cur_node = cur_node->next) {
      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (inner_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(inner_node, "Topology")){
            topology.Serialize(inner_node);
          }
          else if(IsNodeName(inner_node, "Geometry")){
            geometry.Serialize(inner_node);
          }
        }
      }
    }

    return 0;
  };

};

}
#endif
