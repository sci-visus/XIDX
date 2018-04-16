

%module XidxPy
%{ 
#define SWIG_FILE_WITH_INIT
#include "xidx.h"
using namespace xidx;
%}

// Nested class not currently supported
#pragma SWIG nowarn=325

// The 'using' keyword in template aliasing is not fully supported yet.
#pragma SWIG nowarn=342

// Nested struct not currently supported
#pragma SWIG nowarn=312

// warning : Nothing known about base class 'PositionRValue'
#pragma SWIG nowarn=401

//%include <defines.i>
%include <std_shared_ptr.i>
%include <stl.i>
%include pointer.i
%include <std_pair.i>
%include <std_vector.i>
%include <std_deque.i>
%include <std_string.i>
%include <std_map.i>
%include <std_set.i>

//Shared Pointers
%shared_ptr(xidx::Parsable)
%shared_ptr(xidx::Group)
%shared_ptr(xidx::DataSource)
%shared_ptr(xidx::XidxFile)
%shared_ptr(xidx::Domain)
%shared_ptr(xidx::SpatialDomain)
%shared_ptr(xidx::MultiAxisDomain)
%shared_ptr(xidx::HyperSlabDomain)
//%shared_ptr(xidx::ListDomain)
//%shared_ptr(xidx::IndexSpace)
%shared_ptr(xidx::Attribute)
%shared_ptr(xidx::Variable)
%shared_ptr(xidx::ListDomain<double>)
%shared_ptr(ListDomainDouble)

%include <xidx.h>
%include <xidx_file.h>
%include <xidx_data_source.h>
%include <xidx_index_space.h>
%include <elements/xidx_attribute.h>
%include <elements/xidx_dataitem.h>
%include <elements/xidx_domain.h>
%include <elements/xidx_spatial_domain.h>
%include <elements/xidx_hyperslab_domain.h>
%include <elements/xidx_list_domain.h>
%include <elements/xidx_multiaxis_domain.h>

%typemap(out) std::shared_ptr<Domain> xidx::Group::GetDomain() {
  std::string lookup_typename = result->GetClassName();
  if(lookup_typename=="ListDomain")
    //lookup_typename = "_p_std__shared_ptrT_xidx__ListDomainT_double_t_t";
    lookup_typename = "_p_xidx__ListDomainT_double_t";
    //$result = std::dynamic_pointer_cast<xidx::ListDomain<double>>($1);
  
  printf("changing type to %s\n", lookup_typename.c_str());
  swig_type_info * const outtype = SWIG_TypeQuery(lookup_typename.c_str());
  if(outtype)
    printf("found type %s\n", outtype->name);
  else
    printf("ERROR: null out type\n");
  $result = SWIG_NewPointerObj(SWIG_as_voidptr($1.get()), outtype, $owner);
}

%include <elements/xidx_types.h>
%include <elements/xidx_geometry.h>
%include <elements/xidx_topology.h>
%include <elements/xidx_group.h>
%include <elements/xidx_variable.h>
%include <elements/xidx_attribute.h>
%include <elements/xidx_parsable.h>
%include <elements/xidx_parse_utils.h>

%template(ListDomainDouble) xidx::ListDomain<double>;
%template(IndexSpace) std::vector<double>;
%template(GroupVector) std::vector<std::shared_ptr<xidx::Group>>;
%template(AttributeVector) std::vector<xidx::Attribute>;



%include "xidx.h"
