
#ifndef IDX_METADATA_DATATYPES_H_
#define IDX_METADATA_DATATYPES_H_

#include <string>
#include <vector>

#include "idx_metadata_config.h"
#include "idx_metadata_enums.h"
#include "idx_metadata_information.h"
#include "idx_metadata_dataitem.h"
#include "idx_metadata_attribute.h"
#include "idx_metadata_time.h"
#include "idx_metadata_geometry.h"

template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args){
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size);
}

namespace idx_metadata{

struct Topology{
  std::vector<Information> information;
  std::vector<DataItem> items;
  TopologyType topologyType;
  std::string dimensions;
  std::string order;
  std::string nodesPerElement;
};

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

struct Domain{
  std::vector<Information> information;
  std::vector<Grid> grid;
};

struct Graph{
  std::vector<Information> information;
  Time time;
  std::vector<Attribute> attribute;
  std::string name;
  std::string numberColumns;
  std::string numberRows;
};

}
namespace idx_metadata{

static std::string generate_vars_filename(CenterType ctype){
  switch(ctype){
    case NODE_CENTER:   return std::string("NCVars").append(IDX_FILE_EXTENSION);
    case CELL_CENTER:   return std::string("CCVars").append(IDX_FILE_EXTENSION);
    case GRID_CENTER:   return std::string("GCVars").append(IDX_FILE_EXTENSION);
    case FACE_CENTER:   return std::string("FCVars").append(IDX_FILE_EXTENSION);
    case EDGE_CENTER:   return std::string("EGVars").append(IDX_FILE_EXTENSION);
    default:            return std::string("UNKVars").append(IDX_FILE_EXTENSION);
  }
}

class DataGrid{
private:
  Grid grid;

public:
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

  int add_attribute(const char* name, NumberType numberType, const short precision, 
                   const std::vector<Information>& info=std::vector<Information>(),
                   const AttributeType attributeType=AttributeType::SCALAR_ATTRIBUTE_TYPE, 
                   const CenterType center=CenterType::CELL_CENTER, 
                   const EndianType endian=EndianType::LITTLE_ENDIANESS){
    Attribute att;

    att.name = name;
    att.attributeType = attributeType;
    att.centerType = center;
    
    DataItem di;
    di.numberType = numberType;
    di.precision = string_format("%d", precision);
    di.endianType = endian;
    di.dimensions = grid.topology.dimensions; // Use same dimensions of topology
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

class Level{
private:
  std::vector<std::shared_ptr<DataGrid> > grids;

public:
  int add_datagrid(std::shared_ptr<DataGrid> level){ grids.push_back(level); return 0; }

  std::shared_ptr<DataGrid> get_datagrid(int g){ return grids[g]; }

  int clear(){ grids.clear(); return 0; }

  int get_n_datagrids() { return grids.size(); }
};

class TimeStep{
private:
  std::vector<std::shared_ptr<Level> > levels;
  Information log_time_info;
  double time;
  std::string time_str;

public:

  int set_timestep(uint32_t log_time, double phy_time){
    log_time_info.name = "LogicalTime";
    log_time_info.value = string_format("%d", log_time);
    time = phy_time;
    time_str = string_format("%f", time);
    return 0;
  }

  int add_level(std::shared_ptr<Level> level){ levels.push_back(level); return 0; }

  std::shared_ptr<Level> get_level(int l){ return levels[l]; }

  int clear(){ levels.clear(); return 0;}

  int get_n_levels() { return levels.size(); }

  Information& get_log_time_info(){ return log_time_info; }

  int get_logical_time(){ return stoi(log_time_info.value); }

  double get_physical_time(){ return time; }

  const char* get_physical_time_str() { return time_str.c_str();}
};

};

#endif