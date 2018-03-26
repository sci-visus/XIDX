#ifndef XIDX_GROUP_H_
#define XIDX_GROUP_H_

#include "xidx/xidx.h"

namespace xidx{

typedef int DomainIndex;
  
class Group : public Parsable{

private:
  std::shared_ptr<Domain> domain;
  std::vector<std::shared_ptr<Group> > groups;
  std::vector<std::shared_ptr<Variable> > variables;
  
public:

  GroupType group_type;
  VariabilityType variability_type;

  std::vector<std::shared_ptr<DataSource> > data_sources;
  std::vector<Attribute> attributes;
  DomainIndex domain_index;
  
  Group(std::string _name, GroupType _groupType=GroupType::SPATIAL_GROUP_TYPE, VariabilityType _varType=VariabilityType::STATIC_VARIABILITY_TYPE){
    name=_name;
    group_type=_groupType;
    variability_type=_varType;
  }
  
  Group(std::string _name, GroupType _groupType, std::shared_ptr<Domain> _domain, VariabilityType _varType=VariabilityType::STATIC_VARIABILITY_TYPE) {
    name=_name;
    group_type=_groupType;
    variability_type=_varType;
    domain=_domain;
  }
  
  Group(const Group* g){
    SetParent(g->GetParent());
    name=g->name;
    group_type=g->group_type;
    variability_type=g->variability_type;
    domain=g->domain;
    variables = g->variables;
    groups = g->groups;
    data_sources = g->data_sources;
    attributes = g->attributes;
    domain_index = g->domain_index;
  }
  
  inline std::shared_ptr<Domain> GetDomain() { return domain; }
  
  inline int SetDomain(std::shared_ptr<Domain> _domain) { domain = _domain; return 0; }
  
  std::shared_ptr<Variable> AddVariable(const char* name, NumberType numberType, const short bit_precision,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                                        const int n_components=1, const std::vector<INDEX_TYPE> dimensions=std::vector<INDEX_TYPE>()){
    std::shared_ptr<Variable> var(new Variable(this));
    
    var->name = name;
    var->center_type = center;
    
    std::shared_ptr<DataItem> di(new DataItem(this));
    di->number_type = numberType;
    di->bit_precision = string_format("%d", bit_precision);
    di->endian_type = endian;
    if(dimensions.size()>0){
      di->dimensions = std::static_pointer_cast<SpatialDomain>(domain)->topology.dimensions; // Use same dimensions of topology
//      if(n_components > 1)
//        di.dimensions = string_format("%s %d", di.dimensions.c_str(), n_components);
    }
    else
      di->dimensions = dimensions;
    
    di->format_type = FormatType::IDX_FORMAT;
    
    var->AddDataItem(di);
    
    var->AddAttribute(atts);
    
    return AddVariable(var);
  }
  
  int AddDataSource(std::shared_ptr<DataSource> ds) {
    ds->SetParent(this);
    data_sources.push_back(ds);
    return 0;
  }
  
  const std::vector<std::shared_ptr<Group> >& GetGroups(){ return groups; }
  
  const std::vector<std::shared_ptr<Variable> >& GetVariables(){ return variables; }
  
  std::shared_ptr<Variable> AddVariable(const char* name, NumberType numberType, const short bit_precision,
                           const int n_components,
                           const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const std::vector<INDEX_TYPE> dimensions=std::vector<INDEX_TYPE>()){
    return AddVariable(name, numberType, bit_precision, atts, center, endian, n_components, dimensions);
  }
  
  std::shared_ptr<Variable> AddVariable(const char* name, std::shared_ptr<DataItem> item, std::shared_ptr<Domain> domain,
                         const std::vector<Attribute>& atts=std::vector<Attribute>()){
    
    std::shared_ptr<Variable> var(new Variable(this));
    var->name = name;
    SetDomain(domain);
    var->AddDataItem(item);
    
    var->AddAttribute(atts);
    
    return AddVariable(var);
  }
  
  std::shared_ptr<Variable> AddVariable(const char* name, std::string dtype, const CenterType center=CenterType::CELL_CENTER,
                           const EndianType endian=EndianType::LITTLE_ENDIANESS,
                           const std::vector<Attribute>& atts=std::vector<Attribute>(),
                           const std::vector<INDEX_TYPE> dimensions=std::vector<INDEX_TYPE>()){
    std::shared_ptr<Variable> var(new Variable(this));
    
    var->name = name;
    //printf("comp %s ntype %s prec %d\n", dtype.substr(0,comp_idx).c_str(), num_idx, precision);
    
    std::shared_ptr<DataItem> di(new DataItem(dtype, this));
    
    var->center_type = center;
    
    di->endian_type = endian;
    if(dimensions.size()>0){
      di->dimensions = std::static_pointer_cast<SpatialDomain>(domain)->topology.dimensions; // Use same dimensions of topology
      
    }
    else
      di->dimensions = dimensions;
    
    di->format_type = FormatType::IDX_FORMAT;
    
    var->AddDataItem(di);
    
    var->AddAttribute(atts);
    
    return AddVariable(var);
  }
  
  std::shared_ptr<Variable> AddVariable(std::shared_ptr<Variable> attribute){
    variables.push_back(attribute);
    return variables.back();
  }
  
  int AddGroup(std::shared_ptr<Group> group, DomainIndex=0){
    if(group->variability_type == VariabilityType::VARIABLE_VARIABILITY_TYPE){
      group->domain_index = groups.size();
    }
    
    group->SetParent(this);
    groups.push_back(group);
    return 0;
  }
  
  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){

    xmlNodePtr group_node = xmlNewChild(parent, NULL, BAD_CAST "Group", NULL);
    xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(group_node, BAD_CAST "Type", BAD_CAST ToString(group_type));
    xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST ToString(variability_type));
    
    if(variability_type == VariabilityType::VARIABLE_VARIABILITY_TYPE)
      xmlNewProp(group_node, BAD_CAST "DomainIndex", BAD_CAST std::to_string(domain_index).c_str());
    
    for(auto data: data_sources)
      xmlNodePtr data_node = data->Serialize(group_node);
    
    xmlNodePtr domain_node = domain->Serialize(group_node);

    for(auto a:attributes)
      xmlNodePtr a_node = a.Serialize(group_node);
    
    for(auto v:variables)
      xmlNodePtr v_node = v->Serialize(group_node);
      
    for(auto g:groups)
       xmlNodePtr g_node = g->Serialize(group_node);

    return group_node;
  };
  
  int Deserialize(xmlNodePtr node, Parsable* _parent){
    if(!IsNodeName(node,"Group"))
      return -1;
    
    SetParent(_parent);
  
    name = GetProp(node, "Name");
  
    //assert(this->GetParent()!=nullptr);
    
    const char* type_s = GetProp(node, "Type");
    for(int t=GroupType::SPATIAL_GROUP_TYPE; t <= GroupType::TEMPORAL_GROUP_TYPE; t++)
      if (strcmp(type_s, ToString(static_cast<GroupType>(t)))==0)
             group_type = static_cast<GroupType>(t);

    const char* vtype_s = GetProp(node, "VariabilityType");
    for(int t=VariabilityType::STATIC_VARIABILITY_TYPE; t <= VariabilityType::VARIABLE_VARIABILITY_TYPE; t++)
      if (strcmp(vtype_s, ToString(static_cast<VariabilityType>(t)))==0)
        variability_type = static_cast<VariabilityType>(t);
    
    const char* dindex_s = GetProp(node, "DomainIndex");
    
    if(dindex_s != nullptr)
      domain_index = atoi(dindex_s);
    else
      domain_index = 0;

    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      
      if(IsNodeName(cur_node,"DataSource")){
        std::shared_ptr<DataSource> ds(new DataSource());
        ds->Deserialize(cur_node, this);
        data_sources.push_back(ds);
      }
      else if(IsNodeName(cur_node,"Domain")){
        const char* domtype_s = GetProp(cur_node, "Type");
        DomainType dom_type;
        
        for(int t=DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
          if (strcmp(domtype_s, ToString(static_cast<DomainType>(t)))==0)
            dom_type = static_cast<DomainType>(t);

        switch(dom_type){
          case DomainType::HYPER_SLAB_DOMAIN_TYPE:
            domain = std::make_shared<HyperSlabDomain>(new HyperSlabDomain(""));
            break;
          case DomainType::LIST_DOMAIN_TYPE:
            domain = std::make_shared<ListDomain<PHY_TYPE>>(new ListDomain<PHY_TYPE>(""));
            break;
          case DomainType::MULTIAXIS_DOMAIN_TYPE:
            domain = std::make_shared<MultiAxisDomain<PHY_TYPE>>(new MultiAxisDomain<PHY_TYPE>(""));
            break;
          case DomainType::SPATIAL_DOMAIN_TYPE:
            domain = std::make_shared<SpatialDomain>(new SpatialDomain(""));
            break;
          case DomainType::RANGE_DOMAIN_TYPE:
            fprintf(stderr, "Range domain not implemented yet\n");
            break;
        }
        
        if(domain != nullptr)
          domain->Deserialize(cur_node, this);
      }
      else if(IsNodeName(cur_node,"Attribute")){
        Attribute att;
        att.Deserialize(cur_node, this);
        attributes.push_back(att);
      }
      else if(IsNodeName(cur_node,"Variable")){
        std::shared_ptr<Variable> var(new Variable(this));
        var->Deserialize(cur_node, this);
        variables.push_back(var);
        
        //printf("added var %s parent %s\n", variables.back()->name.c_str(), variables.back()->parent->name.c_str());
      }
      else if(IsNodeName(cur_node,"Group")){
        std::shared_ptr<Group> gr(new Group(""));
        gr->Deserialize(cur_node, this);
        groups.push_back(gr);
      }
    }
    
    return 0;
  };
  
  virtual std::string GetClassName() const override { return "Group"; };
  
  virtual Parsable* FindChild(const std::string& class_name) const override {
    if(class_name == "DataSource" && data_sources.size() > 0)
      return data_sources[0].get();
    else
      return nullptr;
  }
  
protected:
  
  virtual std::string GetDataSourceXPath() override {
    if(GetParent() == nullptr)
      xpath_prefix="//Xidx";
    else
      xpath_prefix=GetParent()->GetDataSourceXPath();
      
    xpath_prefix+="/Group";
//    xpath_prefix+="[@Name="+name+"]";
    xpath_prefix+="[@Name=\""+name+"\"]";
    
    return xpath_prefix;
  };

};
  
}

#endif
