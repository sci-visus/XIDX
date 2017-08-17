#ifndef IDX_METADATA_ATTRIBUTE_H_
#define IDX_METADATA_ATTRIBUTE_H_

#include "idx_metadata_parsable.h"
#include "idx_metadata_dataitem.h"

namespace idx_metadata{

class Attribute : public idx_metadata::Parsable{

public:
  std::string name;
  DataItem data; // TODO according to the format this should be a vector
  CenterType centerType;
  AttributeType attributeType;
  std::vector<Information> information;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){
    xmlNodePtr attribute_node = xmlNewChild(parent, NULL, BAD_CAST "Attribute", NULL);
    xmlNewProp(attribute_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(attribute_node, BAD_CAST "Center", BAD_CAST ToString(centerType));
    xmlNewProp(attribute_node, BAD_CAST "AttributeType", BAD_CAST ToString(attributeType));

    xmlNodePtr data_node = data.objToXML(attribute_node, data.text.c_str());

    for(auto& curr_info : information){
      xmlNodePtr info_node = curr_info.objToXML(attribute_node);
    }

    return attribute_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"Attribute"))
      return -1;

    name = idx_metadata::getProp(node, "Name");

    const char* center_type = idx_metadata::getProp(node, "Center");
    for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
      if (strcmp(center_type, ToString(static_cast<CenterType>(t)))==0)
          centerType = static_cast<CenterType>(t);

    const char* att_type = idx_metadata::getProp(node, "AttributeType");
    for(int t=AttributeType::SCALAR_ATTRIBUTE_TYPE; t <= TENSOR_ATTRIBUTE_TYPE; t++)
      if (strcmp(att_type, ToString(static_cast<AttributeType>(t)))==0)
          attributeType = static_cast<AttributeType>(t);

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(idx_metadata::is_node_name(inner_node, "DataItem")){
        data.XMLToObj(inner_node);
      }
      else if(idx_metadata::is_node_name(inner_node, "Information")){
        Information info;
        info.XMLToObj(inner_node);
        information.push_back(info);
      }
    }

    return 0;
  };

};
}

#endif