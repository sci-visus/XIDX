#ifndef IDX_METADATA_DATAITEM_H_
#define IDX_METADATA_DATAITEM_H_

#include "idx_metadata_parsable.h"
#include "idx_metadata_enums.h"
#include "idx_metadata_information.h"

namespace idx_metadata{

namespace defaults{
  const FormatType DATAITEM_FORMAT_TYPE = FormatType::XML_FORMAT;
  const NumberType DATAITEM_NUMBER_TYPE = NumberType::FLOAT_NUMBER_TYPE;
  const std::string DATAITEM_PRECISION = "4";
  const EndianType DATAITEM_ENDIAN_TYPE = EndianType::LITTLE_ENDIANESS;
}

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
    if(formatType != defaults::DATAITEM_FORMAT_TYPE)
      xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(formatType));
    if(numberType != defaults::DATAITEM_NUMBER_TYPE || formatType == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(numberType));
    if(strcmp(precision.c_str(), defaults::DATAITEM_PRECISION.c_str()) != 0 || formatType == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "Precision", BAD_CAST precision.c_str());
    if(endianType != defaults::DATAITEM_ENDIAN_TYPE)
      xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(endianType));

    //if(formatType != FormatType::IDX_FORMAT) // Ignore dimensions for IDX datasets
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
    else{
      numberType = defaults::DATAITEM_NUMBER_TYPE;
    }
    
    const char* val_precision = idx_metadata::getProp(node, "Precision");
    if(val_precision == NULL)
      precision = defaults::DATAITEM_PRECISION;
    else 
      precision = val_precision;

    //if(formatType != FormatType::IDX_FORMAT) { // Ignore dimensions for IDX
      const char* val_dimensions = idx_metadata::getProp(node, "Dimensions");
      if(val_dimensions == NULL){
        assert(false);
        fprintf(stderr, "ERROR: Invalid dimension value for DataItem\n");
      }
      else 
        dimensions = val_dimensions;
    //}

    const char* end_type = idx_metadata::getProp(node, "Endian");
    if (end_type != NULL){
      for(int t=EndianType::LITTLE_ENDIANESS; t <= NATIVE_ENDIANESS; t++)
        if (strcmp(end_type, ToString(static_cast<EndianType>(t)))==0){
          endianType = static_cast<EndianType>(t);
          break;
        }
    }
    else
      endianType = defaults::DATAITEM_ENDIAN_TYPE;

    text = reinterpret_cast<const char*>(node->children->content);

    return 0;
  };

};

}

#endif