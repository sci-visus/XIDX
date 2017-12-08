#ifndef XIDX_TIME_H_
#define XIDX_TIME_H_

#include "xidx_parsable.h"

namespace xidx{

class Time : public xidx::Parsable{

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
    if(!xidx::is_node_name(node,"Time"))
      return -1;

    value = xidx::getProp(node, "Value");
            
    const char* time_type = xidx::getProp(node, "TimeType");
    if(time_type != NULL){
      for(int t=TimeType::SINGLE_TIME_TYPE; t <= TimeType::RANGE_TIME_TYPE; t++)
        if (strcmp(time_type, ToString(static_cast<TimeType>(t)))==0){
          type = static_cast<TimeType>(t);
          break;
        }
    }
    
    if(type == TimeType::HYPER_SLAB_TIME_TYPE){
      for (xmlNode* cur_time_node = node->children; cur_time_node; cur_time_node = cur_time_node->next){ 
        if(cur_time_node->type == XML_ELEMENT_NODE && xidx::is_node_name(cur_time_node,"DataItem")){
          DataItem phy_time_dataitem;
          phy_time_dataitem.XMLToObj(cur_time_node);

          items.push_back(phy_time_dataitem);
        }
      }
    }

    for (xmlNode* cur_time_node = node->children; cur_time_node; cur_time_node = cur_time_node->next){ 
      if(cur_time_node->type == XML_ELEMENT_NODE && xidx::is_node_name(cur_time_node,"Information")){
        Information info;
        info.XMLToObj(cur_time_node);
        information.push_back(info);
      }
    }

    return 0;
  };

};

}

#endif