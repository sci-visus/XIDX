#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>

#include "xidx/xidx.h"

using namespace xidx;

int write_simple(const char* filepath, int n_attributes, int n_timesteps, bool time_hyperslab=false){
  XidxFile meta(filepath); // default: use simple layout

  //TODO check if metadata file is valid

  // Create a group to collect a time series
  std::shared_ptr<Group> time_group(new Group("TimeSeries", GroupType::TEMPORAL_GROUP_TYPE));
  
  // Create a data source for this group
  // if a variable does not redefine a data source the group source will be used
  std::shared_ptr<DataSource> file(new DataSource("data", "file_path"));
  time_group->AddDataSource(file);
  
  const int n_dims = 3;
  
  // Create the time domain
  
  std::shared_ptr<Domain> time_dom;
  
  if(time_hyperslab){// Create an hyperslab time domain (start, step, count)
    time_dom = std::make_shared<TemporalHyperSlabDomain>(new TemporalHyperSlabDomain("Time"));
    int32_t log_time[n_dims] = {0,1,static_cast<int32_t>(n_timesteps)};
    double phy_time[n_dims] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    std::dynamic_pointer_cast<TemporalHyperSlabDomain>(time_dom)->setDomain(n_dims, phy_time, log_time);
  }
  else{// Create series of timestep values
    time_dom = std::make_shared<TemporalListDomain>(new ListDomain("Time"));
    
    for(int i=0; i < n_timesteps; i++){
      std::dynamic_pointer_cast<TemporalListDomain>(time_dom)->AddDomainItem(i, float(i+10));
    }
  }

  // Set the time group domain to use the time domain we just created
  time_group->SetDomain(time_dom);

  // Create a new group to collect a set of variables that share the same spatial domain
  std::shared_ptr<Group> grid(new Group("L0", GroupType::SPATIAL_GROUP_TYPE)); // default static group
  
  // Create a spatial domain
  std::shared_ptr<SpatialDomain> space_dom(new SpatialDomain("Grid"));

  uint32_t dims[3] = {10, 20, 30};// logical dims
  double o[3] = {0, 0, 0};         // origin x y z
  double d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Set topology and geometry of the spatial domain
  int ret = space_dom->SetTopology(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, n_dims,
      dims);
  ret = space_dom->SetGeometry(GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, n_dims, o, d);

  // Set the domain for the spatial group
  grid->SetDomain(space_dom);

  // Add some variables to the spatial group
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->AddVariable(name, NumberType::FLOAT_NUMBER_TYPE, 32);
  }

  // Define a new domain, group and file for a different set of variables
  std::shared_ptr<SpatialDomain> lat_dom(new SpatialDomain("Latitude"));
  lat_dom->SetTopology(TopologyType::DIM_1D_TOPOLOGY_TYPE, 320);
  
  std::shared_ptr<Group> d1_vars(new Group("d1_vars", GroupType::SPATIAL_GROUP_TYPE, lat_dom));
  
  std::shared_ptr<DataSource> lat_file(new DataSource("latitude", "file_path"));
  d1_vars->AddDataSource(lat_file);
  
  Variable* temp = d1_vars->AddVariable("latitude", XidxDataType::FLOAT_32);
  if(!temp)
    printf("error\n");

  temp->AddAttribute("unit", "deg");
  temp->AddAttribute("valid_min", "0");
  temp->AddAttribute("valid_max", "180.0");

//  char some_data[64];
//  std::vector<uint32_t> adims = {64,1};
  // TODO use dummy raw writer for this, no metadata API
  //temp->set_raw_data(adims, some_data, "./");

  // we can define datatypes and set to our variables
  XidxDataType dtype(NumberType::FLOAT_NUMBER_TYPE, 1, 32);
  
//  DataItem data_item(FormatType::IDX_FORMAT, dtype, file);
//  
//  grid->AddVariable("custom0", data_item, space_dom); // can also get a list of XidxDataItem
//  grid->AddVariable("custom1", data_item, space_dom);
//  grid->AddVariable("custom2", data_item, space_dom);
  
  time_group->AddGroup(grid);
  time_group->AddGroup(d1_vars);
  
  meta.SetRootGroup(time_group);
  meta.save();

  printf("%zu timeteps written in %s\n", meta.GetNumberOfGroups(), filepath);

  return 0;
}

int main(int argc, char** argv){

  if(argc < 2){
    fprintf(stderr, "Usage: write file_path [layout_type] [n_attributes] [n_timesteps] [n_levels]\n");

    return 1;
  }

  int n_attributes = 4;
  int n_timesteps = 3;
  int n_levels = 2;


  if(argc > 2)
    n_attributes = atoi(argv[2]);

  if(argc > 3)
    n_timesteps = atoi(argv[3]);

  if(argc > 4)
    n_levels = atoi(argv[4]);

  clock_t start, finish;
  start = clock();
  
  int ret = write_simple(argv[1], n_attributes, n_timesteps);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
