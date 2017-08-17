#ifndef IDX_METADATA_GEOMETRY_H_
#define IDX_METADATA_GEOMETRY_H_

#include "idx_metadata_parsable.h"

namespace idx_metadata{

class Geometry : public idx_metadata::Parsable{

public:
  std::string name;
  GeometryType geometryType;
  std::vector<DataItem> items;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){

    xmlNodePtr geometry_node = xmlNewChild(parent, NULL, BAD_CAST "Geometry", NULL);
    xmlNewProp(geometry_node, BAD_CAST "GeometryType", BAD_CAST ToString(geometryType));
    
    for(auto item: items)
      xmlNodePtr item_node = item.objToXML(geometry_node, item.text.c_str());

    return geometry_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"Geometry"))
      return -1;

    //name = idx_metadata::getProp(node, "Name");
            
    const char* geo_type = idx_metadata::getProp(node, "GeometryType");

    for(int t=GeometryType::XYZ_GEOMETRY_TYPE; t <= ORIGIN_DXDY_GEOMETRY_TYPE; t++)
      if (strcmp(geo_type, ToString(static_cast<GeometryType>(t)))==0)
          geometryType = static_cast<GeometryType>(t);

    int data_items_count = 0;
    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(is_node_name(inner_node, "DataItem")){
        DataItem geo_dataitem;
        geo_dataitem.XMLToObj(inner_node);

        items.push_back(geo_dataitem);
      }
    }

    return 0;
  };
};

}

#endif