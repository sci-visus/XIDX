/*
 * Copyright (c) 2017 University of Utah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XIDX_GROUP_H_
#define XIDX_GROUP_H_

#include "xidx/xidx.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

namespace xidx{

typedef int DomainIndex;

class Variability {
  public:
    enum VariabilityType{
      STATIC_VARIABILITY_TYPE = 0,
      VARIABLE_VARIABILITY_TYPE = 1
    };
    
    static inline const char* ToString(VariabilityType v)
    {
      switch (v)
      {
        case STATIC_VARIABILITY_TYPE:     return "Static";
        case VARIABLE_VARIABILITY_TYPE:   return "Variable";
        default:                          return "[Unknown]";
      }
    }
};
  
class Group : public Parsable{

public:
  enum GroupType{
    SPATIAL_GROUP_TYPE = 0,
    TEMPORAL_GROUP_TYPE = 1
  };
  
  static inline const char* ToString(GroupType v)
  {
    switch (v)
    {
      case SPATIAL_GROUP_TYPE:    return "Spatial";
      case TEMPORAL_GROUP_TYPE:   return "Temporal";
      default:                    return "[Unknown]";
    }
  }
  
private:
  std::shared_ptr<Domain> domain;
  std::vector<std::shared_ptr<Group> > groups;
  std::vector<std::shared_ptr<Variable> > variables;
  std::string filePattern;
  
public:

  GroupType group_type;
  Variability::VariabilityType variability_type;

  std::vector<std::shared_ptr<DataSource> > data_sources;
  std::vector<Attribute> attributes;
  DomainIndex domain_index;
  
  Group(std::string _name, GroupType _groupType=GroupType::SPATIAL_GROUP_TYPE, Variability::VariabilityType _varType=Variability::VariabilityType::STATIC_VARIABILITY_TYPE){
    name=_name;
    group_type=_groupType;
    variability_type=_varType;
  }
  
  Group(std::string _name, GroupType _groupType, std::shared_ptr<Domain> _domain, Variability::VariabilityType _varType=Variability::VariabilityType::STATIC_VARIABILITY_TYPE) {
    name=_name;
    group_type=_groupType;
    variability_type=_varType;
    domain=_domain;
  }
  
  Group(std::string _name, GroupType _groupType, std::string _filePattern) {
    name=_name;
    group_type=_groupType;
    variability_type=Variability::VariabilityType::STATIC_VARIABILITY_TYPE;
    filePattern=_filePattern;
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
    filePattern = g->filePattern;
  }
  
  inline std::shared_ptr<Domain> GetDomain() { return domain; }
  
  inline int SetDomain(std::shared_ptr<Domain> _domain) { domain = _domain; return 0; }
  
  std::shared_ptr<Variable> AddVariable(const char* name, XidxDataType::NumberType numberType, const short bit_precision,
                           const std::vector<std::shared_ptr<Attribute>>& atts=std::vector<std::shared_ptr<Attribute>>(),
                           const Variable::CenterType center=Variable::CenterType::CELL_CENTER,
                           const Endianess::EndianType endian=Endianess::EndianType::LITTLE_ENDIANESS,
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
    
    di->format_type = DataItem::FormatType::IDX_FORMAT;
    
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
  
  std::vector<std::shared_ptr<Variable> > GetVariables(){ return variables; }
  
  std::shared_ptr<Variable> AddVariable(const char* name, XidxDataType::NumberType numberType, const short bit_precision,
                           const int n_components,
                           const Variable::CenterType center=Variable::CenterType::CELL_CENTER,
                           const Endianess::EndianType endian=Endianess::EndianType::LITTLE_ENDIANESS,
                           const std::vector<std::shared_ptr<Attribute>>& atts=std::vector<std::shared_ptr<Attribute>>(),
                           const std::vector<INDEX_TYPE> dimensions=std::vector<INDEX_TYPE>()){
    return AddVariable(name, numberType, bit_precision, atts, center, endian, n_components, dimensions);
  }
  
  std::shared_ptr<Variable> AddVariable(const char* name, std::shared_ptr<DataItem> item, std::shared_ptr<Domain> domain,
                         const std::vector<std::shared_ptr<Attribute>>& atts=std::vector<std::shared_ptr<Attribute>>()){
    
    std::shared_ptr<Variable> var(new Variable(this));
    var->name = name;
    SetDomain(domain);
    var->AddDataItem(item);
    
    var->AddAttribute(atts);
    
    return AddVariable(var);
  }
  
  std::shared_ptr<Variable> AddVariable(const char* name, std::string dtype,
                                        const  Variable::CenterType center=Variable::CenterType::CELL_CENTER,
                           const Endianess::EndianType endian=Endianess::EndianType::LITTLE_ENDIANESS,
                           const std::vector<std::shared_ptr<Attribute>>& atts=std::vector<std::shared_ptr<Attribute>>(),
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
    
    di->format_type = DataItem::FormatType::IDX_FORMAT;
    
    var->AddDataItem(di);
    
    var->AddAttribute(atts);
    
    return AddVariable(var);
  }
  
  std::shared_ptr<Variable> AddVariable(std::shared_ptr<Variable> attribute){
    variables.push_back(attribute);
    return variables.back();
  }
  
  int AddGroup(std::shared_ptr<Group> group, DomainIndex=0){
    if(group->variability_type == Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE){
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
    xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST Variability::ToString(variability_type));
    
    if(filePattern!="")
      xmlNewProp(group_node, BAD_CAST "FilePattern", BAD_CAST filePattern.c_str());
    
    if(variability_type == Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE)
      xmlNewProp(group_node, BAD_CAST "DomainIndex", BAD_CAST std::to_string(domain_index).c_str());
    
    for(auto data: data_sources)
      xmlNodePtr data_node = data->Serialize(group_node);
    
    xmlNodePtr domain_node = domain->Serialize(group_node);

    for(auto a:attributes)
      xmlNodePtr a_node = a.Serialize(group_node);
    
    for(auto v:variables)
      xmlNodePtr v_node = v->Serialize(group_node);
      
    for(auto g:groups){
      xmlNodePtr group_ref = NULL;
      
      xmlNodePtr parent_group = NULL;
      
      if(filePattern!="")
      {
        std::string filePath = string_format(filePattern+"/meta.xidx", g->domain_index);
        
        group_ref = xmlNewChild(group_node, NULL, BAD_CAST "xi:include", NULL);
        xmlNewProp(group_ref, BAD_CAST "href", BAD_CAST filePath.c_str());
        xmlNewProp(group_ref, BAD_CAST "xpointer", BAD_CAST "xpointer(//Xidx/Group/Group)");
        
        parent_group = ResolveExternalNode(filePattern, this);
        
        xmlNodePtr g_node = g->Serialize(parent_group);
        
        std::string dirPath = string_format(filePattern, domain_index);
      //  std::string filePath = string_format(filePattern+"/meta.xidx", g->domain_index);
        
        const int ret = mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
        if (ret != 0 && errno != EEXIST)
        {
          //perror("mkdir");
          fprintf(stderr, "Error: failed to mkdir %s\n", filePath.c_str());
        }
        else
          SaveDoc(filePath, parent_group->doc);
        
        //return group_ref;
      }


    }

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
    for(int t=Variability::VariabilityType::STATIC_VARIABILITY_TYPE; t <= Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE; t++)
      if (strcmp(vtype_s, Variability::ToString(static_cast<Variability::VariabilityType>(t)))==0)
        variability_type = static_cast<Variability::VariabilityType>(t);
    
    const char* dindex_s = GetProp(node, "DomainIndex");
    
    const char* fpattern_s = GetProp(node, "FilePattern");
    if(fpattern_s != nullptr)
      filePattern = fpattern_s;
    
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
        Domain::DomainType dom_type;
        
        for(int t=Domain::DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= Domain::DomainType::RANGE_DOMAIN_TYPE; t++)
          if (strcmp(domtype_s, Domain::ToString(static_cast<Domain::DomainType>(t)))==0)
            dom_type = static_cast<Domain::DomainType>(t);

        switch(dom_type){
          case Domain::DomainType::HYPER_SLAB_DOMAIN_TYPE:
            domain = std::make_shared<HyperSlabDomain>(new HyperSlabDomain(""));
            break;
          case Domain::DomainType::LIST_DOMAIN_TYPE:
            domain = std::make_shared<ListDomain<PHY_TYPE>>(new ListDomain<PHY_TYPE>(""));
            break;
          case Domain::DomainType::MULTIAXIS_DOMAIN_TYPE:
            domain = std::make_shared<MultiAxisDomain>(new MultiAxisDomain(""));
            break;
          case Domain::DomainType::SPATIAL_DOMAIN_TYPE:
            domain = std::make_shared<SpatialDomain>(new SpatialDomain(""));
            break;
          case Domain::DomainType::RANGE_DOMAIN_TYPE:
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
  
  xmlNodePtr ResolveExternalNode(std::string filePath, const Parsable* parent)
  {
    xmlDocPtr doc = NULL;//= xmlReadFile(filePath.c_str(), NULL, 0);
    xmlNodePtr root_element = nullptr;
    
    if (doc != NULL) {
      root_element = xmlDocGetRootElement(doc);
    }
    else{
      doc=NULL;
      root_element=NULL;
      CreateNewDoc(doc, root_element);
    }
    
    const Parsable* group = FindParent("Group", parent);
    
    if(group != NULL)
    {
      Group* mygroup = (Group*)(group);
      xmlNodePtr group_node = xmlNewChild(root_element, NULL, BAD_CAST "Group", NULL);
      xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST mygroup->name.c_str());
      xmlNewProp(group_node, BAD_CAST "Type", BAD_CAST ToString(mygroup->group_type));
      xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST Variability::ToString(mygroup->variability_type));
      
      auto domain = mygroup->GetDomain();
      
      domain->Serialize(group_node);
      
      return group_node;
      
    }
    else
      return nullptr;
    
  }

};
  
}

#endif
