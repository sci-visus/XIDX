#ifndef XIDX_GRID_H_
#define XIDX_GRID_H_

#include "xidx_parsable.h"

namespace xidx{

class Grid : public xidx::Parsable{

public:

  std::string name;
  GridType gridType;
  CollectionType collectionType;
  Topology topology;
  Geometry geometry;
  std::vector<Attribute> attribute;
  Time time;
  std::vector<Grid> grid;
  std::vector<Information> information;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){

    xmlNodePtr grid_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(grid_node, BAD_CAST "GridType", BAD_CAST ToString(gridType));
    xmlNewProp(grid_node, BAD_CAST "Name", BAD_CAST name.c_str());

    if(gridType == GridType::UNIFORM_GRID_TYPE){
      xmlNodePtr topology_node = topology.objToXML(grid_node);
      xmlNodePtr geometry_node = geometry.objToXML(grid_node);
    }else if(gridType == GridType::COLLECTION_GRID_TYPE){

      if(gridType == CollectionType::TEMPORAL_COLLECTION_TYPE){
        xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
        xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format(xidx_TIME_FORMAT,i).c_str());
        xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
        xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

        xmlNodePtr info_node = curr_grid->get_log_time_info().objToXML(curr_time_node);

        xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
        xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST curr_grid->get_physical_time_str());
      }

    }

    return grid_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!xidx::is_node_name(node,"Topology"))
      return -1;

    const char* topo_type = xidx::getProp(node, "TopologyType");

    for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
      if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
          topologyType = static_cast<TopologyType>(t);

    dimensions = xidx::getProp(node, "Dimensions");

    return 0;
  };

};

}

#endif