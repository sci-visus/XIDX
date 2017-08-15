
#ifndef IDX_METADATA_DATATYPES_H_
#define IDX_METADATA_DATATYPES_H_

#include <string>
#include <vector>

#include "idx_metadata_config.h"

template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args){
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size);
}

enum CollectionType{
  SPATIAL_COLLECTION_TYPE = 0,
  TEMPORAL_COLLECTION_TYPE = 1
};

inline const char* ToString(CollectionType v)
{
  switch (v)
  {
    case SPATIAL_COLLECTION_TYPE:   return "Spatial";
    case TEMPORAL_COLLECTION_TYPE:   return "Temporal";
    default:      return "[Unknown]";
  }
}

enum GridType{
  UNIFORM_GRID_TYPE = 0,
  COLLECTION_GRID_TYPE = 1
};

inline const char* ToString(GridType v)
{
  switch (v)
  {
    case UNIFORM_GRID_TYPE:   return "Uniform";
    case COLLECTION_GRID_TYPE:   return "Collection";
    default:      return "[Unknown]";
  }
}

enum TopologyType{
  NO_TOPOLOGY_TYPE = 0,
  RECT_2D_MESH_TOPOLOGY_TYPE = 1,
  CORECT_2D_MESH_TOPOLOGY_TYPE = 2,
  RECT_3D_MESH_TOPOLOGY_TYPE = 3,
  CORECT_3D_MESH_TOPOLOGY_TYPE = 4
};

inline const char* ToString(TopologyType v)
{
  switch (v)
  {
    case NO_TOPOLOGY_TYPE:   return "NoTopologyType";
    case RECT_2D_MESH_TOPOLOGY_TYPE:   return "2DRectMesh";
    case RECT_3D_MESH_TOPOLOGY_TYPE:   return "3DRectMesh";
    case CORECT_2D_MESH_TOPOLOGY_TYPE:   return "2DCoRectMesh";
    case CORECT_3D_MESH_TOPOLOGY_TYPE:   return "3DCoRectMesh";
    default:      return "[Unknown]";
  }
}

enum GeometryType{
  XYZ_GEOMETRY_TYPE = 0,
  XY_GEOMETRY_TYPE = 1,
  X_Y_Z_GEOMETRY_TYPE = 2,
  VXVYVZ_GEOMETRY_TYPE = 3,
  ORIGIN_DXDYDZ_GEOMETRY_TYPE = 4,
  ORIGIN_DXDY_GEOMETRY_TYPE = 5
};

inline const char* ToString(GeometryType v)
{
  switch (v)
  {
    case XYZ_GEOMETRY_TYPE:   return "XYZ";
    case XY_GEOMETRY_TYPE:   return "XY";
    case X_Y_Z_GEOMETRY_TYPE:   return "X_Y_Z";
    case VXVYVZ_GEOMETRY_TYPE:   return "VXVYVZ";
    case ORIGIN_DXDYDZ_GEOMETRY_TYPE:   return "ORIGIN_DXDYDZ";
    case ORIGIN_DXDY_GEOMETRY_TYPE:   return "ORIGIN_DXDY";
    default:      return "[Unknown]";
  }
}

enum ItemType{
  UNIFORM_ITEM_TYPE = 0,
  HYPER_SLAB_ITEM_TYPE = 1,
  FUNCTION_ITEM_TYPE = 2
};

inline const char* ToString(ItemType v)
{
  switch (v)
  {
    case UNIFORM_ITEM_TYPE:   return "Uniform";
    case HYPER_SLAB_ITEM_TYPE:   return "HyperSlab";
    case FUNCTION_ITEM_TYPE:   return "Function";
    default:      return "[Unknown]";
  }
}

enum NumberType{
  CHAR_NUMBER_TYPE = 0,
  UCHAR_NUMBER_TYPE = 1,
  FLOAT_NUMBER_TYPE = 2,
  INT_NUMBER_TYPE = 3,
  UINT_NUMBER_TYPE = 4
};

inline const char* ToString(NumberType v)
{
  switch (v)
  {
    case CHAR_NUMBER_TYPE:   return "Char";
    case UCHAR_NUMBER_TYPE:   return "UChar";
    case FLOAT_NUMBER_TYPE:   return "Float";
    case INT_NUMBER_TYPE:   return "Int";
    case UINT_NUMBER_TYPE:   return "UInt";
    default:      return "[Unknown]";
  }
}

enum EndianType{
  LITTLE_ENDIANESS = 0,
  BIG_ENDIANESS = 1,
  NATIVE_ENDIANESS = 2
};

inline const char* ToString(EndianType v)
{
  switch (v)
  {
    case LITTLE_ENDIANESS:   return "Little";
    case BIG_ENDIANESS:   return "Big";
    case NATIVE_ENDIANESS:   return "Native";
    default:      return "[Unknown]";
  }
}

enum FormatType{
  XML_FORMAT = 0,
  HDF_FORMAT = 1,
  BINARY_FORMAT = 2,
  TIFF_FORMAT = 3,
  IDX_FORMAT = 4
};

inline const char* ToString(FormatType v)
{
  switch (v)
  {
    case XML_FORMAT:   return "XML";
    case HDF_FORMAT:   return "HDF";
    case BINARY_FORMAT:   return "Binary";
    case TIFF_FORMAT:   return "TIFF";
    case IDX_FORMAT:   return "IDX";
    default:      return "[Unknown]";
  }
}

enum CenterType{
  NODE_CENTER = 0,
  CELL_CENTER = 1,
  GRID_CENTER = 2,
  FACE_CENTER = 3,
  EDGE_CENTER = 4
};

inline const char* ToString(CenterType v)
{
  switch (v)
  {
    case NODE_CENTER:   return "Node";
    case CELL_CENTER:   return "Cell";
    case GRID_CENTER:   return "Grid";
    case FACE_CENTER:   return "Face";
    case EDGE_CENTER:   return "Edge";
    default:      return "[Unknown]";
  }
}

enum AttributeType{
  SCALAR_ATTRIBUTE_TYPE = 0,
  VECTOR_ATTRIBUTE_TYPE = 1,
  TENSOR_ATTRIBUTE_TYPE = 2

  // Other types to add
  // Tensor6 | Matrix
};

inline const char* ToString(AttributeType v)
{
  switch (v)
  {
    case SCALAR_ATTRIBUTE_TYPE:   return "Scalar";
    case VECTOR_ATTRIBUTE_TYPE:   return "Vector";
    case TENSOR_ATTRIBUTE_TYPE:   return "Tensor";
    default:      return "[Unknown]";
  }
}

class Information{
public:
  Information(){};
  Information(std::string _name, std::string _value)
  { name=_name; value=_value; };

  std::string name;
  std::string value;
};

// Supported in XDMF 2
enum TimeType{
  SINGLE_TIME_TYPE = 0,
  HYPER_SLAB_TIME_TYPE = 1,
  LIST_TIME_TYPE = 2,
  RANGE_TIME_TYPE = 3
};

inline const char* ToString(TimeType v)
{
  switch (v)
  {
    case SINGLE_TIME_TYPE:   return "Single";
    case HYPER_SLAB_TIME_TYPE:   return "HyperSlab";
    case LIST_TIME_TYPE:   return "List";
    case RANGE_TIME_TYPE:   return "Range";
    default:      return "[Unknown]";
  }
}

struct DataItem{
  std::string name;
  ItemType itemType;
  std::string dimensions;
  NumberType numberType;
  std::string precision;
  std::string reference;
  EndianType endianType;
  FormatType formatType;
  std::string text;
  std::vector<Information> information;
};

struct Time{
  std::string value;
  TimeType type;
  std::vector<DataItem> items;
  std::vector<Information> information;
};

struct Geometry{
  std::string name;
  GeometryType geometryType;
  std::vector<DataItem> items;
};

struct Topology{
  std::vector<Information> information;
  std::vector<DataItem> items;
  TopologyType topologyType;
  std::string dimensions;
  std::string order;
  std::string nodesPerElement;
};

struct Attribute{
  std::string name;
  DataItem data;
  CenterType centerType;
  AttributeType attributeType;
  std::vector<Information> information;
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

enum MetadataLayoutType{
  SIMPLE = 0,
  HPC = 1
};

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
    DataItem item_d;
    item_d.formatType = FormatType::XML_FORMAT;

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

  Information get_log_time_info(){ return log_time_info; }

  int get_logical_time(){ return stoi(log_time_info.value); }

  double get_physical_time(){ return time; }

  const char* get_physical_time_str() { return time_str.c_str();}
};

};

#endif