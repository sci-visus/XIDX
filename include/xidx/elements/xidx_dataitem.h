/*
 * Copyright (c) 2017 University of Utah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef xidx_dataITEM_H_
#define xidx_dataITEM_H_

#include <algorithm>
#include <sstream>
#include "xidx/xidx.h"

namespace xidx{

class Variable;
  
class Endianess{
public:
  enum EndianType{
    LITTLE_ENDIANESS = 0,
    BIG_ENDIANESS = 1,
    NATIVE_ENDIANESS = 2
  };
  
  static inline const char* ToString(EndianType v)
  {
    switch (v)
    {
      case LITTLE_ENDIANESS:    return "Little";
      case BIG_ENDIANESS:       return "Big";
      case NATIVE_ENDIANESS:    return "Native";
      default:                  return "[Unknown]";
    }
  }
};
  
class DataItem : public xidx::Parsable{
  friend class DataSource;
public:
  
  enum FormatType{
    XML_FORMAT = 0,
    HDF_FORMAT = 1,
    BINARY_FORMAT = 2,
    TIFF_FORMAT = 3,
    IDX_FORMAT = 4
  };
  
  static inline const char* ToString(FormatType v)
  {
    switch (v)
    {
      case XML_FORMAT:      return "XML";
      case HDF_FORMAT:      return "HDF";
      case BINARY_FORMAT:   return "Binary";
      case TIFF_FORMAT:     return "TIFF";
      case IDX_FORMAT:      return "IDX";
      default:              return "[Unknown]";
    }
  }
  
  class defaults{
  public:
    constexpr static const char* DATAITEM_BIT_PRECISION = "32";
    constexpr static const char* DATAITEM_N_COMPONENTS = "1";
    static const DataItem::FormatType DATAITEM_FORMAT_TYPE = DataItem::FormatType::XML_FORMAT;
    static const XidxDataType::NumberType DATAITEM_NUMBER_TYPE = XidxDataType::NumberType::FLOAT_NUMBER_TYPE;
    static const Endianess::EndianType DATAITEM_ENDIAN_TYPE = Endianess::EndianType::LITTLE_ENDIANESS;
  };
  
public:
  std::vector<INDEX_TYPE> dimensions;
  XidxDataType::NumberType number_type;
  //TODO change these strings into number types
  std::string bit_precision;
  std::string n_components;
  std::string reference;
  std::string text;
  Endianess::EndianType endian_type;
  DataItem::FormatType format_type;
  std::shared_ptr<DataSource> data_source;
  std::vector<Attribute> attributes;
  
  int SetDefaults(){
    format_type=defaults::DATAITEM_FORMAT_TYPE;
    number_type=defaults::DATAITEM_NUMBER_TYPE;
    bit_precision=defaults::DATAITEM_BIT_PRECISION;
    n_components=defaults::DATAITEM_N_COMPONENTS;
    endian_type=defaults::DATAITEM_ENDIAN_TYPE;
    data_source=nullptr;
    return 0;
  }
  
  DataItem(Parsable* _parent){
    SetParent(_parent);
    SetDefaults();
  }
  
  DataItem(const DataItem& i){
    SetParent(i.GetParent());
    name=i.name;
    dimensions=i.dimensions;
    number_type=i.number_type;
    bit_precision=i.bit_precision;
    n_components=i.n_components;
    reference=i.reference;
    text=i.text;
    endian_type=i.endian_type;
    format_type=i.format_type;
    data_source=i.data_source;
    attributes=i.attributes;
    values=i.values;
  }
  
  DataItem(std::string dtype, Parsable* _parent){
    SetParent(_parent);
    SetDefaults();
    
    ParseDType(dtype);
  }
  
  DataItem(FormatType format, XidxDataType dtype, std::shared_ptr<DataSource> file, Parsable* _parent){
    SetParent(_parent);
    SetDefaults();
    
    format_type=format;
    
    number_type=dtype.type;
    bit_precision=std::to_string(dtype.bit_precision);
    n_components=std::to_string(dtype.n_components);
    
    data_source=nullptr;
    
  }
  
  DataItem(FormatType format, std::string dtype, std::shared_ptr<DataSource> ds, Parsable* _parent){
    SetParent(_parent);
    SetDefaults();
    
    format_type=format;
    ParseDType(dtype);
    data_source=ds;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent_node, const char* text=NULL) override{
    
    if(values.size()>0 && format_type == FormatType::XML_FORMAT){
      std::stringstream stream_data;
      for(auto& v:values)
        stream_data<<v<<" ";
      this->text=std::string(stream_data.str());
    }
    
    xmlNodePtr data_node = xmlNewChild(parent_node, NULL, BAD_CAST "DataItem", BAD_CAST this->text.c_str());
    
    if(name.size())
      xmlNewProp(data_node, BAD_CAST "Name", BAD_CAST name.c_str());
    
    if(format_type != defaults::DATAITEM_FORMAT_TYPE)
      xmlNewProp(data_node, BAD_CAST "Format", BAD_CAST ToString(format_type));
    
      xmlNewProp(data_node, BAD_CAST "NumberType", BAD_CAST XidxDataType::ToString(number_type));
    if(strcmp(bit_precision.c_str(), defaults::DATAITEM_BIT_PRECISION) != 0 || format_type == FormatType::IDX_FORMAT)
      xmlNewProp(data_node, BAD_CAST "BitPrecision", BAD_CAST bit_precision.c_str());
    if(endian_type != defaults::DATAITEM_ENDIAN_TYPE)
      xmlNewProp(data_node, BAD_CAST "Endian", BAD_CAST Endianess::ToString(endian_type));

    if(dimensions.size())
      xmlNewProp(data_node, BAD_CAST "Dimensions", BAD_CAST xidx::ToString(dimensions).c_str());

    if(n_components != defaults::DATAITEM_N_COMPONENTS)
      xmlNewProp(data_node, BAD_CAST "ComponentNumber", BAD_CAST n_components.c_str());

    if(data_source != nullptr)
      xmlNodePtr ds_node = data_source->Serialize(data_node);
    
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
        xmlNewProp(variable_node, BAD_CAST "xpointer", BAD_CAST ("xpointer("+source->GetDataSourceXPath()+")").c_str());
      }
    }
#endif
    
    for(auto att: attributes){
      xmlNodePtr att_node = att.Serialize(data_node);
    }

    return data_node;
  };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    if(!xidx::IsNodeName(node,"DataItem"))
      return -1;

    SetParent(_parent);
    
    if(node->children != nullptr)
      text = (char*)(node->children->content);
    
    const char* name_s = xidx::GetProp(node, "Name");
    
    if(name_s != nullptr)
      name = name_s;
    
    if(this->GetParent()==nullptr)
      printf("%s has no parent\n", name.c_str());
    
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
      for(int t=XidxDataType::NumberType::CHAR_NUMBER_TYPE; t <= XidxDataType::NumberType::UINT_NUMBER_TYPE; t++)
        if (strcmp(num_type, XidxDataType::ToString(static_cast<XidxDataType::NumberType>(t)))==0){
          number_type = static_cast<XidxDataType::NumberType>(t);
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
      if(val_dimensions != NULL)
        dimensions = ToIndexVector(val_dimensions);
    //}

    const char* end_type = xidx::GetProp(node, "Endian");
    if (end_type != NULL){
      for(int t=Endianess::EndianType::LITTLE_ENDIANESS; t <= Endianess::NATIVE_ENDIANESS; t++)
        if (strcmp(end_type, Endianess::ToString(static_cast<Endianess::EndianType>(t)))==0){
          endian_type = static_cast<Endianess::EndianType>(t);
          break;
        }
    }
    else
      endian_type = defaults::DATAITEM_ENDIAN_TYPE;

    if(format_type != FormatType::XML_FORMAT){
      std::stringstream stream_data(text);

      values.clear();
      double n ;
      while( stream_data >> n ) values.push_back(n);

//      Parsable* parent_group = FindParent("Group", parent);
//
////      if(node != nullptr)
////        file_ref->Deserialize(node->children);
    }
    
    if(node->children != nullptr){
      for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
          if (cur_node->type == XML_ELEMENT_NODE) {
            if(IsNodeName(cur_node, "DataSource")){
              data_source = std::make_shared<DataSource>(new DataSource());
              data_source->Deserialize(cur_node, this);
              
            }
        }
      }
    }
    
    return 0;
  };
  
  const std::vector<double>& GetValues() const{ return values; }
  
  int AddValue(double v, int stride){
    values.push_back(v);
    dimensions.resize(stride);
    dimensions[0] = values.size()/stride;
    dimensions[1] = stride;
    return 0;
  }
  
  int AddValue(double v){
    values.push_back(v);
    dimensions.resize(1);
    dimensions[0] = values.size();
    return 0;
  }
  
  virtual std::string GetDataSourceXPath() override {
    Parsable* source=nullptr;
    Parsable* curr_parent=this->GetParent();
    
    while(curr_parent!=nullptr){
      const Parsable* parent_group = FindParent("Group", curr_parent);
      if(parent_group==nullptr)
        break;
      
      source = parent_group->FindChild("DataSource");
      
      if(source!=nullptr && source->GetClassName()=="DataSource")
        break;
      //printf("pass through %s\n", source->GetClassName().c_str());
      curr_parent = parent_group->GetParent();
    }
    
    if(source!=nullptr){
      return source->GetDataSourceXPath();
    }
    else return "";
    
  }
  
  virtual std::shared_ptr<DataSource> GetDataSource() {
    Parsable* source=nullptr;
    Parsable* curr_parent=this->GetParent();
    
    while(curr_parent!=nullptr){
      const Parsable* parent_group = FindParent("Group", curr_parent);
      if(parent_group==nullptr)
        break;
      
      source = parent_group->FindChild("DataSource");
      
      if(source!=nullptr && source->GetClassName()=="DataSource")
        break;
      //printf("pass through %s\n", source->GetClassName().c_str());
      curr_parent = parent_group->GetParent();
    }
    
    if(source!=nullptr){
      return std::make_shared<DataSource>((DataSource*)source);
    }
    else return nullptr;
    
  }
  
  virtual std::string GetClassName() const override { return "DataItem"; };
  
private:
  
  std::vector<double> values;
  
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
    
    for(int t=XidxDataType::NumberType::CHAR_NUMBER_TYPE; t <= XidxDataType::NumberType::UINT_NUMBER_TYPE; t++){
      std::string numType = XidxDataType::ToString(static_cast<XidxDataType::NumberType>(t));
      std::transform(numType.begin(), numType.end(), numType.begin(), ::tolower);
      
      if (strcmp(numType.c_str(), ntype.c_str())==0){
        number_type = static_cast<XidxDataType::NumberType>(t);
        break;
      }
    }
    
    return 0;
  }

  
};
  
}

#endif
