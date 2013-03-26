from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize(
           "ricker.pyx",                 # our Cython source
           sources=["ricker_cpp.cpp"],  # additional source file(s)
           language="c++",             # generate C++ code
      ))
