#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

int main(int argc, char** argv){
    IDX_Metadata meta("test.xmf");

    uint32_t dims[3] = {10, 20, 30};
    float o[3] = {0, 0, 0};
    float d[3] = {1.f, 1.f, 1.f};

    int ret = meta.set_global_grid(TopologyType::CORECT_3D_MESH_TOPOLOGY_TYPE, 
        dims, GeometryType::ORIGIN_DXDYDZ_GEOMETRY_TYPE, o, d);
    
    for(int i=0; i < 10; i++){
      char name[32];
      sprintf(name, "var_%d", i);
      meta.add_attribute(name, NumberType::FLOAT_NUMBER_TYPE, 4);
    }

    for(int i=0; i < 10; i++)
        meta.add_timestep(i, float(i));

    meta.save();

    return ret;
}
