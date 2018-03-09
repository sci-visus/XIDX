#ifndef XIDX_PARSABLE_INTERFACE_H_
#define XIDX_PARSABLE_INTERFACE_H_

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx_parse_utils.h"

namespace xidx{

static const char* GetProp(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

inline bool IsNodeName(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

class Parsable{

public:
  std::string name;
  
  virtual xmlNode* Serialize(xmlNode* parent, const char* text=NULL) = 0;
  virtual int Deserialize(xmlNode* node) = 0;

};

};

#endif
