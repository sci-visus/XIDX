#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

using namespace idx_metadata;

int write_simple(const char* filepath, int n_attributes, int n_timesteps){
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

  // Create one level that contains the grid
  std::shared_ptr<Level> level(new Level());
  ret = level->add_datagrid(grid);

  // Create timesteps that contains the level
  for(int i=0; i < n_timesteps; i++){
    std::shared_ptr<TimeStep> ts(new TimeStep());
    ret = ts->add_level(level);

    ts->set_timestep(i, float(i+10));
    meta.add_timestep(ts);
  }

  meta.save();

  return 0;
}

int write_hpc(const char* filepath, int n_attributes, int n_timesteps){
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

  // Create one level that contains the grid
  std::shared_ptr<Level> level(new Level());
  ret = level->add_datagrid(grid);

  // Create timesteps that contains the level
  for(int i=0; i < n_timesteps; i++){
    std::shared_ptr<TimeStep> ts(new TimeStep());
    ret = ts->add_level(level);

    ts->set_timestep(i, float(i+10));
    meta.add_timestep(ts);
  }

  meta.save();

  return 0;
}

int main(int argc, char** argv){

  if(argc < 2){
    fprintf(stderr, "Usage: write file_path [n_attributes] [n_timesteps]\n");

    return 1;
  }

  int n_attributes = 4;
  int n_timesteps = 5;

  if(argc > 2)
    n_attributes = atoi(argv[2]);

  if(argc > 3)
    n_timesteps = atoi(argv[3]);

  clock_t start, finish;
  start = clock();
  
  //int ret = write_simple(argc, argv);
  int ret = write_hpc(argv[1], n_attributes, n_timesteps);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
