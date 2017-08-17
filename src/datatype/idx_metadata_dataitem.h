#ifndef IDX_METADATA_DATAITEM_H_
#define IDX_METADATA_DATAITEM_H_

#include "idx_metadata_parsable.h"
#include "idx_metadata_enums.h"
#include "idx_metadata_information.h"

namespace idx_metadata{

class DataItem : public idx_metadata::Parsable{

public:
  std::string name;
  ItemType itemType;
  std::string dimensions;
  NumberType numberType;
  std::string precision;
  std::string reference;
  EndianType endianType;
  FormatType formatType;
  std::string text;
  std::vector<Information> information;

  xmlNodePtr objToXML(xmlNode* parent, const char* text=NULL){
    xmlNodePtr data_node = xmlNewChild(parent, NULL, BAD_CAST "DataItem", BAD_CAST text);
    xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(formatType));
    xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(numberType));
    xmlNewProp(data_node, BAD_CAST "Precision", BAD_CAST precision.c_str());
    xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(endianType));
    xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());

    for(auto info: information){
      xmlNodePtr info_node = info.objToXML(data_node);
    }

    return data_node;
  };
  
  int XMLToObj(xmlNodePtr node){
    if(!idx_metadata::is_node_name(node,"DataItem"))
      return -1;

    formatType = FormatType::IDX_FORMAT; // TODO remove this hardcoded
            
    const char* num_type = idx_metadata::getProp(node, "NumberType");
    if(num_type != NULL){
      for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++)
        if (strcmp(num_type, ToString(static_cast<NumberType>(t)))==0){
          numberType = static_cast<NumberType>(t);
          break;
        }
    }
    
    precision = idx_metadata::getProp(node, "Precision");
    dimensions = idx_metadata::getProp(node, "Dimensions");

    const char* end_type = idx_metadata::getProp(node, "Endian");
    if (end_type != NULL){
      for(int t=EndianType::LITTLE_ENDIANESS; t <= NATIVE_ENDIANESS; t++)
        if (strcmp(end_type, ToString(static_cast<EndianType>(t)))==0){
          endianType = static_cast<EndianType>(t);
          break;
        }
    }

    text = reinterpret_cast<const char*>(node->children->content);

    return 0;
  };

};

}

#endif