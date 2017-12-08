
include_directories(./)
include_directories(./layout)
include_directories(./datatype)

set(SRCS
    idx_metadata.cpp
    layout/idx_metadata_layout.cpp
    layout/idx_metadata_simple_layout.cpp
    layout/idx_metadata_hpc_layout.cpp
)

add_library(idx_metadata ${SRCS})

string(CONCAT SRC_HEADERS "idx_metadata.h;"
                          "idx_metadata_config.h;"
                          "layout/idx_metadata_hpc_layout.h;"
                          "layout/idx_metadata_simple_layout.h;"
                          "layout/idx_metadata_layout.h;"
                          "datatype/idx_metadata_attribute.h;"
                          "datatype/idx_metadata_datagrid.h;"
                          "datatype/idx_metadata_dataitem.h;"
                          "datatype/idx_metadata_enums.h;"
                          "datatype/idx_metadata_geometry.h;"
                          "datatype/idx_metadata_grid.h;"
                          "datatype/idx_metadata_information.h;"
                          "datatype/idx_metadata_level.h;"
                          "datatype/idx_metadata_parsable.h;"
                          "datatype/idx_metadata_parse_utils.h;"
                          "datatype/idx_metadata_time.h;"
                          "datatype/idx_metadata_timestep.h;"
                          "datatype/idx_metadata_topology.h"
                       )

set_target_properties(idx_metadata PROPERTIES PUBLIC_HEADER 
                      "${SRC_HEADERS}")

target_link_libraries(idx_metadata ${LIBXML2_LIBRARIES})

install(TARGETS idx_metadata 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)
