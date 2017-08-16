
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata_parse_utils.h"
#include "idx_metadata_attribute.h"

#define MY_ENCODING "ISO-8859-1"

namespace idx_metadata{

int parse_geometry(xmlNode *geo_node, Geometry& geometry){
  const char* geo_type = getProp(geo_node, "GeometryType");

  for(int t=GeometryType::XYZ_GEOMETRY_TYPE; t <= ORIGIN_DXDY_GEOMETRY_TYPE; t++)
    if (strcmp(geo_type, ToString(static_cast<GeometryType>(t)))==0)
        geometry.geometryType = static_cast<GeometryType>(t);

  int data_items_count = 0;
  for (xmlNode* inner_node = geo_node->children->next; inner_node; inner_node = inner_node->next) {
    if(is_node_name(inner_node, "DataItem")){
      xmlNode* item_o_node = inner_node;
      
      DataItem item_o;
      item_o.formatType = FormatType::XML_FORMAT;
      
      item_o.dimensions = getProp(item_o_node, "Dimensions");
      item_o.text = reinterpret_cast<const char*>(item_o_node->children->content);

      geometry.items.push_back(item_o);
      data_items_count++;
    }
  }

  assert(data_items_count == 2);
  // printf("add geometry type %s dim %s O %s D %s \n", 
  //         ToString(geometry.geometryType), geometry.item[0].dimensions.c_str(), geometry.item[0].text.c_str(), geometry.item[1].text.c_str());

  return 0;
}

int parse_topology(xmlNode* topo_node, Topology& topology){
  const char* topo_type = getProp(topo_node, "TopologyType");

  for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
    if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
        topology.topologyType = static_cast<TopologyType>(t);

  topology.dimensions = getProp(topo_node, "Dimensions");

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

        // att.name = getProp(cur_node, "Name");

        // const char* center_type = getProp(cur_node, "Center");
        // for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
        //   if (strcmp(center_type, ToString(static_cast<CenterType>(t)))==0)
        //       att.centerType = static_cast<CenterType>(t);

        // const char* att_type = getProp(cur_node, "AttributeType");
        // for(int t=AttributeType::SCALAR_ATTRIBUTE_TYPE; t <= TENSOR_ATTRIBUTE_TYPE; t++)
        //   if (strcmp(att_type, ToString(static_cast<AttributeType>(t)))==0)
        //       att.attributeType = static_cast<AttributeType>(t);

        // for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
          
        //   if(is_node_name(inner_node, "DataItem")){
        //     xmlNode* item = inner_node;

        //     att.data.formatType = FormatType::IDX_FORMAT;
            
        //     const char* num_type = getProp(item, "NumberType");
        //     for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++)
        //       if (strcmp(num_type, ToString(static_cast<NumberType>(t)))==0)
        //           att.data.numberType = static_cast<NumberType>(t);
            
        //     att.data.precision = getProp(item, "Precision");
        //     att.data.dimensions = getProp(item, "Dimensions");

        //     const char* end_type = getProp(item, "Endian");
        //     for(int t=EndianType::LITTLE_ENDIANESS; t <= NATIVE_ENDIANESS; t++)
        //       if (strcmp(end_type, ToString(static_cast<EndianType>(t)))==0)
        //           att.data.endianType = static_cast<EndianType>(t);

        //     // printf("add attribute %s cent %s type %s num_type %s prec %s dim %s endian %s\n",
        //     //   att.name.c_str(), ToString(att.centerType), ToString(att.attributeType),
        //     //   ToString(att.data.numberType), att.data.precision.c_str(), att.data.dimensions.c_str(),
        //     //   ToString(att.data.endianType));

        //   }else if(is_node_name(inner_node, "Information")){
        //     xmlNode* item = inner_node;
        //     att.information.push_back(Information(getProp(item, "Name"),getProp(item, "Value")));
        //   }
        // }

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