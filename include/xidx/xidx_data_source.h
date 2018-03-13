

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
  
  virtual int Deserialize(xmlNodePtr node) override{
    if(!xidx::IsNodeName(node,"DataSource"))
      return -1;
    
    name = xidx::GetProp(node, "Name");
    url = xidx::GetProp(node, "Url");
    
    return 0;
  }


  virtual std::string GetXPath() override {
    xpath_prefix=parent->GetXPath();
    xpath_prefix+="/DataSource";
//    xpath_prefix+="[@Name="+name+"]";
    xpath_prefix+="[@Name=\""+name+"\"]";
    
    return xpath_prefix;
  };
  
  virtual std::string GetClassName() override { return "DataSource"; };
};

};

namespace std{
  std::string to_string(xidx::DataSource const& d);
}


#endif
