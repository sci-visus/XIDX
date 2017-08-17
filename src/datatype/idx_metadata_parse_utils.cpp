
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata_parse_utils.h"
#include "idx_metadata_attribute.h"

#define MY_ENCODING "ISO-8859-1"

namespace idx_metadata{

int parse_geometry(xmlNode *geo_node, Geometry& geometry){

  geometry.XMLToObj(geo_node);

  // assert(data_items_count == 2);
  // printf("add geometry type %s dim %s O %s D %s \n", 
  //         ToString(geometry.geometryType), geometry.item[0].dimensions.c_str(), geometry.item[0].text.c_str(), geometry.item[1].text.c_str());

  return 0;
}

int parse_topology(xmlNode* topo_node, Topology& topology){
  topology.XMLToObj(topo_node);

  //printf("topo type %s dim %s\n", ToString(topology.topologyType), topology.dimensions.c_str());

  return 0;
}


int parse_level(xmlNode *space_grid, std::shared_ptr<Level> lvl){
  for (xmlNode* cur_node = space_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if(is_node_name(cur_node, "Grid")){
        std::shared_ptr<DataGrid> dgrid(new DataGrid());
        Grid grid;

        grid.name = getProp(cur_node, "Name");

        for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
          if (inner_node->type == XML_ELEMENT_NODE) {
            
            if(is_node_name(inner_node, "Topology")){
              parse_topology(inner_node, grid.topology);
            }
            else if(is_node_name(inner_node, "Geometry")){
              parse_geometry(inner_node, grid.geometry);
            }

          }
        }

        dgrid->set_grid(grid);
        lvl->add_datagrid(dgrid);
      }
    }
  }

  for (xmlNode* cur_node = space_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      
      if(is_node_name(cur_node, "Attribute")){
        Attribute att;

        att.XMLToObj(cur_node);

        // TODO do not assume every grid has same attributes, resolve xpointers
        for(int i=0; i < lvl->get_n_datagrids(); i++){
          lvl->get_datagrid(i)->add_attribute(att);
        }
      }

    }
  }

  return 0;
}

}