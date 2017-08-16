#ifndef IDX_METADATA_INFOMATION_H_
#define IDX_METADATA_INFOMATION_H_

#include "idx_metadata_parsable.h"

class Information: public idx_metadata::Parsable{
public:
  Information(){};
  Information(std::string _name, std::string _value)
  { name=_name; value=_value; };

  std::string name;
  std::string value;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){
    xmlNodePtr info_node = xmlNewChild(parent, NULL, BAD_CAST "Information", BAD_CAST text);
    xmlNewProp(info_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(info_node, BAD_CAST "Value", BAD_CAST value.c_str());

    return info_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name2(node,"Information"))
      return -1;

    name = idx_metadata::getProp2(node, "Name");
    value = idx_metadata::getProp2(node, "Value");

    return 0;
  };

};


#endif