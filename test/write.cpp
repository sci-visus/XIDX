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

  const int ndims = 3;
  
  if(time_hyperslab){
    dom = std::make_shared<HyperSlabDomain>(new HyperSlabDomain("Time"));
    int32_t log_time[ndims] = {0,1,static_cast<int32_t>(n_timesteps)};
    double phy_time[ndims] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    std::static_pointer_cast<HyperSlabDomain>(dom)->setDomain(ndims, phy_time, log_time);
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
  float o[3] = {0, 0, 0};         // origin x y z
  float d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Set grid metadata
  int ret = space_dom->setTopology(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims);
  ret = space_dom->setGeometry(GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);

  //grid->setDomain(space_dom);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->addVariable(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  std::shared_ptr<SpatialDomain> lat_dom(new SpatialDomain("Latitude"));
  lat_dom->SetTopology(TopologyType::DIM_1D_TOPOLOGY_TYPE, 320);

  Variable* temp = grid->addVariable("latitude", XidxDType::FLOAT_32, lat_dom);
  if(!temp)
    printf("error\n");

  temp->addAttribute("unit", "deg");
  temp->addAttribute("valid_min", "0");
  temp->addAttribute("valid_max", "180.0");

  char some_data[64];
  std::vector<uint32_t> adims = {64,1};
  // TODO use dummy raw writer for this, no metadata API
  temp->set_raw_data(adims, some_data, "./");

  // we can define datatypes and set to our variables
  XidxDataType dtype(NumberType::FLOAT_NUMBER_TYPE, 32, 1);

  XidxFile f("file_path");
  XidxDataItem data_item(XidxFormat::IDX, dtype, f);
  
  grid->addVariable("custom0", space_dom, data_item); // can also get a list of XidxDataItem
  grid->addVariable("custom1", space_dom, data_item);
  grid->addVariable("custom2", space_dom, data_item);

  timestep_item->addGroup(grid);

  time_group->addGroupItem(timestep);
  meta.addGroup(time_group);
  meta.save();

  printf("%d timeteps written in %s\n", meta.get_n_timesteps(), filepath);

  return 0;
}

int write_hpc(const char* filepath, int n_attributes, int n_timesteps, bool time_hyperslab=false){
  XIDX_File meta(filepath); // default: use simple layout

  //TODO check if metadata file is valid

  // Create the time series
  std::shared_ptr<XidxGroup> time_group(new XidxGroup("TimeSeries", GroupType::TEMPORAL));

  // using the hierarchical layout each GroupItem will be saved as different metadata
  // in a hierarchy of folder that follow the provided pattern
  // time_group->setLayout(XidxLayout::HIERARCHICAL, "t%9d");

  std::shared_ptr<XidxGroupItem> timestep_item(new XidxVariadicGroupItem("Timestep", "folder_path"));

  std::shared_ptr<XidxGroup> grid(new XidxGroup("L0", XIDX_GroupType::SPATIAL), "folder_path");
  std::shared_ptr<XidxSpatialDomain> space_dom(new XidxSpatialDomain("Grid"));

  uint32_t dims[3] = {10, 20, 30};// logical dims
  float o[3] = {0, 0, 0};         // origin x y z
  float d[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Set grid metadata
  int ret = space_dom->setTopology(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims);
  ret = space_dom->setGeometry(GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);

  //grid->setDomain(space_dom);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->addVariable(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  std::shared_ptr<XidxSpatialDomain> lat_dom(new XidxSpatialDomain("Latitude"));
  lat_dom->setTopology(TopologyType::1D, 320);

  XidxVariable* temp = grid->addVariable("latitude", XidxDType::FLOAT_32, lat_dom);
  if(!temp)
    printf("error\n");

  temp->addAttribute("unit", "deg");
  temp->addAttribute("valid_min", "0");
  temp->addAttribute("valid_max", "180.0");

  char some_data[64];
  std::vector<uint32_t> adims = {64,1};
  // TODO use dummy raw writer for this, no metadata API
  temp->set_raw_data(adims, some_data, "./");

  // we can define datatypes and set to our variables
  XidxDataType dtype(NumberType::FLOAT_NUMBER_TYPE, 32, 1);

  XidxFile f("file_path");
  XidxDataItem data_item(XidxFormat::IDX, dtype, f);
  
  grid->addVariable("custom0", space_dom, data_item); // can also get a list of XidxDataItem
  grid->addVariable("custom1", space_dom, data_item);
  grid->addVariable("custom2", space_dom, data_item);

  timestep_item->addGroup(grid);

  time_group->addGroupItem(timestep); // this will evaluate the parent domain and add a custom domain
                                      // for the specific timestep

  meta.addGroup(time_group);
  meta.save();

  printf("%d timeteps written in %s\n", meta.get_n_timesteps(), filepath);

  return 0;
}

int write_hpc_multigrid(const char* filepath, int n_attributes, int n_timesteps, int n_levels){
  XIDX_File meta(filepath, MetadataLayoutType::HPC); // default: use simple layout

  uint32_t dims[3] = {10, 20, 30};// logical dims
  float o1[3] = {0, 0, 0};         // origin x y z
  float d1[3] = {1.f, 1.f, 1.f};   // dx dy dz
  float o2[3] = {10.0, 20.0, 30.0};         // origin x y z
  float d2[3] = {1.f, 1.f, 1.f};   // dx dy dz

  // Create a grid
  std::shared_ptr<DataGrid> grid1(new DataGrid());
  std::shared_ptr<DataGrid> grid2(new DataGrid());

  // Set grid metadata
  int ret = grid1->set_grid("g0", TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o1, d1);

  ret = grid2->set_grid("g1", TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o2, d2);

  // Set attributes
  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid1->add_attribute(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  for(int i=0; i < n_attributes; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid2->unset_reference_attributes();
    grid2->add_attribute(name, NumberType::INT_NUMBER_TYPE, 4, AttributeType::VECTOR_ATTRIBUTE_TYPE, 3);
  }

  grid2->unset_reference_attributes();
  grid2->add_attribute("custom0", "1*uint32");
  grid2->add_attribute("custom1", "1*float64");
  grid2->add_attribute("custom1", "3*float32");

  // Create timesteps that contains the level
  for(int i=0; i < n_timesteps; i++){
    std::shared_ptr<TimeStep> ts(new TimeStep());

    for(int l=0; l < n_levels; l++){
      // Create one level that contains the grid
      std::shared_ptr<Level> level(new Level());
      ret = level->add_datagrid(grid1);
      ret = level->add_datagrid(grid2);
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
    ret = write_hpc_multigrid(argv[1], n_attributes, n_timesteps, n_levels);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
