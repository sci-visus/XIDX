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

  std::shared_ptr<Parsable> getNext(){
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
