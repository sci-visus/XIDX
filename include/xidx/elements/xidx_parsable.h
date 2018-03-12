#ifndef XIDX_PARSABLE_INTERFACE_H_
#define XIDX_PARSABLE_INTERFACE_H_

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx_parse_utils.h"

namespace xidx{

class Group;
class Domain;
class DataSource;
  
static const char* GetProp(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

inline bool IsNodeName(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

class Parsable{
  
public:
  std::string name;
  Parsable* parent=nullptr;
    
  int SetParent(Parsable* _parent){ parent = _parent; return 0;}
  
  virtual xmlNode* Serialize(xmlNode* parent, const char* text=NULL) = 0;
  virtual int Deserialize(xmlNode* node) = 0;

  virtual std::string GetXPath() { return xpath_prefix; }
  
protected:
  std::string xpath_prefix="//";
  
  template<typename T>
  Parsable* FindFirst(T* obj1){
    return FindFirst(obj1, this->parent);
  }
  
  template<typename T, typename U>
  Parsable* FindFirst(T* obj1, U* obj2){
    if(obj2 == nullptr)
      return nullptr;
    
    //printf(">>going through %s\n", ((Parsable*)obj2)->name.c_str());
    if(std::is_same<T, U>::value){
      return (T*)obj2;
    }
    else
      return FindFirst(obj1, ((Parsable*)obj2)->parent);
      
  }
  
};

};

#endif
