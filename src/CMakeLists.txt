
include_directories(./)

add_library(idx_metadata idx_metadata.cpp)

target_link_libraries(idx_metadata ${LIBXML2_LIBRARIES})
