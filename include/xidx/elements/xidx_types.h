
#ifndef XIDX_TYPES_H_
#define XIDX_TYPES_H_

#include <string>
#include "xidx/xidx_config.h"

enum GroupType{
  SPATIAL_GROUP_TYPE = 0,
  TEMPORAL_GROUP_TYPE = 1
};

inline const char* ToString(GroupType v)
{
  switch (v)
  {
    case SPATIAL_GROUP_TYPE:    return "Spatial";
    case TEMPORAL_GROUP_TYPE:   return "Temporal";
    default:                    return "[Unknown]";
  }
}

enum VariabilityType{
  STATIC_VARIABILITY_TYPE = 0,
  VARIABLE_VARIABILITY_TYPE = 1
};

inline const char* ToString(VariabilityType v)
{
  switch (v)
  {
    case STATIC_VARIABILITY_TYPE:     return "Static";
    case VARIABLE_VARIABILITY_TYPE:   return "Variable";
    default:                          return "[Unknown]";
  }
}

enum TopologyType{
  NO_TOPOLOGY_TYPE = 0,
  RECT_2D_MESH_TOPOLOGY_TYPE = 1,
  CORECT_2D_MESH_TOPOLOGY_TYPE = 2,
  RECT_3D_MESH_TOPOLOGY_TYPE = 3,
  CORECT_3D_MESH_TOPOLOGY_TYPE = 4,
  DIM_1D_TOPOLOGY_TYPE = 5
};

inline const char* ToString(TopologyType v)
{
  switch (v)
  {
    case NO_TOPOLOGY_TYPE:              return "NoTopologyType";
    case RECT_2D_MESH_TOPOLOGY_TYPE:    return "2DRectMesh";
    case RECT_3D_MESH_TOPOLOGY_TYPE:    return "3DRectMesh";
    case CORECT_2D_MESH_TOPOLOGY_TYPE:  return "2DCoRectMesh";
    case CORECT_3D_MESH_TOPOLOGY_TYPE:  return "3DCoRectMesh";
    case DIM_1D_TOPOLOGY_TYPE:          return "1D";
    default:                            return "[Unknown]";
  }
}

enum GeometryType{
  XYZ_GEOMETRY_TYPE = 0,
  XY_GEOMETRY_TYPE = 1,
  X_Y_Z_GEOMETRY_TYPE = 2,
  VXVYVZ_GEOMETRY_TYPE = 3,
  ORIGIN_DXDYDZ_GEOMETRY_TYPE = 4,
  ORIGIN_DXDY_GEOMETRY_TYPE = 5,
  BOX_P1P2_GEOMETRY_TYPE = 6
};

inline const char* ToString(GeometryType v)
{
  switch (v)
  {
    case XYZ_GEOMETRY_TYPE:               return "XYZ";
    case XY_GEOMETRY_TYPE:                return "XY";
    case X_Y_Z_GEOMETRY_TYPE:             return "X_Y_Z";
    case VXVYVZ_GEOMETRY_TYPE:            return "VXVYVZ";
    case ORIGIN_DXDYDZ_GEOMETRY_TYPE:     return "ORIGIN_DXDYDZ";
    case ORIGIN_DXDY_GEOMETRY_TYPE:       return "ORIGIN_DXDY";
    case BOX_P1P2_GEOMETRY_TYPE:          return "BOX_P1P2";
    default:                              return "[Unknown]";
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
    case CHAR_NUMBER_TYPE:    return "Char";
    case UCHAR_NUMBER_TYPE:   return "UChar";
    case FLOAT_NUMBER_TYPE:   return "Float";
    case INT_NUMBER_TYPE:     return "Int";
    case UINT_NUMBER_TYPE:    return "UInt";
    default:                  return "[Unknown]";
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
    case LITTLE_ENDIANESS:    return "Little";
    case BIG_ENDIANESS:       return "Big";
    case NATIVE_ENDIANESS:    return "Native";
    default:                  return "[Unknown]";
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
    case XML_FORMAT:      return "XML";
    case HDF_FORMAT:      return "HDF";
    case BINARY_FORMAT:   return "Binary";
    case TIFF_FORMAT:     return "TIFF";
    case IDX_FORMAT:      return "IDX";
    default:              return "[Unknown]";
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
    default:            return "[Unknown]";
  }
}

// TODO externalize this function (for Uintah)
//static std::string GenerateVarsFilename(CenterType ctype){
//  switch(ctype){
//    case NODE_CENTER:   return std::string("NCVars").append(IDX_FILE_EXTENSION);
//    case CELL_CENTER:   return std::string("CCVars").append(IDX_FILE_EXTENSION);
//    case GRID_CENTER:   return std::string("GCVars").append(IDX_FILE_EXTENSION);
//    case FACE_CENTER:   return std::string("FCVars").append(IDX_FILE_EXTENSION);
//    case EDGE_CENTER:   return std::string("EGVars").append(IDX_FILE_EXTENSION);
//    default:            return std::string("UNKVars").append(IDX_FILE_EXTENSION);
//  }
//}

enum DomainType{
  HYPER_SLAB_DOMAIN_TYPE = 0,
  PHYLOG_HYPER_SLAB_DOMAIN_TYPE = 1,
  LIST_DOMAIN_TYPE = 2,
  MULTIAXIS_DOMAIN_TYPE = 3,
  PHYLOG_LIST_DOMAIN_TYPE = 4,
  SPATIAL_DOMAIN_TYPE = 5,
  TEMPORAL_DOMAIN_TYPE = 6,
  RANGE_DOMAIN_TYPE = 7
};

inline const char* ToString(DomainType v)
{
  switch (v)
  {
    case HYPER_SLAB_DOMAIN_TYPE:         return "HyperSlab";
    case PHYLOG_HYPER_SLAB_DOMAIN_TYPE:  return "PhyLogHyperSlab";
    case LIST_DOMAIN_TYPE:               return "List";
    case TEMPORAL_DOMAIN_TYPE:           return "Temporal";
    case MULTIAXIS_DOMAIN_TYPE:          return "MultiAxisDomain";
    case PHYLOG_LIST_DOMAIN_TYPE:        return "PhyLogList";
    case SPATIAL_DOMAIN_TYPE:            return "Spatial";
    case RANGE_DOMAIN_TYPE:              return "Range";
    default:                             return "[Unknown]";
  }
}

namespace xidx{
class XidxDataType{
  
public:
  static const std::string FLOAT_32;
  static const std::string FLOAT_64;
  static const std::string INT_32;
  static const std::string UINT_32;
  static const std::string INT_16;
  static const std::string UINT_16;
  static const std::string INT_8;
  static const std::string UINT_8;
  
  int n_components;
  NumberType type;
  int bit_precision;
  
  XidxDataType(NumberType _type, int _n_components, int _bit_precision){
    type=_type;
    n_components=_n_components;
    bit_precision=_bit_precision;
  }
};
}
#endif
