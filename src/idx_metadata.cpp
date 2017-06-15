
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <set>

#include "idx_metadata_datatypes.h"
#include "idx_metadata_simple_layout.h"
#include "idx_metadata_hpc_layout.h"
#include "idx_metadata.h"
#include "idx_metadata_config.h"

using namespace idx_metadata;

IDX_Metadata::IDX_Metadata(const char* path, MetadataLayoutType _layout){

  set_correct_path(path);

  layoutType = _layout;
  loaded = false;

  switch(layoutType){
    case MetadataLayoutType::SIMPLE:
      layout = std::unique_ptr<IDX_Metadata_Simple_Layout>(new IDX_Metadata_Simple_Layout(this));
      break;
    case MetadataLayoutType::HPC:
      layout = std::unique_ptr<IDX_Metadata_HPC_Layout>(new IDX_Metadata_HPC_Layout(this));
      break;
    default:
      assert(false);
      break;
  }
};

int IDX_Metadata::add_timestep(std::shared_ptr<TimeStep> ts){ 
    timesteps.push_back(ts); 

    if(loaded)
      touched_ts.insert(ts->get_logical_time());
    return 0;
  }

int IDX_Metadata::load(){
    layout->load();
    loaded = true;
    return 0;
}

int IDX_Metadata::save(){
  layout->save();
  return 0;
}

std::string IDX_Metadata::get_idx_file_path(int timestep, int level, CenterType ctype){
  return layout->get_idx_file_path(timestep, level, ctype);
}

