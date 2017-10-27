#ifndef IDX_METADATA_ATTRIBUTE_H_
#define IDX_METADATA_ATTRIBUTE_H_

#include <algorithm>
#include <sstream>
#include <fstream>

#include "idx_metadata_parsable.h"
#include "idx_metadata_dataitem.h"

namespace idx_metadata{

namespace defaults{
  const CenterType ATTRIBUTE_CENTER_TYPE = CenterType::CELL_CENTER;
  const AttributeType ATTRIBUTE_TYPE = AttributeType::SCALAR_ATTRIBUTE_TYPE;
}

class Attribute : public idx_metadata::Parsable{

public:
  std::string name;
  DataItem data; // TODO according to the format this should be a vector
  CenterType centerType;
  AttributeType attributeType;
  std::vector<Information> information;

  int get_n_components(){
    size_t found=data.dimensions.find_last_of(" \\");
    return stoi(data.dimensions.substr(found));
  }

  std::string get_dtype_str(){
    std::string numberType = ToString(data.numberType); 
    std::transform(numberType.begin(), numberType.end(), numberType.begin(), ::tolower);
    int n_bytes = stoi(data.precision)*8;
    return string_format("%d*%s%d",get_n_components(),numberType.c_str(),n_bytes);
  }

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){
    xmlNodePtr attribute_node = xmlNewChild(parent, NULL, BAD_CAST "Attribute", NULL);
    xmlNewProp(attribute_node, BAD_CAST "Name", BAD_CAST name.c_str());
    if(centerType != defaults::ATTRIBUTE_CENTER_TYPE)
      xmlNewProp(attribute_node, BAD_CAST "Center", BAD_CAST ToString(centerType));
    //if(attributeType != defaults::ATTRIBUTE_TYPE)
      xmlNewProp(attribute_node, BAD_CAST "AttributeType", BAD_CAST ToString(attributeType));

    xmlNodePtr data_node = data.objToXML(attribute_node, data.text.c_str());

    for(auto& curr_info : information){
      xmlNodePtr info_node = curr_info.objToXML(attribute_node);
    }

    return attribute_node;
  };

  int set_raw_data(std::string dims, char* raw_data, char* filepath){
    data.formatType = FormatType::BINARY_FORMAT;
    data.dimensions = dims;

    std::vector<int> idims;
    std::istringstream f(dims);

    std::string s;
    while (getline(f, s, ' ')) {
      idims.push_back(atoi(s.c_str()));
    }

    size_t size = 1;
    for(auto d:idims)
      size *= d;

    std::stringstream ss;
    ss << name << ".raw";

    std::ofstream out;
    out.open(ss.str().c_str(), std::ios::out | std::ios::binary);
    out.write(raw_data, size);
    out.close();

    data.text = ss.str();

    return 0;
  }

  int get_raw_data(char* raw_data, size_t size){
    std::ifstream out;
    out.open(data.text.c_str(), std::ios::in | std::ios::binary);
    out.read(raw_data, size);
    out.close();

    return 0;
  }

  int add_information(std::string name, std::string value){
    Information info(name, value);
    information.push_back(info);
    return 0;
  }
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"Attribute"))
      return -1;

    name = idx_metadata::getProp(node, "Name");

    const char* center_type = idx_metadata::getProp(node, "Center");
    if(center_type != NULL){
      for(int t=CenterType::NODE_CENTER; t <= EDGE_CENTER; t++)
        if (strcmp(center_type, ToString(static_cast<CenterType>(t)))==0)
            centerType = static_cast<CenterType>(t);
    }
    else
      centerType = defaults::ATTRIBUTE_CENTER_TYPE;

    const char* att_type = idx_metadata::getProp(node, "AttributeType");

    if(att_type != NULL){
      for(int t=AttributeType::SCALAR_ATTRIBUTE_TYPE; t <= TENSOR_ATTRIBUTE_TYPE; t++)
        if (strcmp(att_type, ToString(static_cast<AttributeType>(t)))==0)
            attributeType = static_cast<AttributeType>(t);
    }
    else
      attributeType = defaults::ATTRIBUTE_TYPE;

    for (xmlNode* inner_node = node->children->next; inner_node; inner_node = inner_node->next) {
      if(idx_metadata::is_node_name(inner_node, "DataItem")){
        data.XMLToObj(inner_node);
      }
      else if(idx_metadata::is_node_name(inner_node, "Information")){
        Information info;
        info.XMLToObj(inner_node);
        information.push_back(info);
      }
    }

    return 0;
  };

};
}

#endif