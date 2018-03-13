
include_directories(../include/xidx/)
include_directories(../include/xidx/elements)

set(SRCS
    xidx_file.cpp
)

string(CONCAT SRC_HEADERS
             "../include/xidx/xidx.h;"
             "../include/xidx/xidx_file.h;"
             "../include/xidx/xidx_data_source.h;"
             "../include/xidx/xidx_config.h;"
)

string(CONCAT ELEMENTS_HEADERS
             "../include/xidx/elements/xidx_list.h;"
             "../include/xidx/elements/xidx_attribute.h;"
             "../include/xidx/elements/xidx_dataitem.h;"
             "../include/xidx/elements/xidx_domain.h;"
             "../include/xidx/elements/xidx_spatial_domain.h;"
             "../include/xidx/elements/xidx_hyperslab_domain.h;"
             "../include/xidx/elements/xidx_phylog_hyperslab_domain.h;"
             "../include/xidx/elements/xidx_list_domain.h;"
             "../include/xidx/elements/xidx_multiaxis_domain.h;"
             "../include/xidx/elements/xidx_phylog_list_domain.h;"
             "../include/xidx/elements/xidx_types.h;"
             "../include/xidx/elements/xidx_geometry.h;"
             "../include/xidx/elements/xidx_topology.h;"
             "../include/xidx/elements/xidx_group.h;"
             "../include/xidx/elements/xidx_variable.h;"
             "../include/xidx/elements/xidx_attribute.h;"
             "../include/xidx/elements/xidx_parsable.h;"
             "../include/xidx/elements/xidx_parse_utils.h;"
)

add_library(xidx ${SRCS} ${SRC_HEADERS} ${ELEMENTS_HEADERS})

string(CONCAT HEADERS ${SRC_HEADERS} ${ELEMENTS_HEADERS})

set_target_properties(xidx PROPERTIES PUBLIC_HEADER
                      $HEADERS)

target_link_libraries(xidx ${LIBXML2_LIBRARIES})

install(TARGETS xidx
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)
