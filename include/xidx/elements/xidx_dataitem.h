#ifndef xidx_dataITEM_H_
#define xidx_dataITEM_H_

#include "xidx/xidx.h"

namespace xidx{

class Variable;

namespace defaults{
  const FormatType DATAITEM_FORMAT_TYPE = FormatType::XML_FORMAT;
  const NumberType DATAITEM_NUMBER_TYPE = NumberType::FLOAT_NUMBER_TYPE;
  const std::string DATAITEM_BIT_PRECISION = "32";
  const std::string DATAITEM_N_COMPONENTS = "1";
  const EndianType DATAITEM_ENDIAN_TYPE = EndianType::LITTLE_ENDIANESS;
}

class DataItem : public xidx::Parsable{
  friend class DataSource;
  
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
  
  int SetDefaults(){
    format_type=defaults::DATAITEM_FORMAT_TYPE;
    number_type=defaults::DATAITEM_NUMBER_TYPE;
    bit_precision=defaults::DATAITEM_BIT_PRECISION;
    n_components=defaults::DATAITEM_N_COMPONENTS;
    endian_type=defaults::DATAITEM_ENDIAN_TYPE;
    file_ref=nullptr;
    return 0;
  }
  
  DataItem(Parsable* _parent){
    parent = _parent;
    SetDefaults();
  }
  
  DataItem(const DataItem& i){
    name=i.name;
    dimensions=i.dimensions;
    number_type=i.number_type;
    bit_precision=i.bit_precision;
    n_components=i.n_components;
    reference=i.reference;
    text=i.text;
    endian_type=i.endian_type;
    format_type=i.format_type;
    file_ref=i.file_ref;
    attributes=i.attributes;
  }
  
  DataItem(std::string dtype, Parsable* _parent){
    parent = _parent;
    SetDefaults();
    
    ParseDType(dtype);
  }
  
  DataItem(FormatType format, XidxDataType dtype, std::shared_ptr<DataSource> file, Parsable* _parent){
    parent = _parent;
    SetDefaults();
    
    format_type=format;
    
    number_type=dtype.type;
    bit_precision=std::to_string(dtype.bit_precision);
    n_components=std::to_string(dtype.n_components);
    
    file_ref=nullptr;
    
  }
  
  DataItem(FormatType format, std::string dtype, std::shared_ptr<DataSource> file, Parsable* _parent){
    parent = _parent;
    SetDefaults();
    
    format_type=format;
    ParseDType(dtype);
    file_ref=file;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent_node, const char* text=NULL) override{
    xmlNodePtr data_node = xmlNewChild(parent_node, NULL, BAD_CAST "DataItem", BAD_CAST this->text.c_str());
    
    if(name.size())
      xmlNewProp(data_node, BAD_CAST "Name", BAD_CAST name.c_str());
    
    if(format_type != defaults::DATAITEM_FORMAT_TYPE)
      xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(format_type));
    
      xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST ToString(number_type));
    if(strcmp(bit_precision.c_str(), defaults::DATAITEM_BIT_PRECISION.c_str()) != 0 || format_type == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "BitPrecision", BAD_CAST bit_precision.c_str());
    if(endian_type != defaults::DATAITEM_ENDIAN_TYPE)
      xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST ToString(endian_type));

    if(dimensions.size())
      xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST dimensions.c_str());

    if(n_components != defaults::DATAITEM_N_COMPONENTS)
      xmlNewProp(data_node, BAD_CAST "ComponentNumber", BAD_CAST n_components.c_str());

    if(file_ref != nullptr)
      xmlNodePtr file_node = file_ref->Serialize(data_node);
    
#if XIDX_DEBUG_XPATHS
    else if(format_type != FormatType::XML_FORMAT){
      
      Parsable* source=nullptr;
      Parsable* curr_parent=parent;
      while(curr_parent!=nullptr){
        Parsable* parent_group=FindParent("Group", curr_parent);
        source = parent_group->FindChild("DataSource");
        
        if(source!=nullptr && source->GetClassName()=="DataSource")
          break;
        
        curr_parent = parent_group->parent;
      }
      
      if(source!=nullptr){
        xmlNodePtr variable_node = xmlNewChild(data_node, NULL, BAD_CAST "xi:include", NULL);
        xmlNewProp(variable_node, BAD_CAST "xpointer", BAD_CAST ("xpointer("+source->GetXPath()+")").c_str());
      }
    }
#endif
    
    for(auto att: attributes){
      xmlNodePtr att_node = att.Serialize(data_node);
    }

    return data_node;
  };
  
  virtual int Deserialize(xmlNodePtr node) override{
    if(!xidx::IsNodeName(node,"DataItem"))
      return -1;

    if(node->children != nullptr)
      text = (char*)(node->children->content);
    
    const char* name_s = xidx::GetProp(node, "Name");
    
    if(name_s != nullptr)
      name = name_s;
    
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

    //if(format_type != FormatType::IDX_FORMAT) { // Ignore dimensions for IDX
      const char* val_dimensions = xidx::GetProp(node, "Dimensions");
      if(val_dimensions == NULL){
        //assert(false);
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

    if(format_type != FormatType::XML_FORMAT){
      Parsable* parent_group = FindParent("Group", parent);

//      if(node != nullptr)
//        file_ref->Deserialize(node->children);
    }
    
    if(node->children != nullptr){
      for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
          if (cur_node->type == XML_ELEMENT_NODE) {
            if(IsNodeName(cur_node, "DataSource")){
              file_ref = std::make_shared<DataSource>(new DataSource());
              file_ref->Deserialize(cur_node);
              
            }
        }
      }
    }
    
    return 0;
  };
  
  virtual std::string GetClassName() override { return "DataItem"; };
  
private:
  
  int ParseDType(std::string dtype){
    if(!std::isdigit(dtype[0])){ // passed name, not dtype
      name = dtype;
      SetDefaults();
      return 0;
    }
    
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
    
    return 0;
  }

  
};
  
}

#endif
