# build script for 'dvedit' - Python libdv wrapper

# change this as needed
include_paths = ['/home/raf/projects/rads/Profil-2.0.8/lib/',
				 '/home/raf/projects/rads/Profil-2.0.8/include/',
				 '/usr/local/lib/python2.6/dist-packages/Cython/Includes/libcpp/',
				 '/home/raf/projects/rads/src/cpp/']

import sys, os, stat, commands
from distutils.core import setup
from distutils.extension import Extension

# we'd better have Cython installed, or it's a no-go
try:
    from Cython.Distutils import build_ext
except:
    print "You don't seem to have Cython installed. Please get a"
    print "copy from www.cython.org and install it"
    sys.exit(1)


# scan the source directory for extension files, converting
# them to extension names in dotted notation
def scandir(dir, files=[]):
    for file in os.listdir(dir):
        path = os.path.join(dir, file)
        if os.path.isfile(path) and path.endswith(".pyx"):
            files.append(path.replace(os.path.sep, ".")[:-4])
        elif os.path.isdir(path):
            scandir(path, files)
    return files


# generate an Extension object from its dotted name
def makeExtension(extName):
    extPath = extName.replace('.', os.path.sep)+'.pyx'
    return Extension(
        extName,
        [extPath],
		language='c++',
        include_dirs = include_paths+['.'],   # adding the '.' to include_dirs is CRUCIAL!!
        extra_compile_args = ['-O3'],
        extra_link_args = ['-g'],
        libraries = ['Profil', 'lr', 'Bias'],
		cmdclass = {'build_ext': build_ext}
        )

# get the list of extensions
extNames = scandir("cython")

# and build up the set of Extension objects
extensions = [makeExtension(name) for name in extNames]

# finally, we can pass all this to distutils
setup(
  name="cython",
  packages=["cython"],
  ext_modules=extensions,
  cmdclass = {'build_ext': build_ext},
)
