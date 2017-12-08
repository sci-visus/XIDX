#ifndef XIDX_PARSABLE_INTERFACE_H_
#define XIDX_PARSABLE_INTERFACE_H_

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx_parse_utils.h"

namespace xidx{

static const char* getProp(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

inline bool is_node_name(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

class Parsable{

public:
  virtual xmlNode* objToXML(xmlNode* parent, const char* text=NULL) = 0;
  virtual int XMLToObj(xmlNode* node) = 0;

};

};

#endif