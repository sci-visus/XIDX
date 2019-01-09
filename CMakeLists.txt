project(XIDX)
cmake_minimum_required(VERSION 2.8)

option(BUILD_PYTHON_WRAPPER "Build python wrapper" TRUE)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(XIDX_IS_SUBMODULE 0)
else()
    set(XIDX_IS_SUBMODULE 1)
    set(CMAKE_FOLDER_PREFIX "")
endif()

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

# CMAKE_INSTALL_PREFIX

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "" FORCE)
endif()

if (WIN32)
    string(REPLACE "\\" "/" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif()

MESSAGE(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

# Dependencies
find_package(LibXml2)

if(LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
elseif(WIN32)

  if(NOT EXISTS ${CMAKE_BINARY_DIR}/libxml2)
    message("Extracting libxml2...")
    execute_process(
      COMMAND powershell -c "expand-archive -path '${PROJECT_SOURCE_DIR}/external/libxml2.zip' -destinationpath 'libxml2'"
    )
  endif()
  add_subdirectory("${CMAKE_BINARY_DIR}/libxml2")
  set(LIBXML2_INCLUDE_DIR "${CMAKE_BINARY_DIR}/libxml2/libxml2/include")
  set(LIBXML2_LIBRARIES xml2) 
  include_directories(${LIBXML2_INCLUDE_DIR})
endif()

# Create library
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

if(BUILD_PYTHON_WRAPPER)
    # Include SWIG for python wrapper
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    # Include python
    if (NOT DEFINED PYTHON_VERSION)
        set(PYTHON_VERSION 3)
    endif()

    find_package(PythonInterp ${PYTHON_VERSION} REQUIRED)
    find_package(PythonLibs   ${PYTHON_VERSION} REQUIRED)
    message(STATUS "PYTHON_EXECUTABLE   ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON_LIBRARY      ${PYTHON_LIBRARY}")
    message(STATUS "PYTHON_INCLUDE_DIR  ${PYTHON_INCLUDE_DIR}")

    if (WIN32)
        list(LENGTH PYTHON_LIBRARY __n__)
        if (${__n__} EQUAL 1)
            set(PYTHON_DEBUG_LIBRARY     ${PYTHON_LIBRARY})
            set(PYTHON_RELEASE_LIBRARY   ${PYTHON_LIBRARY})
        else()
            # differentiate debug from release
            # example debug;aaaa;optimized;bbb
            list(FIND PYTHON_LIBRARY optimized __index__)
            if (${__index__} EQUAL -1)
                MESSAGE(ERROR "Problem with find python")
            endif()
            math(EXPR __next_index__ "${__index__}+1")
            list(GET PYTHON_LIBRARY ${__next_index__} PYTHON_RELEASE_LIBRARY)

            list(FIND PYTHON_LIBRARY debug __index__)
            if (${__index__} EQUAL -1)
                MESSAGE(ERROR "Problem with find python")
            endif()
            math(EXPR __next_index__ "${__index__}+1")
            list(GET PYTHON_LIBRARY ${__next_index__} PYTHON_DEBUG_LIBRARY)
        endif()
    endif()
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

    if(DEFINED VISUS_IS_SUBMODULE)
        install(TARGETS _xidx DESTINATION bin)
        install(FILES ${PROJECT_BINARY_DIR}/xidx.py DESTINATION bin)
    else()
        install(TARGETS _xidx DESTINATION lib)
        install(FILES ${PROJECT_BINARY_DIR}/xidx.py DESTINATION lib)
    endif()

    # Python deployment

    if (APPLE)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE MACHINE OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(REGEX REPLACE ".*-darwin([0-9]+).*" "\\1" _apple_ver "${MACHINE}")

        if (_apple_ver EQUAL "17")
            set(APPLE_OSX_VERSION 10.13)
        elseif (_apple_ver EQUAL "16")
            set(APPLE_OSX_VERSION 10.12)
        elseif (_apple_ver EQUAL "15")
            set(APPLE_OSX_VERSION 10_11)
        elseif (_apple_ver EQUAL "14")
            set(APPLE_OSX_VERSION 10_10)
        elseif (_apple_ver EQUAL "13")
            set(APPLE_OSX_VERSION 10_9)
        elseif (_apple_ver EQUAL "12")
            set(APPLE_OSX_VERSION 10_8)
        elseif (_apple_ver EQUAL "11")
            set(APPLE_OSX_VERSION 10_7)
        elseif (_apple_ver EQUAL "10")
            set(APPLE_OSX_VERSION 10_6)
        else()
            message(FATAL "Unknown osx version")
        endif()
        message(STATUS "APPLE_OSX_VERSION ${APPLE_OSX_VERSION}")
    endif()

    set(PYTHON_TAG cp${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})

    if (WIN32)

        set(PYTHON_PLAT_NAME win_amd64)

    elseif (APPLE)

        string(SUBSTRING ${APPLE_OSX_VERSION} 0 2 __major__)
        string(SUBSTRING ${APPLE_OSX_VERSION} 3 2 __minor__)
        set(PYTHON_PLAT_NAME macosx_${__major__}_${__minor__}_x86_64)

    else()

        execute_process(COMMAND lsb_release -is OUTPUT_VARIABLE __id__   OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND lsb_release -cs OUTPUT_VARIABLE __name__ OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(PYTHON_PLAT_NAME "${__id__}.${__name__}")

    endif()

    #set(PYTHON_EXECUTABLE python3)

    if (WIN32)
        set(PYTHON_SDIST_FORMAT zip)
    else()
        set(PYTHON_SDIST_FORMAT gztar)
    endif()

    if(NOT XIDX_IS_SUBMODULE)
        add_custom_target(sdist
                "${CMAKE_COMMAND}" -E echo "Installing setuptools..."
                COMMAND "${PYTHON_EXECUTABLE}" -m pip install --user --upgrade setuptools
                COMMAND "${CMAKE_COMMAND}" -E echo "Running sdist target..."
                COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/CMake/setup.py" -q sdist --formats=${PYTHON_SDIST_FORMAT}
                WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}")

        add_custom_target(bdist_wheel
                "${CMAKE_COMMAND}" -E echo "Installing setuptools wheel..."
                COMMAND "${PYTHON_EXECUTABLE}" -m pip install --user --upgrade setuptools wheel
                COMMAND "${CMAKE_COMMAND}" -E echo "Running bdist_wheel target..."
                COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/CMake/setup.py" -q bdist_wheel --python-tag=${PYTHON_TAG} --plat-name=${PYTHON_PLAT_NAME}
                WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
                DEPENDS xidx)
    endif()

endif()