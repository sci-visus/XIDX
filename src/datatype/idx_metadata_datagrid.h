
#ifndef IDX_METADATA_DATAGRID_H_
#define IDX_METADATA_DATAGRID_H_

#include <cctype>

#include "idx_metadata_enums.h"
#include "idx_metadata_topology.h"
#include "idx_metadata_geometry.h"
#include "idx_metadata_attribute.h"
#include "idx_metadata_time.h"

namespace idx_metadata{

struct Grid{
  std::string name;
  GridType gridType;
  CollectionType collectionType;
  Topology topology;
  Geometry geometry;
  std::vector<Attribute> attribute;
  Time time;
  std::vector<Grid> grid;
  std::vector<Information> information;
};

class DataGrid{
private:
  Grid grid;
  int ref_attributes;

public:
  DataGrid(){
    ref_attributes = 1; // default: refer attributes to the first grid
  }

  template<typename T>
  int set_grid(const char* name, TopologyType topologyType, const uint32_t* dimensions, 
               GeometryType geometryType, const T* ox_oy_oz, const T* dx_dy_dz, 
               const char* dataset=NULL){
    grid.name = name;
    int r1 = set_topology(topologyType, dimensions);
    int r2 = set_geometry<T>(geometryType, ox_oy_oz, dx_dy_dz);

    if(dataset != NULL){
      Information data_info;
      data_info.name = "Dataset";
      data_info.value = dataset;
      grid.information.push_back(data_info);
    }

    return r1 | r2;
  }

  int set_grid(Grid _grid){ grid = _grid; return 0; }

  int set_topology(TopologyType topologyType, const uint32_t* dimensions)
  {
    Topology& topology = grid.topology;

    topology.topologyType = topologyType;

    if(topologyType == TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE)
      topology.dimensions = string_format("%d %d %d", dimensions[0],dimensions[1],dimensions[2]);
    else if(topologyType == TopologyType::CORECT_2D_MESH_TOPOLOGY_TYPE)
      topology.dimensions = string_format("%d %d", dimensions[0],dimensions[1]);
    else{
      fprintf(stderr, "TopologyType not supported\n");
      assert(false);
    }

    return 0;
  };

  int set_topology(Topology topology) { grid.topology = topology; return 0; }
  
  template<typename T>
  int set_geometry(GeometryType geometryType, const T* ox_oy_oz, 
                          const T* dx_dy_dz){
    Geometry& geo = grid.geometry;
    
    geo.geometryType = geometryType;
    DataItem item_o;
    item_o.formatType = FormatType::XML_FORMAT;
    item_o.numberType = NumberType::FLOAT_NUMBER_TYPE;
    item_o.precision = "4";
    item_o.endianType = EndianType::LITTLE_ENDIANESS;
    DataItem item_d;
    item_d.formatType = FormatType::XML_FORMAT;
    item_d.numberType = NumberType::FLOAT_NUMBER_TYPE;
    item_d.precision = "4";
    item_d.endianType = EndianType::LITTLE_ENDIANESS;

    if(geometryType == GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE){
      item_o.dimensions = "3";
      item_o.text = string_format("%f %f %f", ox_oy_oz[0], ox_oy_oz[1], ox_oy_oz[2]);

      item_d.dimensions = "3";
      item_d.text = string_format("%f %f %f", dx_dy_dz[0], dx_dy_dz[1], dx_dy_dz[2]);
   
    }
    else if(geometryType == GeometryType::ORIGIN_DXDY_GEOMETRY_TYPE){
      item_o.dimensions = "2";
      item_o.text = string_format("%f %f", ox_oy_oz[0], ox_oy_oz[1]);

      item_d.dimensions = "2";
      item_d.text = string_format("%f %f", dx_dy_dz[0], dx_dy_dz[1]);
    }
    else{
      fprintf(stderr, "GeometryType not supported\n");
      assert(false);
    }

    geo.items.push_back(item_o);
    geo.items.push_back(item_d);

    return 0;
  };

  std::vector<Attribute>& get_attributes(){ return grid.attribute; }

  int set_geometry(Geometry geometry){ grid.geometry = geometry; return 0; }

  int set_reference_attributes(int grid_id){ ref_attributes = grid_id; return 0;}

  int get_reference_attributes(){ return ref_attributes; }

  int unset_reference_attributes(){ ref_attributes = 0; return 0;}

  int add_attribute(const char* name, NumberType numberType, const short precision, 
                   const std::vector<Information>& info=std::vector<Information>(),
                   const AttributeType attributeType=AttributeType::SCALAR_ATTRIBUTE_TYPE, 
                   const CenterType center=CenterType::CELL_CENTER, 
                   const EndianType endian=EndianType::LITTLE_ENDIANESS,
                   const int n_components=1, const char* dimensions=NULL){
    Attribute att;

    att.name = name;
    att.attributeType = attributeType;
    att.centerType = center;
    
    DataItem di;
    di.numberType = numberType;
    di.precision = string_format("%d", precision);
    di.endianType = endian;
    if(dimensions==NULL){
      di.dimensions = grid.topology.dimensions; // Use same dimensions of topology
      di.dimensions = string_format("%s %d", di.dimensions.c_str(), n_components);
    }
    else
      di.dimensions = dimensions;
    
    di.formatType = FormatType::IDX_FORMAT;
    di.text = generate_vars_filename(att.centerType);

    att.data = di;

    att.information = info;

    return add_attribute(att);
  }

  int add_attribute(const char* name, NumberType numberType, const short precision, 
                   const AttributeType attributeType, 
                   const int n_components=1,
                   const CenterType center=CenterType::CELL_CENTER, 
                   const EndianType endian=EndianType::LITTLE_ENDIANESS,
                   const std::vector<Information>& info=std::vector<Information>(),
                   const char* dimensions=NULL){
    return add_attribute(name, numberType, precision, info, attributeType, center, endian, n_components, dimensions);

  }

  int add_attribute(const char* name, std::string dtype, const CenterType center=CenterType::CELL_CENTER,
                    const EndianType endian=EndianType::LITTLE_ENDIANESS,
                    const std::vector<Information>& info=std::vector<Information>(),
                    const char* dimensions=NULL){
    Attribute att;

    att.name = name;
  
    size_t comp_idx= dtype.find_last_of("*\\");
    int n_components = stoi(dtype.substr(0,comp_idx));

    if(n_components == 1)
      att.attributeType = AttributeType::SCALAR_ATTRIBUTE_TYPE;
    else
      att.attributeType = AttributeType::VECTOR_ATTRIBUTE_TYPE;

    size_t num_idx=0;
    for(int i=comp_idx;i<dtype.size(); i++)
      if(!std::isdigit(dtype[i]))
        num_idx++;
      else
        break;

    std::string ntype = dtype.substr(comp_idx+1, num_idx-1);
    int precision = stoi(dtype.substr(num_idx+1))/8;
    //printf("comp %s ntype %s prec %d\n", dtype.substr(0,comp_idx).c_str(), num_idx, precision);

    DataItem di;

    for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++){
      std::string numType = ToString(static_cast<NumberType>(t));
      std::transform(numType.begin(), numType.end(), numType.begin(), ::tolower);
      
      if (strcmp(numType.c_str(), ntype.c_str())==0){
        di.numberType = static_cast<NumberType>(t);
        break;
      }
    }

    att.centerType = center;
    
    di.precision = string_format("%d", precision);
    di.endianType = endian;
    if(dimensions==NULL){
      di.dimensions = grid.topology.dimensions; // Use same dimensions of topology
      di.dimensions = string_format("%s %d", di.dimensions.c_str(), n_components);
    }
    else
      di.dimensions = dimensions;
    
    di.formatType = FormatType::IDX_FORMAT;
    di.text = generate_vars_filename(att.centerType);

    att.data = di;

    att.information = info;

    return add_attribute(att);
  }

  int add_attribute(Attribute attribute){
    grid.attribute.push_back(attribute);
    return 0;
  };

  Grid& get_grid(){ return grid; }
};

}

#endif