
#ifndef XIDX_INDEX_SPACE_H_
#define XIDX_INDEX_SPACE_H_

#include <ostream>
#include <vector>
#include "xidx.h"

namespace xidx{
  
// TODO this is a very basic implementation, which at the
// moment simply wraps a vector. In the future we want to use
// rects, list of points, evaluate overlaps, etc.
// N is dimensions, T is the type of the content

template<typename T>
using IndexSpace = std::vector<T>;
  
//template<int N, typename T>
//class IndexSpace{
//
//private:
//  std::vector<T> values;
//  
//public:
//  IndexSpace(std::vector<T> _values){ values = _values; }
//  
//  void std::vector<T> GetLinearizedValues() { return values; }
//  
//
//};
  
};

#endif
