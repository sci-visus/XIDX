#ifndef IDX_METADATA_TIME_H_
#define IDX_METADATA_TIME_H_

#include "idx_metadata_parsable.h"

class Time : public idx_metadata::Parsable{

public:

  std::string value;
  TimeType type;
  std::vector<DataItem> items;
  std::vector<Information> information;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){

    xmlNodePtr time_node = xmlNewChild(parent, NULL, BAD_CAST "Time", NULL);
    xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST value.c_str());

    xmlNewProp(time_node, BAD_CAST "TimeType", BAD_CAST ToString(type));

    for(auto item: items)
      xmlNodePtr item_node = item.objToXML(time_node, item.text.c_str());

    for(auto info: information)
      xmlNodePtr info_node = info.objToXML(time_node);

    return time_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"Time"))
      return -1;

    value = idx_metadata::getProp(node, "Value");
            
    const char* time_type = idx_metadata::getProp(node, "TimeType");
    if(time_type != NULL){
      for(int t=TimeType::SINGLE_TIME_TYPE; t <= TimeType::RANGE_TIME_TYPE; t++)
        if (strcmp(time_type, ToString(static_cast<TimeType>(t)))==0){
          type = static_cast<TimeType>(t);
          break;
        }
    }
    
    if(type == TimeType::HYPER_SLAB_TIME_TYPE){
      for (xmlNode* cur_time_node = node->children; cur_time_node; cur_time_node = cur_time_node->next){ 
        if(cur_time_node->type == XML_ELEMENT_NODE && idx_metadata::is_node_name(cur_time_node,"DataItem")){
          DataItem phy_time_dataitem;
          phy_time_dataitem.XMLToObj(cur_time_node);

          items.push_back(phy_time_dataitem);
        }
      }
    }

    return 0;
  };

};

#endif