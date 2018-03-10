#ifndef xidx_dataITEM_H_
#define xidx_dataITEM_H_

#include "xidx/xidx.h"

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
  NumberType number_type;
  std::string bit_precision;
  std::string n_components;
  std::string reference;
  std::string text;
  EndianType endian_type;
  FormatType format_type;
  std::shared_ptr<DataSource> file_ref;
  std::vector<Attribute> attributes;
  
  DataItem(){
    format_type=defaults::DATAITEM_FORMAT_TYPE;
    number_type=defaults::DATAITEM_NUMBER_TYPE;
    bit_precision=defaults::DATAITEM_BIT_PRECISION;
    n_components=defaults::DATAITEM_N_COMPONENTS;
    endian_type=defaults::DATAITEM_ENDIAN_TYPE;
  }
  
  DataItem(std::string dtype){
    PopulateDType(dtype);
  }
  
  DataItem(FormatType format, XidxDataType dtype, std::shared_ptr<DataSource> file){
    format_type=format;
    
    PopulateDType(dtype);
    
    // TODO
    file_ref=file;
    
  }

  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr data_node = xmlNewChild(parent, NULL, BAD_CAST "DataItem", BAD_CAST text);
    if(format_type != defaults::DATAITEM_FORMAT_TYPE)
      xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(format_type));
    //if(numberType != defaults::DATAITEM_NUMBER_TYPE || formatType == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(number_type));
    if(strcmp(bit_precision.c_str(), defaults::DATAITEM_BIT_PRECISION.c_str()) != 0 || format_type == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "BitPrecision", BAD_CAST bit_precision.c_str());
    if(endian_type != defaults::DATAITEM_ENDIAN_TYPE)
      xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(endian_type));

    //if(formatType != FormatType::IDX_FORMAT) // Ignore dimensions for IDX datasets
    xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());

    xmlNewProp(data_node, BAD_CAST "ComponentNumber", BAD_CAST n_components.c_str());

    xmlNodePtr file_node = file_ref->Serialize(data_node);
    
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
          format_type = static_cast<FormatType>(t);
          break;
        }
    }

    const char* num_type = xidx::GetProp(node, "NumberType");
    if(num_type != NULL){
      for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++)
        if (strcmp(num_type, ToString(static_cast<NumberType>(t)))==0){
          number_type = static_cast<NumberType>(t);
          break;
        }
    }
    else{
      number_type = defaults::DATAITEM_NUMBER_TYPE;
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
          endian_type = static_cast<EndianType>(t);
          break;
        }
    }
    else
      endian_type = defaults::DATAITEM_ENDIAN_TYPE;

    file_ref->Deserialize(node->children);
    
    return 0;
  };
  
private:
  
  void PopulateDType(XidxDataType dtype){
    bit_precision = dtype.bit_precision;
    number_type = dtype.type;
    n_components = dtype.n_components;
  }
  
  void PopulateDType(std::string dtype){
    size_t comp_idx= dtype.find_last_of("*\\");
    // TODO this uses only 1 digit component
    n_components = dtype.substr(0,comp_idx);
    
    size_t num_idx=0;
    for(int i=comp_idx;i<dtype.size(); i++)
      if(!std::isdigit(dtype[i]))
        num_idx++;
      else
        break;
    
    std::string ntype = dtype.substr(comp_idx+1, num_idx-1);
    bit_precision = dtype.substr(num_idx+1);
    
    for(int t=NumberType::CHAR_NUMBER_TYPE; t <= UINT_NUMBER_TYPE; t++){
      std::string numType = ToString(static_cast<NumberType>(t));
      std::transform(numType.begin(), numType.end(), numType.begin(), ::tolower);
      
      if (strcmp(numType.c_str(), ntype.c_str())==0){
        number_type = static_cast<NumberType>(t);
        break;
      }
    }
  }
  
};
  
}

#endif
