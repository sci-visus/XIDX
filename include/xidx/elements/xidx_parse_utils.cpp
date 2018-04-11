/*
 * BSD 3-Clause License
 * 
 * Copyright (c) 2010-2018 ViSUS L.L.C., 
 * Scientific Computing and Imaging Institute of the University of Utah
 * 
 * ViSUS L.L.C., 50 W. Broadway, Ste. 300, 84101-2044 Salt Lake City, UT
 * University of Utah, 72 S Central Campus Dr, Room 3750, 84112 Salt Lake City, UT
 *  
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For additional information about this project contact: pascucci@acm.org
 * For support: support@visus.net
 * 
 */

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xidx_parse_utils.h"
#include "xidx_attribute.h"

#define MY_ENCODING "ISO-8859-1"

namespace xidx{


// int parse_level(xmlNode *space_grid, std::shared_ptr<Level> lvl){
//   for (xmlNode* cur_node = space_grid; cur_node; cur_node = cur_node->next) {
//     if (cur_node->type == XML_ELEMENT_NODE) {
//       if(is_node_name(cur_node, "Grid")){
//         std::shared_ptr<DataGrid> dgrid(new DataGrid());
//         Grid grid;

//         grid.name = getProp(cur_node, "Name");

//         for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
//           if (inner_node->type == XML_ELEMENT_NODE) {
            
//             if(is_node_name(inner_node, "Topology")){
//               grid.topology.XMLToObj(inner_node);
//             }
//             else if(is_node_name(inner_node, "Geometry")){
//               grid.geometry.XMLToObj(inner_node);
//             }
//             else if(is_node_name(inner_node, "Attribute")){
//               Attribute att;

//               att.XMLToObj(inner_node);
              
//               grid.attribute.push_back(att);
              
//             }

//           }
//         }

//         dgrid->set_grid(grid);
//         lvl->add_datagrid(dgrid);
//       }
//     }
//   }

//   return 0;
// }


}