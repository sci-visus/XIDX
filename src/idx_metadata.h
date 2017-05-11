

#ifndef IDX_METADATA_H_
#define IDX_METADATA_H_

#include <cassert>
#include <string>
#include <vector>

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


// namespace CollectionType{
//   const char* SPATIAL_COLLECTION_TYPE = "Spatial";
//   const char* TEMPORAL_COLLECTION_TYPE = "Temporal";
// }
// namespace GridType{
//   const char* UNIFORM_GRID_TYPE = "Uniform";
//   const char* COLLECTION_GRID_TYPE = "Collection";
// }

// namespace TopologyType{
//   const char* NO_TOPOLOGY_TYPE = "NoTopologyType";
//   const char* RECT_2D_MESH_TOPOLOGY_TYPE = "2DRectMesh";
//   const char* CORECT_2D_MESH_TOPOLOGY_TYPE = "2DCoRectMesh";
//   const char* RECT_3D_MESH_TOPOLOGY_TYPE = "3DRectMesh";
//   const char* CORECT_3D_MESH_TOPOLOGY_TYPE = "3DCoRectMesh";

//   // Other types to add 
//   // | Polyvertex | Polyline | Polygon | Triangle | Quadrilateral | 
//   // Tetrahedron | Wedge | Hexahedron | Edge_3 | Triangle_6 | Quadrilateral_8 
//   // | Quadrilateral_9 | Tetrahedron_10 | Pyramid_13 | Wedge_15 | Wedge_18 
//   // | Hexahedron_20 | Hexahedron_24 | Hexahedron_27 | Hexahedron_64 | 
//   // Hexahedron_125 | Hexahedron_216 | Hexahedron_343 | Hexahedron_512 | 
//   // Hexahedron_729 | Hexahedron_1000 | Hexahedron_1331 | Hexahedron_Spectral_64 
//   // Hexahedron_Spectral_125 | Hexahedron_Spectral_216 | Hexahedron_Spectral_343 
//   //| Hexahedron_Spectral_512 | Hexahedron_Spectral_729 | Hexahedron_Spectral_1000 
//   // Hexahedron_Spectral_1331 | Mixed | 2DSMesh 
// }

// namespace GeometryType{
//   const char* XYZ_GEOMETRY_TYPE = "XYZ";
//   const char* XY_GEOMETRY_TYPE = "XY";
//   const char* X_Y_Z_GEOMETRY_TYPE = "X_Y_Z";
//   const char* VXVYVZ_GEOMETRY_TYPE = "VXVYVZ";
//   const char* ORIGIN_DXDYDZ_GEOMETRY_TYPE = "ORIGIN_DXDYDZ";
//   const char* ORIGIN_DXDY_GEOMETRY_TYPE = "ORIGIN_DXDY";
// }

// namespace ItemType{
//   const char* UNIFORM_ITEM_TYPE = "Uniform";
//   const char* HYPER_SLAB_ITEM_TYPE = "HyperSlab";
//   const char* FUNCTION_ITEM_TYPE = "Function";
// }

// namespace NumberType{
//   const char* CHAR_NUMBER_TYPE = "Char";
//   const char* UCHAR_NUMBER_TYPE = "UChar";
//   const char* FLOAT_NUMBER_TYPE = "Float";
//   const char* INT_NUMBER_TYPE = "Int"; 
//   const char* UINT_NUMBER_TYPE = "UInt";
// }

// namespace Endian{
//   const char* LITTLE_ENDIANESS = "Little";
//   const char* BIG_ENDIANESS = "Big";
//   const char* NATIVE_ENDIANESS = "Native";
// }

// namespace Format{
//   const char* XML_FORMAT = "XML";
//   const char* HDF_FORMAT = "HDF";
//   const char* BINARY_FORMAT = "Binary";
//   const char* TIFF_FORMAT = "TIFF";
//   const char* IDX_FORMAT = "IDX";
// }

// namespace Center{
//   const char* NODE_CENTER = "Node";
//   const char* CELL_CENTER = "Cell";
//   const char* GRID_CENTER = "Grid";
//   const char* FACE_CENTER = "Face";
//   const char* EDGE_CENTER = "Edge";
// }

// namespace AttributeType{
//   const char* SCALAR_ATTRIBUTE_TYPE = "Scalar";
//   const char* VECTOR_ATTRIBUTE_TYPE = "Vector";
//   const char* TENSOR_ATTRIBUTE_TYPE = "Tensor";

//   // Other types to add
//   // Tensor6 | Matrix
// }

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

class IDX_Metadata{

private:
  std::string file_path;
  std::vector<Information> information;
  std::vector<Domain> domain;

  void initialize_main_grids(){
    domain.resize(1);

    if(domain[0].grid.size() < 1){
      domain[0].grid.resize(2);
      domain[0].grid[0].name = "MainGrid";
      //domain[0].grid[0].gridType = GridType::UNIFORM_GRID_TYPE;

      // Set the Time Grid
      domain[0].grid[1].name = "TimeSeries";
      domain[0].grid[1].gridType = GridType::COLLECTION_GRID_TYPE;
      domain[0].grid[1].collectionType = CollectionType::TEMPORAL_COLLECTION_TYPE;
    }
  }

  Grid* get_global_main_grid(){
    initialize_main_grids();

    return &(domain[0].grid[0]);
  };

  Grid* get_global_time_grid(){
    initialize_main_grids();

    return &(domain[0].grid[1]);
  };

public:

  IDX_Metadata(const char* filename){
    file_path = filename;
    initialize_main_grids();
  };

  int load();

  int save();

  template<typename T>
  int set_global_grid(TopologyType topologyType, const uint32_t* dimensions, 
               GeometryType geometryType, const T* ox_oy_oz, const T* dx_dy_dz){
    int r1 = set_topology(topologyType, dimensions);
    int r2 = set_geometry<T>(geometryType, ox_oy_oz, dx_dy_dz);

    return r1 | r2;
  };

  template<typename T>
  int add_grid(TopologyType topologyType, const uint32_t* dimensions, 
               GeometryType geometryType, const T* ox_oy_oz, const T* dx_dy_dz){
    Grid grid;
    int r1 = set_topology(topologyType, dimensions, &grid);
    int r2 = set_geometry<T>(geometryType, ox_oy_oz, dx_dy_dz, &grid);

    Grid* main_grid = get_global_main_grid();
    main_grid->grid.push_back(grid);

    return r1 | r2;
  };

  int set_topology(TopologyType topologyType, const uint32_t* dimensions, Grid* grid=NULL);

  int set_topology(Topology topology, Grid* grid = NULL){
    if(grid == NULL)
      grid = get_global_main_grid();

    grid->topology = topology;

    return 0;
  }
  
  template<typename T>
  int set_geometry(GeometryType geometryType, const T* ox_oy_oz, 
                          const T* dx_dy_dz, Grid* grid=NULL){
    Geometry geo;
    
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

    if(grid == NULL)
      grid = get_global_main_grid();

    grid->geometry = geo;

    return 0;
  };

  int add_timestep(uint32_t log_time, double phy_time);

  int add_attribute(const char* name, NumberType numberType, short precision, 
                   AttributeType attributeType=AttributeType::SCALAR_ATTRIBUTE_TYPE, 
                   CenterType center=CenterType::CELL_CENTER, 
                   EndianType endian=EndianType::LITTLE_ENDIANESS, Grid* grid = NULL);

  int add_attribute(Attribute attribute, Grid* grid = NULL){
    if(grid == NULL)
      grid = get_global_main_grid();

    grid->attribute.push_back(attribute);

    return 0;
  };

};

#endif
