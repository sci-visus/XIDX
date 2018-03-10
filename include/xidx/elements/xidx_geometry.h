#ifndef XIDX_GEOMETRY_H_
#define XIDX_GEOMETRY_H_

#include "xidx/xidx.h"

namespace xidx{

class Geometry : public Parsable{

public:
  std::string name;
  GeometryType type;
  std::vector<DataItem> items;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr geometry_node = xmlNewChild(parent, NULL, BAD_CAST "Geometry", NULL);
    xmlNewProp(geometry_node, BAD_CAST "GeometryType", BAD_CAST ToString(type));
    
    for(auto item: items)
      xmlNodePtr item_node = item.Serialize(geometry_node);

    return geometry_node;
  };
  
  int Deserialize(xmlNodePtr node){
    if(!IsNodeName(node,"Geometry"))
      return -1;

    //name = xidx::getProp(node, "Name");
            
    const char* geo_type = GetProp(node, "GeometryType");

    for(int t=GeometryType::XYZ_GEOMETRY_TYPE; t <= ORIGIN_DXDY_GEOMETRY_TYPE; t++)
      if (strcmp(geo_type, ToString(static_cast<GeometryType>(t)))==0)
          type = static_cast<GeometryType>(t);

    int data_items_count = 0;
    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(IsNodeName(inner_node, "DataItem")){
        DataItem geo_dataitem;
        geo_dataitem.Deserialize(inner_node);

        items.push_back(geo_dataitem);
      }
    }

    return 0;
  };
};

}

#endif
