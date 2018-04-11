##
## BSD 3-Clause License
## 
## Copyright (c) 2010-2018 ViSUS L.L.C., 
## Scientific Computing and Imaging Institute of the University of Utah
## 
## ViSUS L.L.C., 50 W. Broadway, Ste. 300, 84101-2044 Salt Lake City, UT
## University of Utah, 72 S Central Campus Dr, Room 3750, 84112 Salt Lake City, UT
##  
## All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
## 
## * Redistributions of source code must retain the above copyright notice, this
## list of conditions and the following disclaimer.
## 
## * Redistributions in binary form must reproduce the above copyright notice,
## this list of conditions and the following disclaimer in the documentation
## and/or other materials provided with the distribution.
## 
## * Neither the name of the copyright holder nor the names of its
## contributors may be used to endorse or promote products derived from
## this software without specific prior written permission.
## 
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
## DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
## DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
## SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
## CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
## OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
## 
## For additional information about this project contact: pascucci@acm.org
## For support: support@visus.net
## 
##
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