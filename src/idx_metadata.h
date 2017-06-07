

#ifndef IDX_METADATA_H_
#define IDX_METADATA_H_

#include <cassert>
#include <string>
#include <vector>
#include <memory>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args){
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

struct Information{
  std::string name;
  std::string value;
};

struct Time{
  std::string value;
};

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
};

struct Geometry{
  std::string name;
  GeometryType geometryType;
  std::vector<DataItem> item;
};

struct Topology{
  std::vector<Information> information;
  std::vector<Information> dataItem;
  TopologyType topologyType;
  std::string dimensions;
  std::string order;
  std::string nodesPerElement;
};

struct Attribute{
  std::vector<Information> information;
  DataItem data;
  std::string name;
  CenterType centerType;
  AttributeType attributeType;
};

struct Grid{
  std::vector<Information> information;
  Time time;
  std::vector<Grid> grid;
  Topology topology;
  Geometry geometry;
  std::vector<Attribute> attribute;
  std::string name;
  GridType gridType;
  CollectionType collectionType;
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

enum MetadataLayout{
  SIMPLE = 0,
  HPC = 1
};

namespace idx_metadata{

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

    geo.item.push_back(item_o);
    geo.item.push_back(item_d);

    return 0;
  };

  int set_geometry(Geometry geometry){ grid.geometry = geometry; return 0; }

  int add_attribute(const char* name, NumberType numberType, short precision, 
                   AttributeType attributeType=AttributeType::SCALAR_ATTRIBUTE_TYPE, 
                   CenterType center=CenterType::CELL_CENTER, 
                   EndianType endian=EndianType::LITTLE_ENDIANESS){
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

    return add_attribute(att);
  }

  int add_attribute(Attribute attribute){
    grid.attribute.push_back(attribute);
    return 0;
  };

  int get_n_attributes() { return grid.attribute.size(); }

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
  Information info_time;
  Time time;

public:
  int set_timestep(uint32_t log_time, double phy_time){
    info_time.name = "LogicalTime";
    info_time.value = string_format("%d", log_time);
    time.value = string_format("%f", phy_time);
    return 0;
  }

  int add_level(std::shared_ptr<Level> level){ levels.push_back(level); return 0; }

  std::shared_ptr<Level> get_level(int l){ return levels[l]; }

  int clear(){ levels.clear(); return 0;}

  int get_n_levels() { return levels.size(); }
};

class IDX_Metadata{

private:
  std::vector<std::shared_ptr<TimeStep> > timesteps;
  MetadataLayout layout;
  std::string file_path;

public:

  IDX_Metadata(const char* path, MetadataLayout _layout=MetadataLayout::SIMPLE){
    file_path = path;
    layout = _layout;
  };

  int load();

  int save();

  int add_timestep(std::shared_ptr<TimeStep> ts){ timesteps.push_back(ts); return 0;}

  std::shared_ptr<TimeStep> get_timestep(int t){ return timesteps[t]; }

  int clear(){ timesteps.clear(); return 0; }

  int get_n_timesteps() { return timesteps.size(); }
};

};

#endif
