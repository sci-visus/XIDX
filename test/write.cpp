#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

using namespace idx_metadata;

int write_simple(const char* filepath, int n_attributes, int n_timesteps, bool time_hyperslab=false){
  IDX_Metadata meta(filepath); // default: use simple layout

  uint32_t dims[3] = {10, 20, 30};// logical dims
  float o[3] = {0, 0, 0};         // origin x y z
  float d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Create a grid
  std::shared_ptr<DataGrid> grid(new DataGrid());

  // Set grid metadata
  int ret = grid->set_grid("l0", TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->add_attribute(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  std::vector<Information> cf_info;
  cf_info.push_back(Information("unit", "Kelvin"));
  cf_info.push_back(Information("valid_min", "0"));
  cf_info.push_back(Information("valid_max", "10000"));

  grid->add_attribute("temperature", NumberType::FLOAT_NUMBER_TYPE, 4, cf_info);

  // Create one level that contains the grid
  std::shared_ptr<Level> level(new Level());
  ret = level->add_datagrid(grid);

  if(time_hyperslab){
    uint32_t log_time[3] = {2,1,static_cast<uint32_t>(n_timesteps)};
    double phy_time[3] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    meta.add_time_hyperslab(log_time, phy_time, level);
  }else{
    // Create timesteps that contains the level
    for(int i=0; i < n_timesteps; i++){
      std::shared_ptr<TimeStep> ts(new TimeStep());
      ret = ts->add_level(level);

      ts->set_timestep(i, float(i+10));
      meta.add_timestep(ts);
    }
  }

  meta.save();

  return 0;
}

int write_hpc(const char* filepath, int n_attributes, int n_timesteps, int n_levels){
  IDX_Metadata meta(filepath, MetadataLayoutType::HPC); // default: use simple layout

  uint32_t dims[3] = {10, 20, 30};// logical dims
  float o[3] = {0, 0, 0};         // origin x y z
  float d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Create a grid
  std::shared_ptr<DataGrid> grid(new DataGrid());

  // Set grid metadata
  int ret = grid->set_grid("l0", TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->add_attribute(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  // Create timesteps that contains the level
  for(int i=0; i < n_timesteps; i++){
    std::shared_ptr<TimeStep> ts(new TimeStep());

    for(int l=0; l < n_levels; l++){
      // Create one level that contains the grid
      std::shared_ptr<Level> level(new Level());
      ret = level->add_datagrid(grid);
      ret = ts->add_level(level);
    }

    ts->set_timestep(i, float(i+10));
    meta.add_timestep(ts);
  }

  meta.save();

  return ret;
}

int main(int argc, char** argv){

  if(argc < 2){
    fprintf(stderr, "Usage: write file_path [layout_type] [n_attributes] [n_timesteps] [n_levels]\n");

    return 1;
  }

  int layout_type = 0;
  int n_attributes = 4;
  int n_timesteps = 3;
  int n_levels = 2;

  if(argc > 2)
    layout_type = atoi(argv[2]);

  if(argc > 3)
    n_attributes = atoi(argv[3]);

  if(argc > 4)
    n_timesteps = atoi(argv[4]);

  if(argc > 5)
    n_levels = atoi(argv[5]);

  clock_t start, finish;
  start = clock();
  
  int ret = -1;

  if(layout_type == 0){
    ret = write_simple(argv[1], n_attributes, n_timesteps);
    ret = write_simple(std::string(std::string(argv[1])+"_hyper").c_str(), n_attributes, n_timesteps, true);
  }
  else 
    ret = write_hpc(argv[1], n_attributes, n_timesteps, n_levels);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
