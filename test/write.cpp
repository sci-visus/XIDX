#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

using namespace idx_metadata;

int main(int argc, char** argv){
  IDX_Metadata meta("test.xmf"); // default: use simple layout

  uint32_t dims[3] = {10, 20, 30};
  float o[3] = {0, 0, 0};
  float d[3] = {1.f, 1.f, 1.f};

  std::shared_ptr<DataGrid> grid(new DataGrid());

  int ret = grid->set_grid("l0", TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
      dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);

  for(int i=0; i < 3; i++){
    char name[32];
    sprintf(name, "var_%d", i);
    grid->add_attribute(name, NumberType::FLOAT_NUMBER_TYPE, 4);
  }

  std::shared_ptr<Level> level(new Level());
  ret = level->add_datagrid(grid);

  for(int i=0; i < 5; i++){
    std::shared_ptr<TimeStep> ts(new TimeStep());
    ret = ts->add_level(level);

    ts->set_timestep(i, float(i+10));
    meta.add_timestep(ts);
  }

  meta.save();


  return ret;
}
