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

#if _WIN32
#include <windows.h>
#endif

namespace xidx{

typedef int DomainIndex;

class Variability {
  public:
    enum VariabilityType{
      STATIC_VARIABILITY_TYPE = 0,
      VARIABLE_VARIABILITY_TYPE = 1
    };
    
    static inline const char* toString(VariabilityType v)
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
  
  static inline const char* toString(GroupType v)
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
    variability_type=Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE;
    filePattern=_filePattern;
  }
  
  Group(const Group* g){
    setParent(g->getParent());
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
  
  inline std::shared_ptr<Domain> getDomain() { return domain; }
  
  inline int SetDomain(std::shared_ptr<Domain> _domain) { domain = _domain; return 0; }
  
  std::shared_ptr<Variable> addVariable(const char *name, XidxDataType::NumberType numberType,
                                        const short bit_precision,
                                        const std::vector<std::shared_ptr<Attribute>> &atts = std::vector<std::shared_ptr<Attribute>>(),
                                        const Variable::CenterType center = Variable::CenterType::CELL_CENTER,
                                        const Endianess::EndianType endian = Endianess::EndianType::LITTLE_ENDIANESS,
                                        const int n_components = 1,
                                        const std::vector<INDEX_TYPE> dimensions = std::vector<INDEX_TYPE>()){
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
    
    var->addDataItem(di);
    
    var->addAttribute(atts);
    
    return addVariable(var);
  }
  
  int addDataSource(std::shared_ptr<DataSource> ds) {
    ds->setParent(this);
    data_sources.push_back(ds);
    return 0;
  }

  const std::shared_ptr<Group>& getGroup(DomainIndex i){
    if(variability_type == Variability::VariabilityType::STATIC_VARIABILITY_TYPE)
      return groups.back();
    else
      return groups[i];
  }

  const std::vector<std::shared_ptr<Group> >& getGroups(){ return groups; }
  
  std::vector<std::shared_ptr<Variable> > getVariables(){ return variables; }
  
  std::shared_ptr<Variable> addVariable(const char *name, XidxDataType::NumberType numberType,
                                        const short bit_precision,
                                        const int n_components,
                                        const Variable::CenterType center = Variable::CenterType::CELL_CENTER,
                                        const Endianess::EndianType endian = Endianess::EndianType::LITTLE_ENDIANESS,
                                        const std::vector<std::shared_ptr<Attribute>> &atts = std::vector<std::shared_ptr<Attribute>>(),
                                        const std::vector<INDEX_TYPE> dimensions = std::vector<INDEX_TYPE>()){
    return addVariable(name, numberType, bit_precision, atts, center, endian, n_components, dimensions);
  }
  
  std::shared_ptr<Variable> addVariable(const char* name, std::shared_ptr<DataItem> item, std::shared_ptr<Domain> domain,
                         const std::vector<std::shared_ptr<Attribute>>& atts=std::vector<std::shared_ptr<Attribute>>()){
    
    std::shared_ptr<Variable> var(new Variable(this));
    var->name = name;
    SetDomain(domain);
    var->addDataItem(item);
    
    var->addAttribute(atts);
    
    return addVariable(var);
  }
  
  std::shared_ptr<Variable> addVariable(const char *name, std::string dtype,
                                        const Variable::CenterType center = Variable::CenterType::CELL_CENTER,
                                        const Endianess::EndianType endian = Endianess::EndianType::LITTLE_ENDIANESS,
                                        const std::vector <std::shared_ptr<Attribute>> &atts = std::vector <
                                                                                               std::shared_ptr <
                                                                                               Attribute >> (),
                                        const std::vector <INDEX_TYPE> dimensions = std::vector<INDEX_TYPE>()){
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
    
    var->addDataItem(di);
    
    var->addAttribute(atts);
    
    return addVariable(var);
  }
  
  std::shared_ptr<Variable> addVariable(std::shared_ptr<Variable> attribute){
    variables.push_back(attribute);
    return variables.back();
  }
  
  int addGroup(std::shared_ptr<Group> group, DomainIndex=0){
    if(group->variability_type == Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE){
      group->domain_index = groups.size();
    }
    
    group->setParent(this);
    groups.push_back(group);
    return 0;
  }
  
  xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{

    xmlNodePtr group_node = xmlNewChild(parent, NULL, BAD_CAST "Group", NULL);
    xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(group_node, BAD_CAST "Type", BAD_CAST toString(group_type));
    xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST Variability::toString(variability_type));
    
    if(filePattern!="")
      xmlNewProp(group_node, BAD_CAST "FilePattern", BAD_CAST filePattern.c_str());
    
    if(variability_type == Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE)
      xmlNewProp(group_node, BAD_CAST "DomainIndex", BAD_CAST std::to_string(domain_index).c_str());
    
    for(auto data: data_sources)
      xmlNodePtr data_node = data->serialize(group_node);
    
    xmlNodePtr domain_node = domain->serialize(group_node);

    for(auto a:attributes)
      xmlNodePtr a_node = a.serialize(group_node);
    
    for(auto v:variables)
      xmlNodePtr v_node = v->serialize(group_node);
      
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
        
        xmlNodePtr g_node = g->serialize(parent_group);
        
        std::string dirPath = string_format(filePattern, g->domain_index);
      //  std::string filePath = string_format(filePattern+"/meta.xidx", g->domain_index);

#if _WIN32
		    const int ret = CreateDirectory(dirPath.c_str(), NULL);
#else
        const int ret = mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
        if (ret != 0 && errno != EEXIST)
        {
          //perror("mkdir");
          fprintf(stderr, "Error: failed to mkdir %s\n", filePath.c_str());
        }
        else
          saveDoc(filePath, parent_group->doc);
        
        //return group_ref;
      }
      else
        xmlNodePtr g_node = g->serialize(group_node);
    }

    return group_node;
  };
  
  int deserialize(_xmlNode *node, Parsable *_parent) override{
    if(!isNodeName(node,"Group"))
      return -1;
    
    setParent(_parent);
  
    name = xidx::getProp(node, "Name");
  
    //assert(this->getParent()!=nullptr);
    
    const char* type_s = xidx::getProp(node, "Type");
    for(int t=GroupType::SPATIAL_GROUP_TYPE; t <= GroupType::TEMPORAL_GROUP_TYPE; t++)
      if (strcmp(type_s, toString(static_cast<GroupType>(t)))==0)
             group_type = static_cast<GroupType>(t);

    const char* vtype_s = xidx::getProp(node, "VariabilityType");
    for(int t=Variability::VariabilityType::STATIC_VARIABILITY_TYPE; t <= Variability::VariabilityType::VARIABLE_VARIABILITY_TYPE; t++)
      if (strcmp(vtype_s, Variability::toString(static_cast<Variability::VariabilityType>(t)))==0)
        variability_type = static_cast<Variability::VariabilityType>(t);
    
    const char* dindex_s = xidx::getProp(node, "DomainIndex");
    
    const char* fpattern_s = xidx::getProp(node, "FilePattern");
    if(fpattern_s != nullptr)
      filePattern = fpattern_s;
    
    if(dindex_s != nullptr)
      domain_index = atoi(dindex_s);
    else
      domain_index = 0;

    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      
      if(isNodeName(cur_node,"DataSource")){
        std::shared_ptr<DataSource> ds(new DataSource());
        ds->deserialize(cur_node, this);
        data_sources.push_back(ds);
      }
      else if(isNodeName(cur_node,"Domain")){
        const char* domtype_s = xidx::getProp(cur_node, "Type");
        Domain::DomainType dom_type;
        
        for(int t=Domain::DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= Domain::DomainType::RANGE_DOMAIN_TYPE; t++)
          if (strcmp(domtype_s, Domain::toString(static_cast<Domain::DomainType>(t)))==0)
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
          domain->deserialize(cur_node, this);
      }
      else if(isNodeName(cur_node,"Attribute")){
        Attribute att;
        att.deserialize(cur_node, this);
        attributes.push_back(att);
      }
      else if(isNodeName(cur_node,"Variable")){
        std::shared_ptr<Variable> var(new Variable(this));
        var->deserialize(cur_node, this);
        variables.push_back(var);
        
        //printf("added var %s parent %s\n", variables.back()->name.c_str(), variables.back()->parent->name.c_str());
      }
      else if(isNodeName(cur_node,"Group")){
        std::shared_ptr<Group> gr(new Group(""));
        gr->deserialize(cur_node, this);
        groups.push_back(gr);
      }
    }
    
    return 0;
  };
  
  virtual std::string getClassName() const override { return "Group"; };
  
  virtual Parsable* findChild(const std::string &class_name) const override {
    if(class_name == "DataSource" && data_sources.size() > 0)
      return data_sources[0].get();
    else
      return nullptr;
  }
  
protected:
  
  virtual std::string getDataSourceXPath() override {
    if(getParent() == nullptr)
      xpath_prefix="//Xidx";
    else
      xpath_prefix=getParent()->getDataSourceXPath();
      
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
      createNewDoc(doc, root_element);
    }
    
    const Parsable* group = findParent("Group", parent);
    
    if(group != NULL)
    {
      Group* mygroup = (Group*)(group);
      xmlNodePtr group_node = xmlNewChild(root_element, NULL, BAD_CAST "Group", NULL);
      xmlNewProp(group_node, BAD_CAST "Name", BAD_CAST mygroup->name.c_str());
      xmlNewProp(group_node, BAD_CAST "Type", BAD_CAST toString(mygroup->group_type));
      xmlNewProp(group_node, BAD_CAST "VariabilityType", BAD_CAST Variability::toString(mygroup->variability_type));
      
      auto domain = mygroup->getDomain();
      
      domain->serialize(group_node);
      
      return group_node;
      
    }
    else
      return nullptr;
    
  }

};
  
}

#endif
