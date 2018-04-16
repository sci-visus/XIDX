#!/usr/bin/env python

from distutils.core import setup, Extension
from glob import glob

XidxPy_module = Extension('_XidxPy', 
                          sources=['xidx.i'],#,'../../examples/cpp/write.cpp'], 
                          include_dirs=['../','./elements','/usr/include/libxml2/'], 
                          libraries=['xml2'],
                          swig_opts=["-c++"],
                          extra_compile_args=['-std=c++11','-stdlib=libc++'],
                          language="c++")
                          # sources=['elements/xidx_list.h', 'elements/xidx_attribute.h','elements/xidx_dataitem.h','elements/xidx_domain.h','elements/xidx_spatial_domain.h','elements/xidx_hyperslab_domain.h','elements/xidx_list_domain.h','elements/xidx_multiaxis_domain.h','elements/xidx_types.h','elements/xidx_geometry.h','elements/xidx_topology.h','elements/xidx_group.h','elements/xidx_variable.h','elements/xidx_attribute.h','elements/xidx_parsable.h','elements/xidx_parse_utils.h'])
                           

setup (name = 'XidxPy',
       version = '0.0',
       author      = "University of Utah",
       description = """Xidx Python module""",
       ext_modules = [XidxPy_module],
       py_modules = ["_XidxPy"],
       )
