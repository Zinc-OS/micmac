#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

import setuptools
from distutils.core import setup, Extension

import sys
import glob

libs=[]
if "USEQT=ON" in sys.argv:
  print("With USEQT")
  libs=['GL', 'GLU', 'Qt5Core', 'Qt5Gui', 'Qt5Widgets', 'Qt5Concurrent', 'Qt5OpenGL', 'Qt5Xml']
else:
  print("Without USEQT")

#clear arguments for setup()
for arg in sys.argv[:]:
    if arg.startswith("USEQT"):
        sys.argv.remove(arg)

all_cpp_api_files =  glob.glob('api/*.cpp')

mmv2_module = Extension('_mmv2',
           define_macros = [('FORSWIG','')],
           sources = ['mmv2.i'] + all_cpp_api_files,
           swig_opts=['-python', '-py3',  '-DFORSWIG', '-Wall', '-c++', '-I.','-I../include/'],
           libraries = ['X11', 'Xext', 'm', 'dl', 'pthread', 'stdc++fs', 'gomp']+libs,
           library_dirs = [],
           include_dirs = ['/usr/local/include', '.', '../include/', '../ExternalInclude/'],
           language = 'c++',
           extra_objects = ['../bin/P2007.a', '../../lib/libelise.a', '../../lib/libANN.a'],
           extra_compile_args = ['-std=c++17', '-fopenmp']
       )

#https://docs.python.org/3.8/distutils/setupscript.html#installing-additional-files
xml_micmac_files = glob.glob('../../include/XML_MicMac/*.xml')
xml_gen_files = glob.glob('../../include/XML_GEN/*.xml')

setup (name = 'mmv2',
       version = '0.0.1',
       license = 'CeCILL-B',
       author_email = 'jean-michael.muller@ign.fr',
       url = 'https://github.com/micmacIGN',
       author      = "IGN",
       description = """MicMac Python API""",
       long_description = "MicMac v2 Python API",
       ext_modules = [mmv2_module],
       py_modules = ["mmv2"],
       data_files = [("mmv2/include/XML_MicMac", xml_micmac_files),
                     ("mmv2/include/XML_GEN", xml_gen_files)],
       platforms  = ['x86_64'],
       )

#https://docs.python.org/3/extending/building.html
#swig -python -py3 -DFORSWIG -c++ -I. -I../include/ mmv2.i
#python3 setup.py clean
#rm *.so
#python3 setup.py build_ext --inplace
#sudo python3 setup.py install
