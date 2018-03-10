#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>

#include "xidx/xidx.h"

using namespace xidx;

int write_simple(const char* filepath, int n_attributes, int n_timesteps, bool time_hyperslab=false){
  XidxFile meta(filepath); // default: use simple layout

  //TODO check if metadata file is valid

  // Create the time series
  std::shared_ptr<Group> time_group(new Group("TimeSeries", GroupType::TEMPORAL_GROUP_TYPE));

  std::shared_ptr<Domain> dom;

  const int n_dims = 3;
  
  if(time_hyperslab){
    dom = std::make_shared<HyperSlabDomain>(new HyperSlabDomain("Time"));
    int32_t log_time[n_dims] = {0,1,static_cast<int32_t>(n_timesteps)};
    double phy_time[n_dims] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    std::static_pointer_cast<HyperSlabDomain>(dom)->setDomain(n_dims, phy_time, log_time);
  }
  else{
    dom = std::make_shared<ListDomain>(new ListDomain("Time"));
    // Create series of timestep values
    for(int i=0; i < n_timesteps; i++){
      std::static_pointer_cast<ListDomain>(dom)->AddDomainItem(i, float(i+10));
    }
  }

  time_group->SetDomain(dom);

  std::shared_ptr<Group> grid(new Group("L0", GroupType::SPATIAL_GROUP_TYPE)); // default static group
  std::shared_ptr<SpatialDomain> space_dom(new SpatialDomain("Grid"));

  uint32_t dims[3] = {10, 20, 30};// logical dims
  double o[3] = {0, 0, 0};         // origin x y z
  double d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Set grid metadata
  int ret = space_dom->SetTopology(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, n_dims,
      dims);
  ret = space_dom->SetGeometry(GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, n_dims, o, d);

  grid->SetDomain(space_dom);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->AddVariable(name, NumberType::FLOAT_NUMBER_TYPE, 32);
  }

  std::shared_ptr<SpatialDomain> lat_dom(new SpatialDomain("Latitude"));
  lat_dom->SetTopology(TopologyType::DIM_1D_TOPOLOGY_TYPE, 320);

  Variable* temp = grid->AddVariable("latitude", XidxDataType::FLOAT_32, lat_dom);
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
  XidxDataType dtype(NumberType::FLOAT_NUMBER_TYPE, 32, 1);

  std::shared_ptr<DataSource> f(new DataSource("file_path"));
  DataItem data_item(FormatType::IDX_FORMAT, dtype, f);
  
  grid->AddVariable("custom0", data_item, space_dom); // can also get a list of XidxDataItem
  grid->AddVariable("custom1", data_item, space_dom);
  grid->AddVariable("custom2", data_item, space_dom);

  time_group->AddGroup(grid);
  //meta.AddRootGroup(time_group);
  meta.save();

  printf("%zu timeteps written in %s\n", meta.GetNumberOfGroups(), filepath);

  return 0;
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
  
  int ret = write_simple(argv[1], n_attributes, n_timesteps);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
