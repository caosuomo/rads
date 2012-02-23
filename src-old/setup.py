# build script for 'dvedit' - Python libdv wrapper

# change this as needed
include_paths = ['/home/raf/projects/rads/Profil-2.0.8/include/',
				 '/usr/local/lib/python2.6/dist-packages/Cython/Includes/libc/',
				 '/usr/local/lib/python2.6/dist-packages/Cython/Includes/libcpp/',
				 '/home/raf/projects/rads/src/rads/maps/',
				 '/home/raf/projects/rads/src/rads/',
				 '/home/raf/projects/rads/src/cpp/']
cydeps = {
	'cyboxset': ['box','boxset'],
	'cytree': ['tree','box','boxset','treedata'],
	'cyutils': ['box'],
	'cymapper': ['mapper','box','boxset'],
	'cycombenc': ['tree','treedata','mapper','box','boxset']
#	'point': [],
#	'box': ['box'],
#	'tree': ['tree','box','treedata']
	}

def cpp_deps(path):
	parts = path.split('.')
	name = parts[-1]
	if 'maps' in parts:
		return ['rads/maps/'+name+'_cpp.cpp']
	return ['cpp/'+s+'.cpp' for s in cydeps[name]]

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
        name=extName,
        sources=[extPath]+cpp_deps(extName),
		language='c++',
        include_dirs = include_paths+['.'],   # adding the '.' to include_dirs is CRUCIAL!!
        extra_compile_args = ['-O3','-w'],
        extra_link_args = ['-g','-L/home/raf/projects/rads/Profil-2.0.8/lib/'],
        libraries = ['Profil', 'lr', 'Bias'],
        )

# get the list of extensions
extNames = scandir("rads")

# and build up the set of Extension objects
extensions = [makeExtension(name) for name in extNames]

# finally, we can pass all this to distutils
setup(
  name="rads",
  packages=['rads','rads.maps'],
  ext_modules=extensions,
  cmdclass = {'build_ext': build_ext},
)
