#ifndef XIDX_GEOMETRY_H_
#define XIDX_GEOMETRY_H_

#include "xidx/xidx.h"

namespace xidx{

class Geometry : public Parsable{

public:
  std::string name;
  GeometryType type;
  std::vector<DataItem> items;
  
  Geometry(){}
  
  Geometry(GeometryType _type){
    type = _type;
  }
  
  Geometry(GeometryType _type, DataItem item){
    type = _type;
    items.push_back(item);
  }

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr geometry_node = xmlNewChild(parent, NULL, BAD_CAST "Geometry", NULL);
    xmlNewProp(geometry_node, BAD_CAST "Type", BAD_CAST ToString(type));
    
    for(auto item: items)
      xmlNodePtr item_node = item.Serialize(geometry_node);

    return geometry_node;
  };
  
  int Deserialize(xmlNodePtr node, Parsable* _parent){
    if(!IsNodeName(node,"Geometry"))
      return -1;

    SetParent(_parent);
    //name = xidx::getProp(node, "Name");
            
    const char* geo_type = GetProp(node, "Type");

    for(int t=GeometryType::XYZ_GEOMETRY_TYPE; t <= RECT_GEOMETRY_TYPE; t++)
      if (strcmp(geo_type, ToString(static_cast<GeometryType>(t)))==0)
          type = static_cast<GeometryType>(t);

    int data_items_count = 0;
    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(IsNodeName(inner_node, "DataItem")){
        DataItem geo_dataitem(this);
        geo_dataitem.Deserialize(inner_node, this);

        items.push_back(geo_dataitem);
      }
    }

    return 0;
  };
  
  virtual std::string GetClassName() const override { return "Geometry"; };
};

}

#endif
