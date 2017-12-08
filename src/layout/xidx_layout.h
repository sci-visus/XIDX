

#ifndef XIDX_LAYOUT_H_
#define XIDX_LAYOUT_H_

#include <string>

#include "xidx.h"
#include "datatype/xidx_enums.h"

namespace xidx{

class XIDX_File;
class xidx_Layout{

public:

  XIDX_File* metadata;

  xidx_Layout(XIDX_File* xidx)
  : metadata(xidx){};

  virtual int load() = 0;
  virtual int save() = 0;
  virtual std::string get_idx_file_path(int timestep, int level, CenterType ctype) = 0;

  std::string read_layout_type();
};

};

#endif
