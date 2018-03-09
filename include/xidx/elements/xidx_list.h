#ifndef XIDX_LIST_H_
#define XIDX_LIST_H_

#include <string>
#include <map>

#include "xidx_parsable.h"

namespace xidx{

class XidxList {

private:
  std::map<std::string, std::shared_ptr<Parsable> > map;
  std::map<std::string, std::shared_ptr<Parsable> >::iterator it;

public:
  XidxList(){ it = map.begin(); }
  
  void Insert(std::shared_ptr<Parsable> p){
    map[p->name] = p;
  }
  
  std::shared_ptr<Parsable> Get(std::string name){
    if(map.count(name))
      return map.at(name);
    else
      return NULL;
  }

  void InitIteration(){
    it = map.begin();
  }

  std::shared_ptr<Parsable> GetNext(){
    if(it == map.end())
      return NULL;
    else{
      std::shared_ptr<Parsable> c = it->second;
      it++;
      return c;
    }
  }

  inline size_t size() const{ return map.size(); }

};

}

#endif
