#ifndef XIDX_GROUP_H_
#define XIDX_GROUP_H_

#include "xidx/xidx.h"

namespace xidx{

class Group : public Parsable{

public:

  GroupType groupType;
  VariabilityType variabilityType;
  std::shared_ptr<Domain> domain;
  std::vector<std::shared_ptr<Group> > groups;
  std::vector<std::shared_ptr<DataSource> > data_sources;
  std::vector<Variable> variables;
  std::vector<Attribute> attributes;
  
  Group(std::string _name, GroupType _groupType, VariabilityType _varType=VariabilityType::STATIC_VARIABILITY_TYPE)
    {
    name=_name;
    groupType=_groupType;
    variabilityType=_varType;
  }
  
  Group(std::string _name, GroupType _groupType, std::shared_ptr<Domain> _domain, VariabilityType _varType=VariabilityType::STATIC_VARIABILITY_TYPE) {
    name=_name;
    groupType=_groupType;
    variabilityType=_varType;
    domain=_domain;
  }
  
  inline int SetDomain(std::shared_ptr<Domain> _domain) { domain = _domain; return 0; }
  
  Variable* AddVariable(const char* name, NumberType numberType, const short bit_precision,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                           const int n_components=1, const char* dimensions=NULL){
    Variable var(this);
    
    var.name = name;
    var.center_type = center;
    
    DataItem di(this);
    di.number_type = numberType;
    di.bit_precision = string_format("%d", bit_precision);
    di.endian_type = endian;
    if(dimensions==NULL){
      di.dimensions = std::static_pointer_cast<SpatialDomain>(domain)->topology.dimensions; // Use same dimensions of topology
//      if(n_components > 1)
//        di.dimensions = string_format("%s %d", di.dimensions.c_str(), n_components);
    }
    else
      di.dimensions = dimensions;
    
    di.format_type = FormatType::IDX_FORMAT;
    
    var.data_items.push_back(di);
    
    var.attributes = atts;
    
    return AddVariable(var);
  }
  
  int AddDataSource(std::shared_ptr<DataSource> ds) {
    ds->SetParent(this);
    data_sources.push_back(ds);
    return 0;
  }
  
  Variable* AddVariable(const char* name, NumberType numberType, const short bit_precision,
                           const int n_components,
                           const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const char* dimensions=NULL){
    return AddVariable(name, numberType, bit_precision, atts, center, endian, n_components, dimensions);
  }
  
  Variable* AddVariable(const char* name, DataItem item, std::shared_ptr<Domain> domain,
                         const std::vector<Attribute>& atts=std::vector<Attribute>()){
    
    Variable var(this);
    var.name = name;
    SetDomain(domain);
    var.data_items.push_back(item);
    
    var.attributes = atts;

    return AddVariable(var);
  }
  
  Variable* AddVariable(const char* name, std::string dtype, const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const char* dimensions=NULL){
    Variable var(this);
    
    var.name = name;
    //printf("comp %s ntype %s prec %d\n", dtype.substr(0,comp_idx).c_str(), num_idx, precision);
    
    DataItem di(dtype, this);
    
    var.center_type = center;
    
    di.endian_type = endian;
    if(dimensions==NULL){
      di.dimensions = std::static_pointer_cast<SpatialDomain>(domain)->topology.dimensions; // Use same dimensions of topology
      
    }
    else
      di.dimensions = dimensions;
    
    di.format_type = FormatType::IDX_FORMAT;
    
    var.data_items.push_back(di);
    
    var.attributes = atts;
    
    return AddVariable(var);
  }
  
  Variable* AddVariable(Variable& attribute){
    variables.push_back(attribute);
    return &variables.back();
  }
  
  int AddGroup(std::shared_ptr<Group> group){
    group->SetParent(this);
    groups.push_back(group);
    return 0;
  }
  
  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr group_node = xmlNewChild(parent, NULL, BAD_CAST "Group", NULL);
    xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(group_node, BAD_CAST "Type", BAD_CAST ToString(groupType));
    xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST ToString(variabilityType));
    
    for(auto data: data_sources)
      xmlNodePtr data_node = data->Serialize(group_node);
    
    xmlNodePtr domain_node = domain->Serialize(group_node);

    for(auto a:attributes)
      xmlNodePtr a_node = a.Serialize(group_node);
    
    for(auto v:variables)
      xmlNodePtr v_node = v.Serialize(group_node);
      
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
  
protected:
  
  virtual std::string GetXPath() override {
    if(parent == nullptr)
      xpath_prefix="//Xidx";
    else
      xpath_prefix=parent->GetXPath();
      
    xpath_prefix+="/Group";
    xpath_prefix+="[@Name="+name+"]";
//    xpath_prefix+="[@Name=\""+name+"\"]";
    
    return xpath_prefix;
  };

};
  
}

#endif
