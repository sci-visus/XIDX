#ifndef XIDX_SPATIAL_DOMAIN_H_
#define XIDX_SPATIAL_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class SpatialDomain : public Domain{

public:
  
  SpatialDomain(std::string _name) : Domain(_name){};
  
  Topology topology;
  Geometry geometry;

  xmlNodePtr Serialize(xmlNode* parent){
    Domain::Serialize(parent);

    xmlNodePtr topology_node = topology.Serialize(parent);

    xmlNodePtr geometry_node = geometry.Serialize(parent);

    return parent;
  };
  
  int SetTopology(Topology _topology) { topology = _topology; return 0; }
  
  int SetTopology(TopologyType type, uint32_t dims){
    topology.dimensions = string_format("%d", dims);
    topology.type = type;
    
    return 0;
  }
  
  int SetTopology(TopologyType type, int n_dims, uint32_t* dims){
    for(int i=0; i< n_dims; i++)
      topology.dimensions = string_format("%d ", dims[i]);
    
    topology.type = type;
    
    return 0;
  }
  
  int SetGeometry(Geometry _geometry) { geometry = _geometry; return 0; }

  int SetGeometry(GeometryType type, int n_dims, const double* ox_oy_oz,
                  const double* dx_dy_dz) {
    geometry.type = type;
    
    DataItem item_o;
    item_o.format_type = FormatType::XML_FORMAT;
    item_o.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_o.bit_precision = "32";
    item_o.endian_type = EndianType::LITTLE_ENDIANESS;
    DataItem item_d;
    item_d.format_type = FormatType::XML_FORMAT;
    item_d.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_d.bit_precision = "32";
    item_d.endian_type = EndianType::LITTLE_ENDIANESS;
    
    
    item_o.dimensions = string_format("%d", n_dims);
    item_d.dimensions = string_format("%d", n_dims);
    
    for(int i=0; i< n_dims; i++){
      item_o.text +=string_format("%f ",ox_oy_oz[i]);
      item_d.text +=string_format("%f ",dx_dy_dz[i]);
    }
    
    geometry.items.push_back(item_o);
    geometry.items.push_back(item_d);
  }
  
  int Deserialize(xmlNodePtr node){
    Domain::Deserialize(node);

    for (xmlNode* cur_node = node; cur_node; cur_node = cur_node->next) {
      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (inner_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(inner_node, "Topology")){
            topology.Serialize(inner_node);
          }
          else if(IsNodeName(inner_node, "Geometry")){
            geometry.Serialize(inner_node);
          }
        }
      }
    }

    return 0;
  };

};

}
#endif
