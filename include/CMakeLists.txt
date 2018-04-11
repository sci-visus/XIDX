add_library(xidx INTERFACE)

target_include_directories(xidx INTERFACE ./)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

string(CONCAT SRC_HEADERS
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/xidx.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/xidx_file.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/xidx_data_source.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/xidx_config.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/xidx_index_space.h;"
)

string(CONCAT ELEMENTS_HEADERS
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_list.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_attribute.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_dataitem.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_spatial_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_hyperslab_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_phylog_hyperslab_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_list_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_multiaxis_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_phylog_list_domain.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_types.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_geometry.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_topology.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_group.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_variable.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_attribute.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_parsable.h;"
             "${CMAKE_CURRENT_SOURCE_DIR}/xidx/elements/xidx_parse_utils.h;"
)

target_sources(xidx INTERFACE 
                    ${SRC_HEADERS} 
                    ${ELEMENTS_HEADERS}
)

install(TARGETS xidx
        PUBLIC_HEADER DESTINATION include)