
#include <cassert>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata.h"

#define MY_ENCODING "ISO-8859-1"

using namespace std;

int IDX_Metadata::set_topology(TopologyType topologyType, const uint32_t* dimensions, Grid* grid){
  Topology topo;
  topo.topologyType = topologyType;

  if(topologyType == TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE)
    topo.dimensions = string_format("%d %d %d", dimensions[0],dimensions[1],dimensions[2]);
  else if(topologyType == TopologyType::CORECT_2D_MESH_TOPOLOGY_TYPE)
    topo.dimensions = string_format("%d %d", dimensions[0],dimensions[1]);
  else{
    fprintf(stderr, "TopologyType not supported\n");
    assert(false);
  }

  if(grid == NULL)
      grid = get_global_main_grid();

  grid->topology = topo;

  return 0;
};

int IDX_Metadata::add_attribute(const char* name, NumberType numberType, short precision, 
                   AttributeType attributeType, CenterType center, EndianType endian, Grid* grid){
  Attribute att;

  att.name = name;
  att.attributeType = attributeType;
  att.centerType = center;
  
  DataItem di;
  di.numberType = numberType;
  di.precision = string_format("%d", precision);
  di.endianType = endian;
  di.dimensions = get_global_main_grid()->topology.dimensions; // Use same dimensions of topology
  di.formatType = FormatType::IDX_FORMAT;

  att.data = di;

  return add_attribute(att, grid);

}

int IDX_Metadata::add_timestep(uint32_t log_time, double phy_time){
  Grid* time_grid = get_global_time_grid();

  Grid nt;
  Information info_log_time;
  info_log_time.name = "LogicalTime";
  info_log_time.value = string_format("%d", log_time);
  nt.information.push_back(info_log_time);

  nt.time.value = string_format("%f", phy_time);

  time_grid->grid.push_back(nt);

  return 0;
}

int IDX_Metadata::save(){
  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
  char buff[256];
  int i, j;

  LIBXML_TEST_VERSION;

  /* 
   * Creates a new document, a node and set it as a root node
   */
  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xdmf");
  xmlDocSetRootElement(doc, root_node);

  /*
   * Creates a DTD declaration. Isn't mandatory. 
   */
  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../examples/Xdmf.dtd");

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  /* 
   * xmlNewChild() creates a new node, which is "attached" as child node
   * of root_node node. 
   */
  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

  xmlNodePtr main_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(main_grid_node, BAD_CAST "Name", BAD_CAST "MainGrid");

  xmlNodePtr topology_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Topology", NULL);

  Grid* grid = get_global_main_grid();
  xmlNewProp(topology_node, BAD_CAST "TopologyType", BAD_CAST ToString(grid->topology.topologyType));
  xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST grid->topology.dimensions.c_str());

  xmlNodePtr geometry_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Geometry", NULL);
  xmlNewProp(geometry_node, BAD_CAST "GeometryType", BAD_CAST ToString(grid->geometry.geometryType));
  
  xmlNodePtr item_o = xmlNewChild(geometry_node, NULL, BAD_CAST "DataItem", BAD_CAST grid->geometry.item[0].text.c_str());
  xmlNewProp(item_o, BAD_CAST "Format", BAD_CAST ToString(grid->geometry.item[0].formatType));
  xmlNewProp(item_o, BAD_CAST "Dimensions", BAD_CAST grid->geometry.item[0].dimensions.c_str());

  xmlNodePtr item_d = xmlNewChild(geometry_node, NULL, BAD_CAST "DataItem", BAD_CAST grid->geometry.item[1].text.c_str());
  xmlNewProp(item_d, BAD_CAST "Format", BAD_CAST ToString(grid->geometry.item[1].formatType));
  xmlNewProp(item_d, BAD_CAST "Dimensions", BAD_CAST grid->geometry.item[1].dimensions.c_str());

  for(auto& curr_attribute : grid->attribute){
    xmlNodePtr attribute_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Attribute", NULL);
    xmlNewProp(attribute_node, BAD_CAST "Name", BAD_CAST curr_attribute.name.c_str());
    xmlNewProp(attribute_node, BAD_CAST "Center", BAD_CAST ToString(curr_attribute.centerType));
    xmlNewProp(attribute_node, BAD_CAST "AttributeType", BAD_CAST ToString(curr_attribute.attributeType));

    xmlNodePtr data_node = xmlNewChild(attribute_node, NULL, BAD_CAST "DataItem", BAD_CAST "&main_idx_file;");
    xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(curr_attribute.data.formatType));
    xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(curr_attribute.data.numberType));
    xmlNewProp(data_node, BAD_CAST "Precision", BAD_CAST curr_attribute.data.precision.c_str());
    xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(curr_attribute.data.endianType));
    xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST curr_attribute.data.dimensions.c_str());
  }

  // Set Time series
  xmlNodePtr time_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(time_grid_node, BAD_CAST "Name", BAD_CAST "TimeSeries");
  xmlNewProp(time_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(time_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::TEMPORAL_COLLECTION_TYPE));

  Grid* time_grid = get_global_time_grid();
  
  for(int i=0; i<time_grid->grid.size(); i++){
    Grid& curr_grid = time_grid->grid[i];

    xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format("t_%09d",i).c_str());
    xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST "Uniform");

    xmlNodePtr info_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Information", NULL);
    xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST curr_grid.information[0].name.c_str());
    xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST curr_grid.information[0].value.c_str());

    xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
    xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST curr_grid.time.value.c_str());

    xmlNodePtr xtopology_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xtopology_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Topology[1])");

    xmlNodePtr xgeometry_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xgeometry_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Geometry[1])");

    xmlNodePtr xattributes_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xattributes_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Attribute)");
  }

  /* 
   * Dumping document to stdio or file
   */
  xmlSaveFormatFileEnc(file_path.c_str(), doc, "UTF-8", 1);

  /*free the document */
  xmlFreeDoc(doc);

  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
  xmlCleanupParser();

  /*
   * this is to debug memory for regression tests
   */
  xmlMemoryDump();

  return 0; 
}