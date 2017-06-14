
#include <cassert>
#include <map>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata.h"

#define MY_ENCODING "ISO-8859-1"

using namespace std;
using namespace idx_metadata;

const char* getProp(xmlNode* node, string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

void print_node(xmlNode* node){
  printf("node name %s Name=%s\n", node->name, getProp(node,"Name"));
}

inline bool is_node_name(xmlNode* node, string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

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
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");

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

int save_hpc_level(shared_ptr<Level> lvl, int n, shared_ptr<TimeStep> ts, const char* time_path){
  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
  char buff[256];
  int i, j;

  LIBXML_TEST_VERSION;

  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xdmf");
  xmlDocSetRootElement(doc, root_node);
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");

  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../Xdmf.dtd");

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

   xmlNodePtr main_grid_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(main_grid_node, BAD_CAST "Name", BAD_CAST "Grids");
  xmlNewProp(main_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(main_grid_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  char level_name[128];
  sprintf(level_name,"l%04d", n);

  char mkdir_cmd[150];
  sprintf(mkdir_cmd,"mkdir -p %s/%s", time_path, level_name);
  const int dir_err = system(mkdir_cmd);
  if (-1 == dir_err){
      fprintf(stderr, "Error creating directory %s!\n", level_name);
      exit(1);
  }

  Grid grid = lvl->get_datagrid(0)->get_grid();

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

  for(int i=0; i<lvl->get_n_datagrids(); i++){
    Grid& curr_grid = lvl->get_datagrid(i)->get_grid();
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
  
  xmlNodePtr curr_time_node = xmlNewChild(time_grid_node, NULL, BAD_CAST "Grid", NULL);
  xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format("t_%09d",i).c_str());
  xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  xmlNodePtr info_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Information", NULL);
  xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST ts->get_log_time_info().name.c_str());
  xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST ts->get_log_time_info().value.c_str());

  xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
  xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST ts->get_time().value.c_str());

  xmlNodePtr xgrids_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "xi:include", NULL);
  xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1]/Grid)");
  
  char level_path[128];
  sprintf(level_path,"%s/l%04d/meta.xmf", time_path, n);
  xmlSaveFormatFileEnc(level_path, doc, "UTF-8", 1);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

  return 0;
}


int save_hpc_timestep(shared_ptr<TimeStep> ts){
  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
  char buff[256];
  int i, j;

  LIBXML_TEST_VERSION;

  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xdmf");
  xmlDocSetRootElement(doc, root_node);
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");

  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../Xdmf.dtd");

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

  xmlNodePtr curr_time_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  char time_name[128];
  sprintf(time_name,"t%09d", ts->get_logical_time());

  char mkdir_cmd[150];
  sprintf(mkdir_cmd,"mkdir -p %s", time_name);
  const int dir_err = system(mkdir_cmd);
  if (-1 == dir_err){
      fprintf(stderr, "Error creating directory %s!\n", time_name);
      exit(1);
  }

  xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST time_name);
  xmlNewProp(curr_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  xmlNodePtr info_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Information", NULL);
  xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST ts->get_log_time_info().name.c_str());
  xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST ts->get_log_time_info().value.c_str());

  xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
  xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST ts->get_time().value.c_str());

  xmlNodePtr levels_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
  xmlNewProp(levels_node, BAD_CAST "Name", BAD_CAST "Grids");
  xmlNewProp(levels_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(levels_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

  for(int l=0; l < ts->get_n_levels(); l++){
    char lname[128];
    sprintf(lname, "l%04d/meta.xmf",l);
    xmlNodePtr level_node = xmlNewChild(levels_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(level_node, BAD_CAST "href", BAD_CAST lname);
    xmlNewProp(level_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid[1])");

    shared_ptr<Level> level = ts->get_level(l);
    save_hpc_level(level, l, ts, time_name);
  }

  char time_path[128];
  sprintf(time_path,"t%09d/meta.xmf", ts->get_logical_time());
  xmlSaveFormatFileEnc(time_path, doc, "UTF-8", 1);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

  return 0;

}

int IDX_Metadata::save_hpc(){

  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
  char buff[256];
  int i, j;

  LIBXML_TEST_VERSION;

  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "Xdmf");
  xmlDocSetRootElement(doc, root_node);
  xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
  xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");

  xmlCreateIntSubset(doc, BAD_CAST "Xdmf", NULL, BAD_CAST "../Xdmf.dtd");

  xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");

  xmlNodePtr domain_node = xmlNewChild(root_node, NULL, BAD_CAST "Domain", NULL);

  xmlNodePtr main_time_node = xmlNewChild(domain_node, NULL, BAD_CAST "Grid", NULL);

  xmlNewProp(main_time_node, BAD_CAST "Name", BAD_CAST "TimeSeries");
  xmlNewProp(main_time_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::COLLECTION_GRID_TYPE));
  xmlNewProp(main_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::TEMPORAL_COLLECTION_TYPE));

  for(int t=0; t<get_n_timesteps(); t++){
    char tname[128];
    sprintf(tname, "t%09d/meta.xmf",t);
    xmlNodePtr xgrids_node = xmlNewChild(main_time_node, NULL, BAD_CAST "xi:include", NULL);
    xmlNewProp(xgrids_node, BAD_CAST "href", BAD_CAST tname);
    xmlNewProp(xgrids_node, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xdmf/Domain/Grid)");
  
    shared_ptr<TimeStep> ts = get_timestep(t);
    save_hpc_timestep(ts);
  }

  xmlSaveFormatFileEnc(file_path.c_str(), doc, "UTF-8", 1);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

  return 0; 
}

int IDX_Metadata::load_hpc_grid(string gpath, shared_ptr<TimeStep> ts){
  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */
  int ret = 0;

  doc = xmlReadFile(gpath.c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", gpath.c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);
  if(!root_element || !(root_element->children))
    return 1;

  xmlNode *grid_node = root_element->children->next->children->next->children;
  
  shared_ptr<Level> lvl(new Level());

  ret = parse_level(grid_node, lvl);
  
  ts->add_level(lvl);

  return ret;
}

int IDX_Metadata::load_hpc_timestep(string& tpath){
  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */
  int ret = 0;

  doc = xmlReadFile(tpath.c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", tpath.c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);
  if(!root_element || !(root_element->children))
    return 1;

  xmlNode *time_grid_node = root_element->children->next->children->next;
  
  std::shared_ptr<TimeStep> ts(new TimeStep());

  xmlNode* log_time_node = time_grid_node->children->next;
  const char* att_name = getProp(log_time_node, "Name");

  int log_time = -1;
  if(strcmp(att_name,"LogicalTime")==0)
    log_time = atoi(getProp(log_time_node, "Value"));
  else
    fprintf(stderr, "LogicalTime attribute not found\n");

  xmlNode* phy_time_node = log_time_node->next->next;

  double phy_time = stod(getProp(phy_time_node, "Value"));

  printf("load timestep %d %f\n", log_time, phy_time);
  ts->set_timestep(log_time, phy_time);

  xmlNode* grids_node = phy_time_node->next->next->children->next;

  for (xmlNode* cur_node = grids_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      const char* href = getProp(cur_node, "href");

      size_t found=tpath.find_last_of("/\\");

      string path=tpath.substr(0,found+1) + string(href);
      ret += load_hpc_grid(path, ts);

    }
  }

  add_timestep(ts);

  xmlFreeDoc(doc);

  return ret;
}

int IDX_Metadata::load_hpc(){
  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */
  int ret = 0;

  doc = xmlReadFile(file_path.c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", file_path.c_str());
    return 1;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);

  if(!root_element || !(root_element->children) || !(root_element->children->next))
    return 1;

  xmlNode *time_grid = root_element->children->next->children->next->children;

  for (xmlNode* cur_node = time_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {

      const char* href = getProp(cur_node, "href");

      size_t found=file_path.find_last_of("/\\");

      string path=file_path.substr(0,found+1) + string(href);
      ret += load_hpc_timestep(path);
    }
  }

  xmlFreeDoc(doc);

  return ret;
}

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

      geometry.item.push_back(item_o);
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


int IDX_Metadata::parse_level(xmlNode *space_grid, std::shared_ptr<Level> lvl){
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

        att.name = getProp(cur_node, "Name");

        const char* center_type = getProp(cur_node, "Center");
        for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
          if (strcmp(center_type, ToString(static_cast<CenterType>(t)))==0)
              att.centerType = static_cast<CenterType>(t);

        const char* att_type = getProp(cur_node, "AttributeType");
        for(int t=AttributeType::SCALAR_ATTRIBUTE_TYPE; t <= TENSOR_ATTRIBUTE_TYPE; t++)
          if (strcmp(att_type, ToString(static_cast<AttributeType>(t)))==0)
              att.attributeType = static_cast<AttributeType>(t);

        for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
          
          if(is_node_name(inner_node, "DataItem")){
            xmlNode* item = inner_node;

            att.data.formatType = FormatType::IDX_FORMAT;
            
            const char* num_type = getProp(item, "NumberType");
            for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++)
              if (strcmp(num_type, ToString(static_cast<NumberType>(t)))==0)
                  att.data.numberType = static_cast<NumberType>(t);
            
            att.data.precision = getProp(item, "Precision");
            att.data.dimensions = getProp(item, "Dimensions");

            const char* end_type = getProp(item, "Endian");
            for(int t=EndianType::LITTLE_ENDIANESS; t <= NATIVE_ENDIANESS; t++)
              if (strcmp(end_type, ToString(static_cast<EndianType>(t)))==0)
                  att.data.endianType = static_cast<EndianType>(t);

            // printf("add attribute %s cent %s type %s num_type %s prec %s dim %s endian %s\n",
            //   att.name.c_str(), ToString(att.centerType), ToString(att.attributeType),
            //   ToString(att.data.numberType), att.data.precision.c_str(), att.data.dimensions.c_str(),
            //   ToString(att.data.endianType));

          }
        }

        // TODO do not assume every grid has same attributes, resolve xpointers
        for(int i=0; i < lvl->get_n_datagrids(); i++){
          lvl->get_datagrid(i)->add_attribute(att);
        }
      }

    }
  }

  return 0;
}

int IDX_Metadata::load_simple(){

  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */

  doc = xmlReadFile(file_path.c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", file_path.c_str());
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
      add_timestep(ts);
    }
  }

  xmlFreeDoc(doc);

  return 0;
}