
#ifndef IDX_METADATA_ENUMS_H_
#define IDX_METADATA_ENUMS_H_


enum MetadataLayoutType{
  SIMPLE = 0,
  HPC = 1
};

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

#endif