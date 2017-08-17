#ifndef IDX_METADATA_LEVEL_H_
#define IDX_METADATA_LEVEL_H_

#include "idx_metadata_parsable.h"
#include "idx_metadata_datagrid.h"

namespace idx_metadata{

class Level : public Parsable{
private:
  std::vector<std::shared_ptr<DataGrid> > grids;

public:
  int add_datagrid(std::shared_ptr<DataGrid> level){ grids.push_back(level); return 0; }

  std::shared_ptr<DataGrid> get_datagrid(int g){ return grids[g]; }

  int clear(){ grids.clear(); return 0; }

  int get_n_datagrids() { return grids.size(); }

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){
    
    for(int i=0; i<get_n_datagrids(); i++){
      Grid& curr_grid = get_datagrid(i)->get_grid();
      xmlNodePtr curr_grid_node = xmlNewChild(parent, NULL, BAD_CAST "Grid", NULL);
      xmlNewProp(curr_grid_node, BAD_CAST "GridType", BAD_CAST ToString(GridType::UNIFORM_GRID_TYPE));
      xmlNewProp(curr_grid_node, BAD_CAST "Name", BAD_CAST curr_grid.name.c_str());

      xmlNodePtr topology_node = curr_grid.topology.objToXML(curr_grid_node);

      xmlNodePtr geometry_node = curr_grid.geometry.objToXML(curr_grid_node);

      int ref_attributes = get_datagrid(i)->get_reference_attributes();
      if(i > 0 && ref_attributes > 0){
        xmlNodePtr xattributes_node = xmlNewChild(curr_grid_node, NULL, BAD_CAST "xi:include", NULL);
        xmlNewProp(xattributes_node, BAD_CAST "xpointer", BAD_CAST (std::string("xpointer(//Xdmf/Domain/Grid[1]/Grid[").append(string_format("%d%s",ref_attributes,"]/Attribute"))).c_str());
      }else{
        if(i==0 || ref_attributes == 0){ // list all the attributes if reference grid (0) or referencing disabled
          for(auto att: curr_grid.attribute)
            xmlNodePtr attribute_node = att.objToXML(curr_grid_node);
        }
      }

    }

    return parent;
  };
  
  int XMLToObj(xmlNodePtr node){
    for (xmlNode* cur_node = node; cur_node; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
        if(is_node_name(cur_node, "Grid")){
          std::shared_ptr<DataGrid> dgrid(new DataGrid());
          Grid grid;

          grid.name = getProp(cur_node, "Name");

          for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
            if (inner_node->type == XML_ELEMENT_NODE) {
              
              if(is_node_name(inner_node, "Topology")){
                grid.topology.XMLToObj(inner_node);
              }
              else if(is_node_name(inner_node, "Geometry")){
                grid.geometry.XMLToObj(inner_node);
              }
              else if(is_node_name(inner_node, "Attribute")){
                Attribute att;

                att.XMLToObj(inner_node);
                
                grid.attribute.push_back(att);
                
              }

            }
          }

          dgrid->set_grid(grid);
          add_datagrid(dgrid);
        }
      }
    }

    return 0;
  };

};

}
#endif