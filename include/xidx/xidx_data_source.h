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

#include <fstream>
#include "xidx.h"

namespace xidx{

class DataSource : public Parsable{

private:
  std::string url;
  bool inline_metadata;

public:

  DataSource() {
    url = "undefined";
    name = "undefined";
    inline_metadata = false;
  };
  
  DataSource(const DataSource* ds){
    url = ds->url;
    name = ds->name;
    inline_metadata = ds->inline_metadata;
  }
  
  DataSource(std::string _name, std::string path, bool do_inline_metadata=false){
    url = path;
    name = _name;
    inline_metadata = do_inline_metadata;
  };
  
  std::string getUrl(){ return url; }
  
  int setFilePath(std::string path){ url = path; return 0; }
  
  xmlNodePtr serialize(xmlNode *parent_node, const char *text = NULL) override{
    xmlNodePtr ds_node = xmlNewChild(parent_node, NULL, BAD_CAST "DataSource", NULL);
    xmlNewProp(ds_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(ds_node, BAD_CAST "Url", BAD_CAST url.c_str());
    
    if(inline_metadata){
      if (url.find("://") != std::string::npos)
        fprintf(stderr, "url data source inline not supported\n");
      
      xmlDocPtr doc = xmlReadFile(url.c_str(), NULL, 0);

      if (doc != NULL) {
        xmlNode* root_element = xmlDocGetRootElement(doc);
        ds_node->children = root_element;
      }
      else{
        //fprintf(stderr, "Failed to parse %s as XML\n", url.c_str());
        std::ifstream ifs(url.c_str());
        
        if(ifs){
          std::string content( (std::istreambuf_iterator<char>(ifs) ),
                              (std::istreambuf_iterator<char>()    ) );

          xmlNodePtr cdata = xmlNewCDataBlock(doc, (const xmlChar *)content.c_str(), content.size());
          xmlAddChild(ds_node,cdata);
        }
        else
          fprintf(stderr, "Unable to read file %s\n", url.c_str());
      }
    }
    
    return ds_node;
  }
  
  virtual int deserialize(xmlNodePtr node, Parsable *_parent) override{
    setParent(_parent);
    
    if(!xidx::isNodeName(node,"DataSource"))
      return -1;
    
    name = xidx::getProp(node, "Name");
    url = xidx::getProp(node, "Url");
    
    return 0;
  }

  virtual std::string getDataSourceXPath() override {
    xpath_prefix=this->getParent()->getDataSourceXPath();
    xpath_prefix+="/DataSource";
    xpath_prefix+="[@Name=\""+name+"\"]";
    
    return xpath_prefix;
  };
  
  int PopulateInlineMetadata(bool do_inline_metadata){
    inline_metadata = do_inline_metadata;
    return 0;
  }
  
  virtual std::string getClassName() const override { return "DataSource"; };
  
};

};

#endif
