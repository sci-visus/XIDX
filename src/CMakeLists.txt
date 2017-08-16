
include_directories(./)
include_directories(./layout)
include_directories(./datatype)

#file(GLOB SRCS "*.cpp")

set(SRCS
    idx_metadata.cpp
    datatype/idx_metadata_parse_utils.cpp
    layout/idx_metadata_layout.cpp
    layout/idx_metadata_simple_layout.cpp
    layout/idx_metadata_hpc_layout.cpp
)

add_library(idx_metadata ${SRCS})

target_link_libraries(idx_metadata ${LIBXML2_LIBRARIES})

add_subdirectory(datatype)
add_subdirectory(layout)
