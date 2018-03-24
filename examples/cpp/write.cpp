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
    double phy_time[n_dims] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    std::dynamic_pointer_cast<TemporalHyperSlabDomain>(time_dom)->setDomain(n_dims, phy_time);
  }
  else{// Create series of timestep values
    time_dom = std::make_shared<TemporalListDomain>(new TemporalListDomain("Time"));
    
    for(int i=0; i < n_timesteps; i++){
      std::dynamic_pointer_cast<TemporalListDomain>(time_dom)->AddDomainItem(float(i+10));
    }
  }

  // Set the time group domain to use the time domain we just created
  time_group->SetDomain(time_dom);

  ////////////////////////////////////////////////////////////
  //
  //  Spatial grid with shared dataset
  //
  ////////////////////////////////////////////////////////////
  
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

  ///////////////////////////////////////////////////////////////
  //
  //  Spatial grid defined by an explicit set of axis (Climate)
  //
  ///////////////////////////////////////////////////////////////
  
  // Define a new domain, group and file for a different set of variables
  std::shared_ptr<MultiAxisDomain<double>> geo_dom(new MultiAxisDomain<double>("Geospatial"));
  Axis<double> latitude_axis("latitude");
  Axis<double> longitude_axis("longitude");
  
  // populate the axis with explicit values (will be written in the XML)
  for(int i=0; i < 10; i++){
    latitude_axis.AddDomainItem((double)i);
    longitude_axis.AddDomainItem((double)i*2);
  }
  
  // Add this axis to the domain
  geo_dom->AddAxis(latitude_axis);
  geo_dom->AddAxis(longitude_axis);
  
  // Create group for the variables defined in the geospatial domain
  std::shared_ptr<Group> geo_vars(new Group("geo_vars", GroupType::SPATIAL_GROUP_TYPE, geo_dom));
  
  // Create and add a variable to the group
  Variable* temp = geo_vars->AddVariable("geo_temperature", XidxDataType::FLOAT_32);
  if(!temp)
    printf("error\n");

  // Add attribute to the variable (key-value) pairs
  temp->AddAttribute("unit", "Celsius");
  temp->AddAttribute("valid_min", "-100.0");
  temp->AddAttribute("valid_max", "200.0");

  
  ///////////////////////////////////////////////////////////////
  //
  //  Rectilinear grid with coordinates saved on file
  //
  ///////////////////////////////////////////////////////////////
  
  /// Use a binary file to define a rectilinear grid coordinates
  int file_n_dims = 2;
  uint32_t file_dims[2] = {100, 200};
  
  std::shared_ptr<SpatialDomain> file_dom(new SpatialDomain("FileBasedDomain"));
  file_dom->SetTopology(TopologyType::RECT_2D_MESH_TOPOLOGY_TYPE, file_n_dims,
                         file_dims);
  
  // Create a DataSource that points to the file
  std::shared_ptr<DataSource> rect_grid_file(new DataSource("grid_data", "file_path"));
  
  // Create a DataItem which describes the content of the data
  DataItem file_item(FormatType::BINARY_FORMAT, XidxDataType::FLOAT_64, rect_grid_file, file_dom.get());
  
  // Create a geometry which will point to the file
  Geometry file_geom(GeometryType::XY_GEOMETRY_TYPE, file_item);
  
  file_dom->SetGeometry(file_geom);
  
  // Create group for the variables defined in the geospatial domain
  std::shared_ptr<Group> rect_grid_vars(new Group("rect_grid_vars", GroupType::SPATIAL_GROUP_TYPE, file_dom));
  
  rect_grid_vars->AddVariable("rect_var", NumberType::INT_NUMBER_TYPE, 32);
  
  // Add all the groups of variables to the time series
  time_group->AddGroup(grid);
  time_group->AddGroup(geo_vars);
  time_group->AddGroup(rect_grid_vars);
  
  // Set the root group of the metadata
  meta.SetRootGroup(time_group);
  // Write to disk
  meta.Save();

  printf("%zu timeteps written in %s\n", meta.GetNumberOfGroups(), filepath);

  return 0;
}

int write_time_variant(const char* filepath, int n_attributes, int n_timesteps, bool time_hyperslab=false){
  XidxFile meta(filepath); // default: use simple layout
  
  //TODO check if metadata file is valid
  
  // Create a group to collect a time series
  std::shared_ptr<Group> time_group(new Group("TimeSeries", GroupType::TEMPORAL_GROUP_TYPE));
  
  const int n_dims = 3;
  
  // Create the time domain
  
  std::shared_ptr<Domain> time_dom;
  
  if(time_hyperslab){// Create an hyperslab time domain (start, step, count)
    time_dom = std::make_shared<TemporalHyperSlabDomain>(new TemporalHyperSlabDomain("Time"));
    double phy_time[n_dims] = {2.0,float(n_timesteps-1)*0.1,float(n_timesteps)};
    std::dynamic_pointer_cast<TemporalHyperSlabDomain>(time_dom)->setDomain(n_dims, phy_time);
  }
  else{// Create series of timestep values
    time_dom = std::make_shared<TemporalListDomain>(new TemporalListDomain("Time"));
    
    for(int i=0; i < n_timesteps; i++){
      std::dynamic_pointer_cast<TemporalListDomain>(time_dom)->AddDomainItem(float(i+10));
    }
  }
  
  // Set the time group domain to use the time domain we just created
  time_group->SetDomain(time_dom);
  
  for(int t=0; t < n_timesteps; t++){
    // Create a data source for this timestep
    std::shared_ptr<DataSource> file(new DataSource("timestep"+std::to_string(t),
                                                    "timestep"+std::to_string(t)+"/file_path"));
    // Create a new group to collect a set of variables that share the same spatial domain
    std::shared_ptr<Group> grid(new Group("L0", GroupType::SPATIAL_GROUP_TYPE, VariabilityType::VARIABLE_VARIABILITY_TYPE)); // default static group
    
    grid->AddDataSource(file);
    
    // Create a spatial domain
    std::shared_ptr<SpatialDomain> space_dom(new SpatialDomain("Grid"));
    
    uint32_t dims[6] = {10, 20, 30}; // logical box (p1x,p2x,p1y,p2y,p1z,p2z)
    double box_phy[6] = {0.3, 4.2, 0.0, 9.4, 2.5, 19.0};   // dx dy dz
    
    // Set topology and geometry of the spatial domain
    int ret = space_dom->SetTopology(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, n_dims,
                                     dims);
    ret = space_dom->SetGeometry(GeometryType::RECT_GEOMETRY_TYPE, n_dims, box_phy);
    
    // Set the domain for the spatial group
    grid->SetDomain(space_dom);
    
    // Add some variables to the spatial group
    for(int i=0; i < n_attributes; i++){
      char name[32];
      sprintf(name, "var_%d", i);
      grid->AddVariable(name, NumberType::FLOAT_NUMBER_TYPE, 32);
    }
    
    time_group->AddGroup(grid);
  }

  meta.SetRootGroup(time_group);
  meta.Save();
  
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
  ret = write_time_variant((std::string(argv[1])+"_time").c_str(), n_attributes, n_timesteps);

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  return ret;
}
