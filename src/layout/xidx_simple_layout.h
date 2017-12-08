

#ifndef XIDX_SIMPLE_LAYOUT_H_
#define XIDX_SIMPLE_LAYOUT_H_

#include "xidx_layout.h"

namespace xidx{

class xidx_Simple_Layout : public xidx_Layout{

public:

  xidx_Simple_Layout(XIDX_File* xidx) : xidx_Layout(xidx){};

  virtual int load();
  virtual int save();
  virtual std::string get_idx_file_path(int timestep, int level, CenterType ctype);
};

};

#endif
