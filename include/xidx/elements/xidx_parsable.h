#ifndef XIDX_PARSABLE_INTERFACE_H_
#define XIDX_PARSABLE_INTERFACE_H_

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx/xidx.h"

namespace xidx{

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
  virtual int Deserialize(xmlNode* node, Parsable* parent) = 0;

  virtual std::string GetXPath() { return xpath_prefix; }
  
  virtual std::string GetClassName() = 0;
  
  virtual Parsable* FindParent(const std::string& class_name, Parsable* obj2) const{
    if(obj2 == nullptr)
      return nullptr;
    printf(">>going through %s\n", ((Parsable*)obj2)->name.c_str());
    if(obj2->GetClassName() == class_name){
      return obj2;
    }
    else
      return FindParent(class_name, obj2->parent);
  }
  
  virtual Parsable* FindChild(const std::string& class_name) const{
    return nullptr;
  }
  
protected:
  std::string xpath_prefix="//";
  
//  template<typename T>
//  Parsable* FindFirst(T* obj1){
//    return FindParent(obj1, this->parent);
//  }
//  
//  template<class T, class U>
//  T* FindParent(T* obj1, U* obj2){
//    if(obj2 == nullptr)
//      return nullptr;
//    printf("same Group1 %d\n", std::is_same<Group,Group>::value);
//    printf("same Group2 %d\n", std::is_same<Group*,Group*>::value);
//    printf("same Group3 %d\n", std::is_same<T*,U*>::value);
//    printf(">>going through %s\n", ((Parsable*)obj2)->name.c_str());
//    if(std::is_same<T*, U*>::value){
//      return (T*)obj2;
//    }
//    else
//      return FindParent(obj1, obj2->parent);
//      
//  }
  
  
  
};

};

#endif
