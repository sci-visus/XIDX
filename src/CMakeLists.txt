
include_directories(./)
include_directories(./layout)
include_directories(./datatype)

set(SRCS
    xidx.cpp
    layout/xidx_layout.cpp
    layout/xidx_simple_layout.cpp
    layout/xidx_hpc_layout.cpp
)

add_library(xidx ${SRCS})

string(CONCAT SRC_HEADERS "xidx.h;"
                          "xidx_config.h;"
                          "layout/xidx_hpc_layout.h;"
                          "layout/xidx_simple_layout.h;"
                          "layout/xidx_layout.h;"
                          "datatype/xidx_attribute.h;"
                          "datatype/xidx_datagrid.h;"
                          "datatype/xidx_dataitem.h;"
                          "datatype/xidx_enums.h;"
                          "datatype/xidx_geometry.h;"
                          "datatype/xidx_grid.h;"
                          "datatype/xidx_information.h;"
                          "datatype/xidx_level.h;"
                          "datatype/xidx_parsable.h;"
                          "datatype/xidx_parse_utils.h;"
                          "datatype/xidx_time.h;"
                          "datatype/xidx_timestep.h;"
                          "datatype/xidx_topology.h"
                       )

set_target_properties(xidx PROPERTIES PUBLIC_HEADER 
                      "${SRC_HEADERS}")

target_link_libraries(xidx ${LIBXML2_LIBRARIES})

install(TARGETS xidx 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)
