
exe = {
	'cython': 'cython --cplus'.split(),	  # raf: was just 'cython'
	'c++': 'g++',
	'c': 'gcc'
}

# CAPD include and lib directories are loaded unsing the capd-config script
dirs = {
	'base': '/home/raf/projects/rads/',
# used with Popen call along with `--cflags --libs`'
	'capd_config': '/home/raf/projects/capd/bin/capd-config ',
	'capd':  '/home/raf/projects/capd/'
}

include = {
	'sage': '/usr/local/share/sage-4.2-linux-ubuntu9.10-i686-Linux/devel/sage-main/',
	'sage c': '/usr/local/share/sage-4.2-linux-ubuntu9.10-i686-Linux/devel/sage-main/c_lib/include/',
	'python': '/usr/include/python2.7',
	'cython': '/usr/lib/pymodules/python2.7/Cython/Includes/',
	'numpy': '/usr/lib/python2.7/dist-packages/numpy/core/include/',
	'capd': '/home/raf/projects/capd/capdAlg/include/'
}


link = {
	'capd': dirs['capd'],
	'c++ cython': '-lpython2.7'.split(),
	'c++': '-lpython2.7'.split()
}

 
flags = {
	'c': '-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(),
	'c++ cython': '-pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions'.split(),
	'c++': '',
}


# From Jesse's
# flags = {
# 	'c': '-fno-strict-aliasing -fno-common -arch x86_64 -DNDEBUG -O1'.split(),
# 	 'c++ cython': '-arch x86_64 -bundle -undefined dynamic_lookup'.split()
#         }



