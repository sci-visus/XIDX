
#ifndef IDX_METADATA_TIMESTEP_H_
#define IDX_METADATA_TIMESTEP_H_

#include "idx_metadata_level.h"
#include "idx_metadata_information.h"

namespace idx_metadata{

class TimeStep{
private:
  std::vector<std::shared_ptr<Level> > levels;
  Information log_time_info;
  double time;
  std::string time_str;

public:

  int set_timestep(uint32_t log_time, double phy_time){
    log_time_info.name = "LogicalTime";
    log_time_info.value = string_format("%d", log_time);
    time = phy_time;
    time_str = string_format("%f", time);
    return 0;
  }

  int add_level(std::shared_ptr<Level> level){ levels.push_back(level); return 0; }

  std::shared_ptr<Level> get_level(int l){ return levels[l]; }

  int clear(){ levels.clear(); return 0;}

  int get_n_levels() { return levels.size(); }

  Information& get_log_time_info(){ return log_time_info; }

  int get_logical_time(){ return stoi(log_time_info.value); }

  double get_physical_time(){ return time; }

  const char* get_physical_time_str() { return time_str.c_str();}
};

}

#endif