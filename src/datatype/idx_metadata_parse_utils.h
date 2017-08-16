

#ifndef IDX_METADATA_PARSE_UTILS_H_
#define IDX_METADATA_PARSE_UTILS_H_

#include <cassert>
#include <string>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata_datatypes.h"

namespace idx_metadata{
class Level;
class Topology;
class Geometry;
}

static const char* getProp2(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

static void print_node(xmlNode* node){
  printf("node name %s Name=%s\n", node->name, getProp2(node,"Name"));
}

inline bool is_node_name2(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

namespace idx_metadata{
  int parse_level(xmlNode *grid, std::shared_ptr<Level> lvl);
  int parse_topology(xmlNode* topo_node, Topology& topology);
  int parse_geometry(xmlNode *geo_node, Geometry& geometry);

};

#endif
