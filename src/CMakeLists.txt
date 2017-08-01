
include_directories(./)

#file(GLOB SRCS "*.cpp")

set(SRCS
    idx_metadata.cpp
    idx_metadata_parse_utils.cpp
    idx_metadata_layout.cpp
    idx_metadata_simple_layout.cpp
    idx_metadata_hpc_layout.cpp
)

add_library(idx_metadata ${SRCS})

target_link_libraries(idx_metadata ${LIBXML2_LIBRARIES})
