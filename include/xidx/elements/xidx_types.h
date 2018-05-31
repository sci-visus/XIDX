/*


 * Copyright (c) 2017 University of Utah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XIDX_TYPES_H_
#define XIDX_TYPES_H_

#include <string>
#include <sstream>
#include <algorithm>

#include "xidx/xidx_config.h"


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

namespace xidx{
class XidxDataType{
public:
  enum NumberType{
    CHAR_NUMBER_TYPE = 0,
    UCHAR_NUMBER_TYPE = 1,
    FLOAT_NUMBER_TYPE = 2,
    INT_NUMBER_TYPE = 3,
    UINT_NUMBER_TYPE = 4
  };
  
  static inline const char* ToString(NumberType v)
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
  
public:
  // TODO add vector types
  constexpr static const char* FLOAT_32="1*float32";
  constexpr static const char* FLOAT_64="1*float64";
  constexpr static const char* INT_32="1*int32";
  constexpr static const char* UINT_32="1*uint32";
  constexpr static const char* INT_16="1*int16";
  constexpr static const char* UINT_16="1*int16";
  constexpr static const char* INT_8="1*int8";
  constexpr static const char* UINT_8="1*uint8";
  
  int n_components;
  NumberType type;
  int bit_precision;
  
  XidxDataType() {}
  
  //static inline const std::string FLOAT_32(){ return "1*float32"; }
  
  XidxDataType(NumberType _type, int _n_components, int _bit_precision){
    type=_type;
    n_components=_n_components;
    bit_precision=_bit_precision;
  }
};
  
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
