

#ifndef XIDX_HPC_LAYOUT_H_
#define XIDX_HPC_LAYOUT_H_

#include "xidx_layout.h"

namespace xidx{

class xidx_HPC_Layout : public xidx_Layout{
private:
  int save_hpc_level(std::shared_ptr<Level> lvl, int n, std::shared_ptr<TimeStep> ts, const char* time_path);
  int save_hpc_timestep(std::shared_ptr<TimeStep> ts);
  int load_hpc_grid(std::string gpath, std::shared_ptr<TimeStep> ts);
  int load_hpc_timestep(std::string& tpath);

public:

  xidx_HPC_Layout(XIDX_File* xidx) : xidx_Layout(xidx){};

  virtual int load();
  virtual int save();
  virtual std::string get_idx_file_path(int timestep, int level, CenterType ctype);

};

};

#endif
