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

#ifndef XIDX_ATTRIBUTE_H_
#define XIDX_ATTRIBUTE_H_

#include "xidx_parsable.h"

namespace xidx{

class Attribute: public xidx::Parsable{
public:
  Attribute(){};
  Attribute(std::string _name, std::string _value)
  { name=_name; value=_value; };

  std::string value;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    xmlNodePtr att_node = xmlNewChild(parent, NULL, BAD_CAST "Attribute", BAD_CAST text);
    xmlNewProp(att_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(att_node, BAD_CAST "Value", BAD_CAST value.c_str());

    return att_node;
  };
  
  int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    if(!xidx::IsNodeName(node,"Attribute"))
      return -1;
    
    SetParent(_parent);

    name = xidx::GetProp(node, "Name");
    value = xidx::GetProp(node, "Value");

    return 0;
  };
  
  virtual std::string GetClassName() const override { return "Attribute"; };

};
}

#endif
