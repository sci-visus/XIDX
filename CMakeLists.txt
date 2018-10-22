project(XIDX)
cmake_minimum_required(VERSION 2.8)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")# -stdlib=libc++")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")# -stdlib=libc++")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
endif()

#add_subdirectory(include)

add_library(xidx INTERFACE)
target_include_directories(xidx INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/XidxConfigVersion.cmake"
        VERSION 0.0
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS xidx
        EXPORT XidxTargets
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
        )

include(CMakePackageConfigHelpers)
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/XidxConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/XidxConfig.cmake"
        INSTALL_DESTINATION lib/cmake/mylib
)

install(EXPORT XidxTargets DESTINATION lib/cmake/xidx)
install(FILES "${PROJECT_BINARY_DIR}/XidxConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/XidxConfig.cmake"
        DESTINATION lib/cmake/xidx)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

add_subdirectory(examples/cpp)

# SWIG stuff
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# Include python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/include/xidx/xidx.i PROPERTIES CPLUSPLUS ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/xidx
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/xidx/elements
                    ${LIBXML2_INCLUDE_DIR})

# Add swig module
if (CMAKE_VERSION VERSION_LESS "3.8")
    swig_add_module(xidx python ${CMAKE_CURRENT_SOURCE_DIR}/include/xidx/xidx.i)
else()
    swig_add_library(xidx LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/xidx/xidx.i)
endif()

target_link_libraries(_xidx PUBLIC xidx ${PYTHON_LIBRARIES} ${LIBXML2_LIBRARIES})

install(TARGETS _xidx DESTINATION lib)
install(FILES ${PROJECT_BINARY_DIR}/xidx.py DESTINATION lib)


