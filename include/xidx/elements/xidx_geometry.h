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

#ifndef XIDX_GEOMETRY_H_
#define XIDX_GEOMETRY_H_

#include "xidx/xidx.h"

namespace xidx{

class Geometry : public Parsable{
public:
  
  enum GeometryType{
    XYZ_GEOMETRY_TYPE = 0,
    XY_GEOMETRY_TYPE = 1,
    X_Y_Z_GEOMETRY_TYPE = 2,
    VXVYVZ_GEOMETRY_TYPE = 3,
    ORIGIN_DXDYDZ_GEOMETRY_TYPE = 4,
    ORIGIN_DXDY_GEOMETRY_TYPE = 5,
    RECT_GEOMETRY_TYPE = 6
  };
  
  static inline const char* toString(GeometryType v)
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
  
public:
  std::string name;
  GeometryType type;
  std::vector<DataItem> items;
  
  Geometry(){}
  
  Geometry(GeometryType _type){
    type = _type;
  }
  
  Geometry(GeometryType _type, DataItem item){
    type = _type;
    items.push_back(item);
  }

  xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{

    xmlNodePtr geometry_node = xmlNewChild(parent, NULL, BAD_CAST "Geometry", NULL);
    xmlNewProp(geometry_node, BAD_CAST "Type", BAD_CAST toString(type));
    
    for(auto item: items)
      xmlNodePtr item_node = item.serialize(geometry_node);

    return geometry_node;
  };
  
  int deserialize(xmlNodePtr node, Parsable *_parent) override{
    if(!isNodeName(node,"Geometry"))
      return -1;

    setParent(_parent);
    //name = xidx::getProp(node, "Name");
            
    const char* geo_type = xidx::getProp(node, "Type");

    for(int t=GeometryType::XYZ_GEOMETRY_TYPE; t <= RECT_GEOMETRY_TYPE; t++)
      if (strcmp(geo_type, toString(static_cast<GeometryType>(t)))==0)
          type = static_cast<GeometryType>(t);

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(isNodeName(inner_node, "DataItem")){
        DataItem geo_dataitem(this);
        geo_dataitem.deserialize(inner_node, this);

        items.push_back(geo_dataitem);
      }
    }

    return 0;
  };
  
  size_t getVolume() const{
    size_t total = 1;
    for(auto item: items){
      for(int i=0; i < item.dimensions.size(); i++)
        total *= item.dimensions[i];
    }
    return total;
  }
  
  virtual std::string getClassName() const override { return "Geometry"; };
};

}

#endif
