

#ifndef IDX_METADATA_HPC_LAYOUT_H_
#define IDX_METADATA_HPC_LAYOUT_H_

#include "idx_metadata_layout.h"
#include "idx_metadata_datatypes.h"

namespace idx_metadata{

class IDX_Metadata_HPC_Layout : public IDX_Metadata_Layout{
private:
  int save_hpc_level(std::shared_ptr<Level> lvl, int n, std::shared_ptr<TimeStep> ts, const char* time_path);
  int save_hpc_timestep(std::shared_ptr<TimeStep> ts);
  int load_hpc_grid(std::string gpath, std::shared_ptr<TimeStep> ts);
  int load_hpc_timestep(std::string& tpath);

public:

  IDX_Metadata_HPC_Layout(IDX_Metadata* idx_metadata) : IDX_Metadata_Layout(idx_metadata){};

  virtual int load();
  virtual int save();
  virtual std::string get_file_path(int timestep, int level, CenterType ctype);

};

};

#endif
