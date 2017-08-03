

#ifndef IDX_METADATA_H_
#define IDX_METADATA_H_

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <set>

#include "idx_metadata_layout.h"
#include "idx_metadata_config.h"
#include "idx_metadata_datatypes.h"

namespace idx_metadata{

class IDX_Metadata_Layout;
class IDX_Metadata{

private:
  std::vector<std::shared_ptr<TimeStep> > timesteps;
  MetadataLayoutType layoutType;
  std::string file_path;
  std::unique_ptr<idx_metadata::IDX_Metadata_Layout> layout;

  bool loaded;
  std::set<int> touched_ts;

  void set_correct_path(const char* path){
    file_path = std::string(path);
    size_t found=file_path.find_last_of(".\\");

    file_path=file_path.substr(0,found).append(IDX_METADATA_FILE_EXTENSION);
  }

public:

  IDX_Metadata(const char* path, MetadataLayoutType _layout=MetadataLayoutType::SIMPLE);

  int load();
  int save();

  std::string get_idx_file_path(int timestep, int level, CenterType ctype);
  std::string get_md_file_path(){ return file_path; }
  int set_md_file_path(const char* new_path){ set_correct_path(new_path); return 0; }

  int add_timestep(std::shared_ptr<TimeStep> ts);

  std::shared_ptr<TimeStep> get_timestep(int t){ return timesteps[t]; }

  int clear(){ timesteps.clear(); return 0; }

  int get_n_timesteps() { return timesteps.size(); }
};

};

#endif
