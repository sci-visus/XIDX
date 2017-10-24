
#include <cassert>
#include <map>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata.h"
#include "idx_metadata_simple_layout.h"
#include "idx_metadata_parse_utils.h"

using namespace std;
using namespace idx_metadata;

std::string IDX_Metadata_Simple_Layout::get_idx_file_path(int timestep, int level, CenterType ctype){
    return metadata->get_md_file_path()+generate_vars_filename(ctype);
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

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

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

  std::shared_ptr<Level> level = metadata->get_timesteps().begin()->second->get_level(0);

  level->objToXML(main_grid_node);
  
  // Set Time series
  xmlNodePtr time_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(time_grid_node, BAD_CAST "Name", BAD_CAST "TimeSeries");
  xmlNewProp(time_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(time_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::TEMPORAL_COLLECTION_TYPE));
  
  Time& metadata_time = metadata->get_time();

  if(metadata_time.type == TimeType::SINGLE_TIME_TYPE){
    for(auto it_ts : metadata->get_timesteps()){
      shared_ptr<TimeStep> curr_grid = it_ts.second;

      xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
      xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format(IDX_METADATA_TIME_FORMAT,it_ts.first).c_str());
      xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
      xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

      Time t;
      t.value = curr_grid->get_physical_time_str();
      t.information.push_back(curr_grid->get_log_time_info());
      xmlNodePtr time_node = t.objToXML(curr_time_node);

      xmlNodePtr xgrids_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
      xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Grid)");
    }
  }else if(metadata_time.type == TimeType::HYPER_SLAB_TIME_TYPE){

    xmlNodePtr time_node = metadata_time.objToXML(time_grid_node);

    xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
    xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format(IDX_METADATA_TIME_FORMAT,0).c_str());
    xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
    xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

    xmlNodePtr single_time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
    std::string init_time = std::string(metadata_time.items[0].text);
    size_t found=init_time.find_first_of(" \\");

    init_time=init_time.substr(0,found);
    xmlNewProp(single_time_node, BAD_CAST "Value", BAD_CAST init_time.c_str());

    xmlNodePtr xgrids_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Grid)");

  }

  /* 
   * Dumping document to stdio or file
   */
  xmlSaveFormatFileEnc(metadata->get_md_file_path().c_str(), doc, "UTF-8", 1);

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

  doc = xmlReadFile(metadata->get_md_file_path().c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", metadata->get_md_file_path().c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);

  if(!root_element || !(root_element->children) || !(root_element->children->next))
    return 1;

  xmlNode *space_grid = root_element->children->next->children->next->children;

  xmlNode *time_grid = root_element->children->next->children->next->next->next->children;

  std::shared_ptr<Level> lvl(new Level());
  lvl->XMLToObj(space_grid);

  // Time
  for (xmlNode* cur_node = time_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE && is_node_name(cur_node,"Grid")) {
      std::shared_ptr<TimeStep> ts(new TimeStep());

      int log_time = -1;
      double phy_time = -1.0;

      for (xmlNode* cur_time_node = cur_node->children; cur_time_node; cur_time_node = cur_time_node->next){ 
        if(cur_time_node->type == XML_ELEMENT_NODE && is_node_name(cur_time_node,"Time")){
          Time t;
          t.XMLToObj(cur_time_node);
          phy_time = stod(t.value);

          for(auto info:t.information)
            if(strcmp(info.name.c_str(), "LogicalTime")==0)
              log_time = stoi(info.value);
        }
      }
      //printf("timestep %d %f\n", log_time, phy_time);
      ts->set_timestep(log_time, phy_time);

      ts->add_level(lvl);
      metadata->add_timestep(ts);
    }
    else if(cur_node->type == XML_ELEMENT_NODE && is_node_name(cur_node,"Time")){

      metadata->get_time().XMLToObj(cur_node);

    }
  }

  xmlFreeDoc(doc);

  return 0;
}