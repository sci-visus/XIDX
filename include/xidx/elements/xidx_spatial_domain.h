#ifndef XIDX_SPATIAL_DOMAIN_H_
#define XIDX_SPATIAL_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class SpatialDomain : public Domain{

public:
  
  SpatialDomain(std::string _name) : Domain(_name){
    type = DomainType::SPATIAL_DOMAIN_TYPE;
  };
  
  SpatialDomain(const SpatialDomain* dom) : Domain(dom->name){
    topology = dom->topology;
    geometry = dom->geometry;
  }
  
  Topology topology;
  Geometry geometry;
  
  int SetTopology(Topology _topology) { topology = _topology; return 0; }
  
  int SetTopology(TopologyType type, uint32_t dims){
    topology.dimensions = string_format("%d", dims);
    topology.type = type;
    
    return 0;
  }
  
  int SetTopology(TopologyType type, int n_dims, uint32_t* dims){
    for(int i=0; i< n_dims; i++)
      topology.dimensions += std::to_string(dims[i])+" ";
    
    topology.type = type;
    
    return 0;
  }
  
  int SetGeometry(Geometry _geometry) { geometry = _geometry; return 0; }

  int SetGeometry(GeometryType type, int n_dims, const double* ox_oy_oz,
                  const double* dx_dy_dz) {
    geometry.type = type;
    
    DataItem item_o(this);
    item_o.format_type = FormatType::XML_FORMAT;
    item_o.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_o.bit_precision = "32";
    item_o.endian_type = EndianType::LITTLE_ENDIANESS;
    DataItem item_d(this);
    item_d.format_type = FormatType::XML_FORMAT;
    item_d.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_d.bit_precision = "32";
    item_d.endian_type = EndianType::LITTLE_ENDIANESS;
    
    item_o.dimensions = string_format("%d", n_dims);
    item_d.dimensions = string_format("%d", n_dims);
    
    if(type == GeometryType::BOX_P1P2_GEOMETRY_TYPE)
      n_dims *= 2; // two points per dimension
    
    for(int i=0; i< n_dims; i++){
      item_o.text += std::to_string(ox_oy_oz[i])+" ";
      item_d.text += std::to_string(dx_dy_dz[i])+" ";
    }
    
    geometry.items.push_back(item_o);
    geometry.items.push_back(item_d);
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
    xmlNodePtr topology_node = topology.Serialize(domain_node);
    
    xmlNodePtr geometry_node = geometry.Serialize(domain_node);
    
    return domain_node;
  };
  
  virtual int Deserialize(xmlNodePtr node) override{
    Domain::Deserialize(node);

    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
//      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(cur_node, "Topology")){
            topology.Deserialize(cur_node);
          }
          else if(IsNodeName(cur_node, "Geometry")){
            geometry.Deserialize(cur_node);
          }
//        }
      }
    }

    return 0;
  };
  
  virtual std::string GetClassName() override { return "SpatialDomain"; };

};

}
#endif
