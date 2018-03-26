
#ifndef XIDX_TYPES_H_
#define XIDX_TYPES_H_

#include <string>
#include "xidx/xidx_config.h"

//TODO change name to ToString since it returns a const char*

namespace xidx{
  typedef uint32_t INDEX_TYPE;
  typedef double PHY_TYPE;
}

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
  RECT_GEOMETRY_TYPE = 6
};

inline const char* ToString(GeometryType v)
{
  switch (v)
  {
    case XYZ_GEOMETRY_TYPE:               return "XYZ";
    case XY_GEOMETRY_TYPE:                return "XY";
    case X_Y_Z_GEOMETRY_TYPE:             return "X_Y_Z";
    case VXVYVZ_GEOMETRY_TYPE:            return "VxVyVz";
    case ORIGIN_DXDYDZ_GEOMETRY_TYPE:     return "Origin_DxDyDz";
    case ORIGIN_DXDY_GEOMETRY_TYPE:       return "Origin_DxDy";
    case RECT_GEOMETRY_TYPE:              return "Rect";
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
  LIST_DOMAIN_TYPE = 1,
  MULTIAXIS_DOMAIN_TYPE = 2,
  SPATIAL_DOMAIN_TYPE = 3,
  RANGE_DOMAIN_TYPE = 4
};

inline const char* ToString(DomainType v)
{
  switch (v)
  {
    case HYPER_SLAB_DOMAIN_TYPE:         return "HyperSlab";
    case LIST_DOMAIN_TYPE:               return "List";
    case MULTIAXIS_DOMAIN_TYPE:          return "MultiAxisDomain";
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

// TODO add vector types
const std::string XidxDataType::FLOAT_32="1*float32";
const std::string XidxDataType::FLOAT_64="1*float64";
const std::string XidxDataType::INT_32="1*int32";
const std::string XidxDataType::UINT_32="1*uint32";
const std::string XidxDataType::INT_16="1*int16";
const std::string XidxDataType::UINT_16="1*int16";
const std::string XidxDataType::INT_8="1*int8";
const std::string XidxDataType::UINT_8="1*uint8";
  
}

namespace std{
  std::string to_string(xidx::DataSource const& d){
    return "";
  }
}

namespace xidx{
  
// trim from start
static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
  return ltrim(rtrim(s));
}
  
inline std::string ToString(const std::vector<xidx::INDEX_TYPE>& vec){
  std::string str="";
  for(auto& v: vec)
    str+=std::to_string(v)+" ";
  
  return trim(str);
}
  

inline std::vector<INDEX_TYPE> ToIndexVector(std::string s){
  std::vector<INDEX_TYPE> vec;
  
  std::string delimiter = " ";
  
  if(s.find(delimiter) == std::string::npos){
    vec.push_back(stoi(s));
    return vec;
  }
  
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    vec.push_back(stoi(token));
    s.erase(0, pos + delimiter.length());
  }

  vec.push_back(stoi(s));
  
  return vec;
}

}

#endif
