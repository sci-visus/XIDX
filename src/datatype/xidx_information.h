#ifndef XIDX_INFOMATION_H_
#define XIDX_INFOMATION_H_

#include "xidx_parsable.h"

namespace xidx{

class Information: public xidx::Parsable{
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
    if(!xidx::is_node_name(node,"Information"))
      return -1;

    name = xidx::getProp(node, "Name");
    value = xidx::getProp(node, "Value");

    return 0;
  };

};
}

#endif