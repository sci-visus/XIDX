
#include <cassert>
#include <string>
#include <sstream>
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

  time.type = TimeType::SINGLE_TIME_TYPE;

};

int IDX_Metadata::add_timestep(std::shared_ptr<TimeStep> ts){ 
  timesteps[ts->get_logical_time()] = ts; 

  if(loaded)
    touched_ts.insert(ts->get_logical_time());
  return 0;
}

int IDX_Metadata::add_time_hyperslab(uint32_t* log_time, double* phy_time, std::shared_ptr<Level> level){
  time.type = TimeType::HYPER_SLAB_TIME_TYPE;
  
  DataItem phy_item;
  phy_item.formatType = FormatType::XML_FORMAT;
  phy_item.dimensions = "3";
  phy_item.numberType = NumberType::FLOAT_NUMBER_TYPE;
  phy_item.precision = "8";
  phy_item.text = string_format("%f %f %f", phy_time[0],phy_time[1],phy_time[2]);

  Information log_info;
  log_info.name = "LogicalTime";
  log_info.value = string_format("%d %d %d", log_time[0],log_time[1],log_time[2]);

  phy_item.information.push_back(log_info);

  time.items.push_back(phy_item);

  std::shared_ptr<TimeStep> ts(new TimeStep());
  ts->set_timestep(log_time[0], phy_time[0]);
  ts->add_level(level);
  timesteps[ts->get_logical_time()] = ts;

  return 0;
}

std::shared_ptr<TimeStep> IDX_Metadata::get_timestep(int t){ 
  if(time.type == TimeType::SINGLE_TIME_TYPE) 
    return timesteps[t]; 
  else if(time.type == TimeType::HYPER_SLAB_TIME_TYPE){
    std::istringstream iss(time.items[0].text);
    std::string token;
    std::getline(iss, token, ' ');
    double start = stod(token);
    std::getline(iss, token, ' ');
    double stride = stod(token);

    std::shared_ptr<TimeStep> ts = timesteps.begin()->second;
    ts->set_timestep(t, start+stride*t);
    return ts;
  }
  else 
    return NULL;
}

int IDX_Metadata::get_n_timesteps() { 
  if(time.type == TimeType::SINGLE_TIME_TYPE) 
    return timesteps.size(); 
  else if(time.type == TimeType::HYPER_SLAB_TIME_TYPE){
    std::string ntime = std::string(time.items[0].text);
    size_t found=ntime.find_last_of(" \\");

    ntime=ntime.substr(found+1);
    return stoi(ntime);
  }
  else
    return -1;

}

int IDX_Metadata::load(){
  layout = std::unique_ptr<IDX_Metadata_Simple_Layout>(new IDX_Metadata_Simple_Layout(this));

  std::string layout_type = layout->read_layout_type();
  
  size_t found=layout_type.find_last_of("-\\");

  if(found != -1){ // HPC layout
    int hpc_level=std::stoi(layout_type.substr(found+1)); 
    if(hpc_level < 2){ // Use HPC Layout insted
      layout.release();
      layout = std::unique_ptr<IDX_Metadata_HPC_Layout>(new IDX_Metadata_HPC_Layout(this));
    }
  }

  layout->load();
  loaded = true;
  return 0;
}

int IDX_Metadata::save(){
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

  layout->save();
  return 0;
}

std::string IDX_Metadata::get_idx_file_path(int timestep, int level, CenterType ctype){
  return layout->get_idx_file_path(timestep, level, ctype);
}

