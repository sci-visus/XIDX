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

#ifndef XIDX_DATA_SOURCE_H_
#define XIDX_DATA_SOURCE_H_

#include "xidx.h"

namespace xidx{

class DataSource : public Parsable{

private:
  std::string url;

public:

  DataSource() {
    url = "undefined";
    name = "undefined";
  };
  
  DataSource(const DataSource* ds){
    url=ds->url;
    name=ds->name;
  }
  
  DataSource(std::string _name, std::string path){
    url = path;
    name = _name;
  };
  
  std::string GetUrl(){ return url; }
  
  int SetFilePath(std::string path){ url = path; return 0; }
  
  xmlNodePtr Serialize(xmlNode* parent_node, const char* text=NULL) override{
    xmlNodePtr variable_node = xmlNewChild(parent_node, NULL, BAD_CAST "DataSource", NULL);
    xmlNewProp(variable_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(variable_node, BAD_CAST "Url", BAD_CAST url.c_str());
    
    return variable_node;
  }
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    SetParent(_parent);
    
    if(!xidx::IsNodeName(node,"DataSource"))
      return -1;
    
    name = xidx::GetProp(node, "Name");
    url = xidx::GetProp(node, "Url");
    
    return 0;
  }


  virtual std::string GetDataSourceXPath() override {
    xpath_prefix=this->GetParent()->GetDataSourceXPath();
    xpath_prefix+="/DataSource";
    xpath_prefix+="[@Name=\""+name+"\"]";
    
    return xpath_prefix;
  };
  
  virtual std::string GetClassName() const override { return "DataSource"; };
  
};

};

#endif
