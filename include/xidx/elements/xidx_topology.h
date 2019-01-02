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

#ifndef XIDX_TOPOLOGY_H_
#define XIDX_TOPOLOGY_H_

#include "xidx/xidx.h"

namespace xidx{

class Topology : public Parsable{
public:
  enum TopologyType{
    NO_TOPOLOGY_TYPE = 0,
    RECT_2D_MESH_TOPOLOGY_TYPE = 1,
    CORECT_2D_MESH_TOPOLOGY_TYPE = 2,
    RECT_3D_MESH_TOPOLOGY_TYPE = 3,
    CORECT_3D_MESH_TOPOLOGY_TYPE = 4,
    DIM_1D_TOPOLOGY_TYPE = 5
  };
  
  static inline const char* toString(TopologyType v)
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

public:

  std::vector<Attribute> attributes;
  std::vector<DataItem> items;
  TopologyType type;
  std::vector<INDEX_TYPE> dimensions;

  xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{

    xmlNodePtr topology_node = xmlNewChild(parent, NULL, BAD_CAST "Topology", NULL);

    xmlNewProp(topology_node, BAD_CAST "Type", BAD_CAST toString(type));
    xmlNewProp(topology_node, BAD_CAST "Dimensions", BAD_CAST xidx::toString(dimensions).c_str());
    
    for(auto item: items)
      xmlNodePtr item_node = item.serialize(topology_node);

    return topology_node;
  };
  
  int deserialize(xmlNodePtr node, Parsable *_parent) override{
    if(!isNodeName(node,"Topology"))
      return -1;
    
    setParent(_parent);

    const char* topo_type = getProp(node, "Type");

    for(int t=TopologyType::NO_TOPOLOGY_TYPE; t <= CORECT_3D_MESH_TOPOLOGY_TYPE; t++)
      if (strcmp(topo_type, toString(static_cast<TopologyType>(t)))==0)
          type = static_cast<TopologyType>(t);

    dimensions = toIndexVector(xidx::getProp(node, "Dimensions"));

    return 0;
  };
  
  virtual std::string getClassName() const override { return "Topology"; };

  ~Topology(){}

};

}

#endif
