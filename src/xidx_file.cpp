
#include <cassert>
#include <map>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx.h"

using namespace xidx;

// TODO add vector default types
const std::string XidxDataType::FLOAT_32="1*float32";
const std::string XidxDataType::FLOAT_64="1*float64";
const std::string XidxDataType::INT_32="1*int32";
const std::string XidxDataType::UINT_32="1*uint32";
const std::string XidxDataType::INT_16="1*int16";
const std::string XidxDataType::UINT_16="1*int16";
const std::string XidxDataType::INT_8="1*int8";
const std::string XidxDataType::UINT_8="1*uint8";


namespace std{
  std::string to_string(xidx::DataSource const& d){
    return "";
  }
}

// std::string XidsFile::get_idx_file_path(int timestep, int level, CenterType ctype){
//     return metadata->get_md_file_path()+generate_vars_filename(ctype);
// }

int XidxFile::save(){

  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */

  LIBXML_TEST_VERSION;

  /* 
   * Creates a new document, a node and set it as a root node
   */
  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xidx");
  xmlDocSetRootElement(doc, root_node);
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");

  // xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  /*
   * Creates a DTD declaration. Isn't mandatory. 
   */
  xmlCreateIntSubset(doc, BAD_CAST "Xidx", NULL, BAD_CAST "Xidx.dtd");

  root_group->Serialize(root_node);

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

int XidxFile::load(){

  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */

  doc = xmlReadFile(file_path.c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", file_path.c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);

  // if(!root_element || !(root_element->children) || !(root_element->children->next))
  //   return 1;

  // xmlNode *space_grid = root_element->children->next->children->next->children;

  // xmlNode *time_grid = root_element->children->next->children->next->next->next->children;

  // std::shared_ptr<Level> lvl(new Level());
  // lvl->XMLToObj(space_grid);

  // // Time
  // for (xmlNode* cur_node = time_grid; cur_node; cur_node = cur_node->next) {
  //   if (cur_node->type == XML_ELEMENT_NODE && is_node_name(cur_node,"Grid")) {
  //     std::shared_ptr<TimeStep> ts(new TimeStep());

  //     int log_time = -1;
  //     double phy_time = -1.0;

  //     for (xmlNode* cur_time_node = cur_node->children; cur_time_node; cur_time_node = cur_time_node->next){ 
  //       if(cur_time_node->type == XML_ELEMENT_NODE && is_node_name(cur_time_node,"Time")){
  //         Time t;
  //         t.XMLToObj(cur_time_node);
  //         phy_time = stod(t.value);

  //         for(auto info:t.information)
  //           if(strcmp(info.name.c_str(), "LogicalTime")==0)
  //             log_time = stoi(info.value);
  //       }
  //     }
  //     //printf("timestep %d %f\n", log_time, phy_time);
  //     ts->set_timestep(log_time, phy_time);

  //     ts->add_level(lvl);
  //     metadata->add_timestep(ts);
  //   }
  //   else if(cur_node->type == XML_ELEMENT_NODE && is_node_name(cur_node,"Time")){

  //     metadata->get_time().XMLToObj(cur_node);

  //   }
  // }

  xmlFreeDoc(doc);

  return 0;
}
