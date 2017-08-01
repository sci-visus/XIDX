
#include <cassert>
#include <map>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata.h"
#include "idx_metadata_simple_layout.h"
#include "idx_metadata_parse_utils.h"

using namespace std;
using namespace idx_metadata;

std::string IDX_Metadata_Simple_Layout::get_file_path(int timestep, int level, CenterType ctype){
    return metadata->get_file_path()+generate_vars_filename(ctype);
}

int IDX_Metadata_Simple_Layout::save(){

  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */

  LIBXML_TEST_VERSION;

  /* 
   * Creates a new document, a node and set it as a root node
   */
  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xdmf");
  xmlDocSetRootElement(doc, root_node);
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");
  xmlNewProp(root_node, BAD_CAST "Layout", BAD_CAST "Simple");

  /*
   * Creates a DTD declaration. Isn't mandatory. 
   */
  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../Xdmf.dtd");

  /* 
   * xmlNewChild() creates a new node, which is "attached" as child node
   * of root_node node. 
   */
  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

  xmlNodePtr main_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(main_grid_node, BAD_CAST "Name", BAD_CAST "Grids");
  xmlNewProp(main_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(main_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  std::shared_ptr<Level> level = metadata->get_timestep(0)->get_level(0);

  Grid grid = level->get_datagrid(0)->get_grid();

  for(auto& curr_attribute : grid.attribute){
    xmlNodePtr attribute_node = xmlNewChild(main_grid_node, NULL, BAD_CAST "Attribute", NULL);
    xmlNewProp(attribute_node, BAD_CAST "Name", BAD_CAST curr_attribute.name.c_str());
    xmlNewProp(attribute_node, BAD_CAST "Center", BAD_CAST ToString(curr_attribute.centerType));
    xmlNewProp(attribute_node, BAD_CAST "AttributeType", BAD_CAST ToString(curr_attribute.attributeType));

    xmlNodePtr data_node = xmlNewChild(attribute_node, NULL, BAD_CAST "DataItem", BAD_CAST generate_vars_filename(curr_attribute.centerType).c_str());
    xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(curr_attribute.data.formatType));
    xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(curr_attribute.data.numberType));
    xmlNewProp(data_node, BAD_CAST "Precision", BAD_CAST curr_attribute.data.precision.c_str());
    xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(curr_attribute.data.endianType));
    xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST curr_attribute.data.dimensions.c_str());

    for(auto& curr_info : curr_attribute.information){
      xmlNodePtr info_node = xmlNewChild(attribute_node, NULL, BAD_CAST "Information", NULL);
      xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST curr_info.name.c_str());
      xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST curr_info.value.c_str());
    }

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
  
  for(int i=0; i < metadata->get_n_timesteps(); i++){
    shared_ptr<TimeStep> curr_grid = metadata->get_timestep(i);

    xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format(IDX_METADATA_TIME_FORMAT,i).c_str());
    xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
    xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

    xmlNodePtr info_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Information", NULL);
    xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST curr_grid->get_log_time_info().name.c_str());
    xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST curr_grid->get_log_time_info().value.c_str());

    xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
    xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST curr_grid->get_time().value.c_str());

    xmlNodePtr xgrids_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Grid)");
  }

  /* 
   * Dumping document to stdio or file
   */
  xmlSaveFormatFileEnc(metadata->get_file_path().c_str(), doc, "UTF-8", 1);

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

int IDX_Metadata_Simple_Layout::load(){

  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */

  doc = xmlReadFile(metadata->get_file_path().c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", metadata->get_file_path().c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);

  if(!root_element || !(root_element->children) || !(root_element->children->next))
    return 1;

  xmlNode *space_grid = root_element->children->next->children->next->children;

  xmlNode *time_grid = root_element->children->next->children->next->next->next->children;

  std::shared_ptr<Level> lvl(new Level());

  parse_level(space_grid, lvl);

  // Time
  for (xmlNode* cur_node = time_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      std::shared_ptr<TimeStep> ts(new TimeStep());

      xmlNode* log_time_node = cur_node->children->next;
      const char* att_name = getProp(log_time_node, "Name");

      int log_time = -1;
      if(strcmp(att_name,"LogicalTime")==0)
        log_time = atoi(getProp(log_time_node, "Value"));
      else
        fprintf(stderr, "LogicalTime attribute not found\n");

      xmlNode* phy_time_node = log_time_node->next->next;

      double phy_time = stod(getProp(phy_time_node, "Value"));

      //printf("timestep %d %f\n", log_time, phy_time);
      ts->set_timestep(log_time, phy_time);

      ts->add_level(lvl);
      metadata->add_timestep(ts);
    }
  }

  xmlFreeDoc(doc);

  return 0;
}