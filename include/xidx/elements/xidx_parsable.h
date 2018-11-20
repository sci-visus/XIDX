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

#ifndef XIDX_PARSABLE_INTERFACE_H_
#define XIDX_PARSABLE_INTERFACE_H_

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx/xidx.h"

namespace xidx{

class Domain;
class DataSource;
  
static const char* GetProperty(xmlNode* node, std::string propName){
  return reinterpret_cast<const char*>(xmlGetProp(node, BAD_CAST propName.c_str()));
}

inline bool IsNodeName(xmlNode* node, std::string name){
  return strcmp(reinterpret_cast<const char*>(node->name), name.c_str())==0;
}

class Parsable{
  
private:
  Parsable* parent=nullptr;
  
public:
  std::string name;
  
  int SetParent(Parsable* _parent){ parent = _parent; return 0;}
  
  virtual xmlNode* Serialize(xmlNode* parent, const char* text=NULL) = 0;
  virtual int Deserialize(xmlNode* node, Parsable* parent) = 0;

  virtual std::string GetDataSourceXPath() { return xpath_prefix; }
  
  virtual std::string ClassName() const = 0;
  
  virtual const Parsable* FindParent(const std::string& class_name, const Parsable* obj2) const{
    if(obj2 == nullptr)
      return nullptr;
    if(obj2->ClassName() == class_name){
      return obj2;
    }
    else
      return FindParent(class_name, obj2->parent);
  }
  
  virtual Parsable* FindChild(const std::string& class_name) const{
    return nullptr;
  }
  
  virtual Parsable* GetParent() const { return parent; };
  
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
