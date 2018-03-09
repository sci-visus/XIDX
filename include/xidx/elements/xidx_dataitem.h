#ifndef xidx_dataITEM_H_
#define xidx_dataITEM_H_

#include "xidx_parsable.h"
#include "xidx_types.h"
#include "xidx_attribute.h"

namespace xidx{

namespace defaults{
  const FormatType DATAITEM_FORMAT_TYPE = FormatType::XML_FORMAT;
  const NumberType DATAITEM_NUMBER_TYPE = NumberType::FLOAT_NUMBER_TYPE;
  const std::string DATAITEM_BIT_PRECISION = "32";
  const std::string DATAITEM_N_COMPONENTS = "1";
  const EndianType DATAITEM_ENDIAN_TYPE = EndianType::LITTLE_ENDIANESS;
}

class DataItem : public xidx::Parsable{

public:
  std::string name;
  std::string dimensions;
  NumberType numberType;
  std::string bit_precision;
  std::string n_components;
  std::string reference;
  EndianType endianType;
  FormatType formatType;
  std::string text;
  std::vector<Attribute> attributes;

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr data_node = xmlNewChild(parent, NULL, BAD_CAST "DataItem", BAD_CAST text);
    if(formatType != defaults::DATAITEM_FORMAT_TYPE)
      xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(formatType));
    //if(numberType != defaults::DATAITEM_NUMBER_TYPE || formatType == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(numberType));
    if(strcmp(bit_precision.c_str(), defaults::DATAITEM_BIT_PRECISION.c_str()) != 0 || formatType == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "BitPrecision", BAD_CAST bit_precision.c_str());
    if(endianType != defaults::DATAITEM_ENDIAN_TYPE)
      xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(endianType));

    //if(formatType != FormatType::IDX_FORMAT) // Ignore dimensions for IDX datasets
    xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());

    xmlNewProp(data_node, BAD_CAST "ComponentNumber", BAD_CAST n_components.c_str());

    for(auto att: attributes){
      xmlNodePtr att_node = att.Serialize(data_node);
    }

    return data_node;
  };
  
  int Deserialize(xmlNodePtr node){
    if(!xidx::IsNodeName(node,"DataItem"))
      return -1;

    const char* form_type = xidx::GetProp(node, "Format");
    if(form_type != NULL){
      for(int t=FormatType::XML_FORMAT; t <= IDX_FORMAT; t++)
        if (strcmp(form_type, ToString(static_cast<FormatType>(t)))==0){
          formatType = static_cast<FormatType>(t);
          break;
        }
    }

    const char* num_type = xidx::GetProp(node, "NumberType");
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
    
    const char* val_precision = xidx::GetProp(node, "BitPrecision");
    if(val_precision == NULL)
      bit_precision = defaults::DATAITEM_BIT_PRECISION;
    else 
      bit_precision = val_precision;

    const char* val_components = xidx::GetProp(node, "ComponentNumber");
    if(val_components == NULL)
      n_components = defaults::DATAITEM_N_COMPONENTS;
    else 
      n_components = val_components;

    //if(formatType != FormatType::IDX_FORMAT) { // Ignore dimensions for IDX
      const char* val_dimensions = xidx::GetProp(node, "Dimensions");
      if(val_dimensions == NULL){
        assert(false);
        fprintf(stderr, "ERROR: Invalid dimension value for DataItem\n");
      }
      else 
        dimensions = val_dimensions;
    //}

    const char* end_type = xidx::GetProp(node, "Endian");
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
