#!/usr/bin/env python

from distutils.core import setup, Extension
from glob import glob

XidxPy_module = Extension('_XidxPy', 
                          sources=['xidx.i'],#,'../../examples/cpp/write.cpp'], 
                          include_dirs=['../','./elements','/usr/include/libxml2/'], 
                          libraries=['xml2'],
                          swig_opts=["-c++"],
                          extra_compile_args=['-std=c++11'], #,'-stdlib=libc++'],
                          language="c++") 

setup (name = 'XidxPy',
       version = '0.0',
       author      = "University of Utah",
       description = """Xidx Python module""",
       ext_modules = [XidxPy_module],
       py_modules = ["_XidxPy"],
       )
