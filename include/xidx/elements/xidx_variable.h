#ifndef XIDX_VARIABLE_H_
#define XIDX_VARIABLE_H_

#include <algorithm>
#include <sstream>
#include <fstream>

#include "xidx_parsable.h"
#include "xidx_dataitem.h"

namespace xidx{

namespace defaults{
  const CenterType VARIABLE_CENTER_TYPE = CenterType::CELL_CENTER;
}

class Variable : public xidx::Parsable{

public:
  std::vector<std::shared_ptr<DataItem> > data_items;
  CenterType center_type;
  std::vector<Attribute> attributes;

  // int get_n_components(){
  //   size_t found=data.dimensions.find_last_of(" \\");
  //   return stoi(data.dimensions.substr(found));
  // }

  // std::string get_dtype_str(){
  //   std::string numberType = ToString(data.numberType); 
  //   std::transform(numberType.begin(), numberType.end(), numberType.begin(), ::tolower);
  //   int n_bytes = stoi(data.precision)*8;
  //   return string_format("%d*%s%d",get_n_components(),numberType.c_str(),n_bytes);
  // }

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr variable_node = xmlNewChild(parent, NULL, BAD_CAST "Variable", NULL);
    xmlNewProp(variable_node, BAD_CAST "Name", BAD_CAST name.c_str());
    if(centerType != defaults::ATTRIBUTE_CENTER_TYPE)
      xmlNewProp(variable_node, BAD_CAST "Center", BAD_CAST ToString(centerType));

    for(auto item: data_items)
      xmlNodePtr data_node = item->Serialize(variable_node);

    for(auto& curr_att : attributes){
      xmlNodePtr info_node = curr_att.Serialize(variable_node);
    }

    return variable_node;
  };

  // int set_raw_data(std::vector<uint32_t>& dims, char* raw_data, char* filepath){
  //   std::stringstream ss;
  //   for(int i=0; i< dims.size(); i++)
  //     if(i < dims.size()-1)
  //       ss << dims[i] << " ";
  //     else
  //       ss << dims[i];

  //   set_raw_data(ss.str(), raw_data, filepath);

  //   return 0;
  // }

  // int set_raw_data(std::string dims, char* raw_data, char* filepath){
  //   data.formatType = FormatType::BINARY_FORMAT;
  //   data.dimensions = dims;

  //   std::vector<int> idims;
  //   std::istringstream f(dims);

  //   std::string s;
  //   while (getline(f, s, ' ')) {
  //     idims.push_back(atoi(s.c_str()));
  //   }

  //   size_t size = 1;
  //   for(auto d:idims)
  //     size *= d;

  //   std::stringstream ss;
  //   ss << name << ".raw";

  //   std::ofstream out;
  //   out.open(ss.str().c_str(), std::ios::out | std::ios::binary);
  //   out.write(raw_data, size);
  //   out.close();

  //   data.text = ss.str();

  //   return 0;
  // }

  // int get_raw_data(char* raw_data, size_t size){
  //   std::ifstream out;
  //   out.open(data.text.c_str(), std::ios::in | std::ios::binary);
  //   out.read(raw_data, size);
  //   out.close();

  //   return 0;
  // }

  int AddAttribute(std::string name, std::string value){
    Attribute att(name, value);
    attributes.push_back(att);
    return 0;
  }
  
  int Deserialize(xmlNodePtr node){
    if(!xidx::is_node_name(node,"Variable"))
      return -1;

    name = xidx::getProp(node, "Name");

    const char* center_type = xidx::getProp(node, "Center");
    if(center_type != NULL){
      for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
        if (strcmp(center_type, ToString(static_cast<CenterType>(t)))==0)
            centerType = static_cast<CenterType>(t);
    }
    else
      centerType = defaults::ATTRIBUTE_CENTER_TYPE;

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(xidx::is_node_name(inner_node, "DataItem")){
        data.Deserialize(inner_node);
      }
      else if(xidx::is_node_name(inner_node, "Attribute")){
        Attribute att;
        att.Deserialize(inner_node);
        attributes.push_back(att);
      }
    }

    return 0;
  };

};
}

#endif