
#include <cassert>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata.h"

#define MY_ENCODING "ISO-8859-1"

using namespace std;
using namespace idx_metadata;

int IDX_Metadata::save_simple(){

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
  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../Xdmf.dtd");

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  /* 
   * xmlNewChild() creates a new node, which is "attached" as child node
   * of root_node node. 
   */
  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

  xmlNodePtr main_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(main_grid_node, BAD_CAST "Name", BAD_CAST "Grids");
  xmlNewProp(main_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(main_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  std::shared_ptr<Level> level = get_timestep(0)->get_level(0);

  Grid grid = level->get_datagrid(0)->get_grid();

  for(auto& curr_attribute : grid.attribute){
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

  for(int i=0; i<level->get_n_datagrids(); i++){
    Grid& curr_grid = level->get_datagrid(i)->get_grid();
    xmlNodePtr curr_grid_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(curr_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::UNIFORM_GRID_TYPE));
    xmlNewProp(curr_grid_node, BAD_CAST "Name", BAD_CAST curr_grid.name.c_str());

    xmlNodePtr topology_node = xmlNewChild(curr_grid_node, NULL, BAD_CAST "Topology", NULL);

    xmlNewProp(topology_node, BAD_CAST "TopologyType", BAD_CAST ToString(curr_grid.topology.topologyType));
    xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST curr_grid.topology.dimensions.c_str());

    xmlNodePtr geometry_node = xmlNewChild(curr_grid_node, NULL, BAD_CAST "Geometry", NULL);
    xmlNewProp(geometry_node, BAD_CAST "GeometryType", BAD_CAST ToString(curr_grid.geometry.geometryType));
    
    xmlNodePtr item_o = xmlNewChild(geometry_node, NULL, BAD_CAST "DataItem", BAD_CAST curr_grid.geometry.item[0].text.c_str());
    xmlNewProp(item_o, BAD_CAST "Format", BAD_CAST ToString(curr_grid.geometry.item[0].formatType));
    xmlNewProp(item_o, BAD_CAST "Dimensions", BAD_CAST curr_grid.geometry.item[0].dimensions.c_str());

    xmlNodePtr item_d = xmlNewChild(geometry_node, NULL, BAD_CAST "DataItem", BAD_CAST curr_grid.geometry.item[1].text.c_str());
    xmlNewProp(item_d, BAD_CAST "Format", BAD_CAST ToString(curr_grid.geometry.item[1].formatType));
    xmlNewProp(item_d, BAD_CAST "Dimensions", BAD_CAST curr_grid.geometry.item[1].dimensions.c_str());

    xmlNodePtr xtopology_node = xmlNewChild(curr_grid_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xtopology_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Attribute)");

  }
  
  // Set Time series
  xmlNodePtr time_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(time_grid_node, BAD_CAST "Name", BAD_CAST "TimeSeries");
  xmlNewProp(time_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(time_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::TEMPORAL_COLLECTION_TYPE));
  
  for(int i=0; i<get_n_timesteps(); i++){
    shared_ptr<TimeStep> curr_grid = get_timestep(i);

    xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format("t_%09d",i).c_str());
    xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
    xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

    xmlNodePtr info_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Information", NULL);
    xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST curr_grid->log_time_info.name.c_str());
    xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST curr_grid->log_time_info.value.c_str());

    xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
    xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST curr_grid->time.value.c_str());

    xmlNodePtr xgrids_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Grid)");
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