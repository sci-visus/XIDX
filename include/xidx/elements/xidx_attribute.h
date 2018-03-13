#ifndef XIDX_ATTRIBUTE_H_
#define XIDX_ATTRIBUTE_H_

#include "xidx_parsable.h"

namespace xidx{

class Attribute: public xidx::Parsable{
public:
  Attribute(){};
  Attribute(std::string _name, std::string _value)
  { name=_name; value=_value; };

  std::string value;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr att_node = xmlNewChild(parent, NULL, BAD_CAST "Attribute", BAD_CAST text);
    xmlNewProp(att_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(att_node, BAD_CAST "Value", BAD_CAST value.c_str());

    return att_node;
  };
  
  int Deserialize(xmlNodePtr node){
    if(!xidx::IsNodeName(node,"Attribute"))
      return -1;

    name = xidx::GetProp(node, "Name");
    value = xidx::GetProp(node, "Value");

    return 0;
  };
  
  virtual std::string GetClassName() override { return "Attribute"; };

};
}

#endif
