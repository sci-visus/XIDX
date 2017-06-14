
include_directories(./)

file(GLOB SRCS "*.cpp")

add_library(idx_metadata ${SRCS})

target_link_libraries(idx_metadata ${LIBXML2_LIBRARIES})
