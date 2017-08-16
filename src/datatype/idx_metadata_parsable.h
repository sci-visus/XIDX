#ifndef IDX_METADATA_PARSABLE_INTERFACE_H_
#define IDX_METADATA_PARSABLE_INTERFACE_H_

namespace idx_metadata{

static const char* getProp2(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

inline bool is_node_name2(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

class Parsable{

public:
  virtual xmlNode* objToXML(xmlNode* parent) = 0;
  virtual int XMLToObj(xmlNode* node) = 0;

};

};

#endif