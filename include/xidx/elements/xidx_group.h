#ifndef XIDX_GROUP_H_
#define XIDX_GROUP_H_

#include "xidx_types.h"
#include "xidx_domain.h"
#include "xidx_attribute.h"
#include "xidx_parsable.h"

namespace xidx{

class Group : public xidx::Parsable{

public:

  GroupType groupType;
  VariabilityType variabilityType;
  std::shared_ptr<Domain> domain;
  std::vector<std::shared_ptr<Group> > groups;
  std::vector<Attribute> attributes;
  
  Group(std::string _name, GroupType _groupType, VariabilityType _varType=VariabilityType::STATIC_VARIABILITY_TYPE) {
    name=_name;
    groupType=_groupType;
    variabilityType=_varType;
  }

  inline int SetDomain(std::shared_ptr<Domain> _domain) { domain = _domain; return 0; }
  
  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr group_node = xmlNewChild(parent, NULL, BAD_CAST "Group", NULL);
    xmlNewProp(group_node, BAD_CAST "GroupType", BAD_CAST ToString(groupType));
    xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST ToString(variabilityType));
    xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST name.c_str());

    xmlNodePtr domain_node = domain->Serialize(group_node);

    for(auto g:groups)
       xmlNodePtr g_node = g->Serialize(group_node);

    // if(groupType == GroupType::SPATIAL_GRID_TYPE){
    //   xmlNodePtr topology_node = topology.objToXML(group_node);
    //   xmlNodePtr geometry_node = geometry.objToXML(group_node);
    // }else if(groupType == GroupType::TEMPORAL_GRID_TYPE){

    //   if(groupType == CollectionType::TEMPORAL_COLLECTION_TYPE){
    //     xmlNodePtr curr_time_node = xmlNewChild(time_group_node, NULL, BAD_CAST "Group", NULL);
    //     xmlNewProp(curr_time_node, BAD_CAST "Name", BAD_CAST string_format(xidx_TIME_FORMAT,i).c_str());
    //     xmlNewProp(curr_time_node, BAD_CAST "GroupType", BAD_CAST ToString(GroupType::COLLECTION_GRID_TYPE));
    //     xmlNewProp(curr_time_node, BAD_CAST "CollectionType", BAD_CAST ToString(CollectionType::SPATIAL_COLLECTION_TYPE));

    //     xmlNodePtr info_node = curr_group->get_log_time_info().objToXML(curr_time_node);

    //     xmlNodePtr time_node = xmlNewChild(curr_time_node, NULL, BAD_CAST "Time", NULL);
    //     xmlNewProp(time_node, BAD_CAST "Value", BAD_CAST curr_group->get_physical_time_str());
    //   }

    // }

    return group_node;
  };
  
  int Deserialize(xmlNodePtr node){
    // if(!xidx::is_node_name(node,"Topology"))
    //   return -1;

    // const char* topo_type = xidx::getProp(node, "TopologyType");

    // for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
    //   if (strcmp(topo_type, ToString(static_cast<TopologyType>(t)))==0)
    //       topologyType = static_cast<TopologyType>(t);

    // dimensions = xidx::getProp(node, "Dimensions");

    return 0;
  };

};

}

#endif
