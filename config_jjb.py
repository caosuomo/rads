
exe = {
	'cython': '/Applications/sage/local/bin/sage-cython', #cython
	'c++': 'g++',
	'c': 'gcc',
	}

# CAPD include and lib directories are loaded unsing the capd-config script
dirs = {
	'base': '/Users/jberwald/Dropbox/Projects/entropy/rads/',
	'capd_config': '/Users/jberwald/src/capd/bin/capd-config ', #--cflags --libs`'
	'capd':  '/Users/jberwald/src/capd/'
        }

include = {
	'sage': '/Applications/sage/devel/sage-main/',
	'sage c': '/Applications/sage/devel/sage-main/c_lib/include/',
	'python': '/Library/Frameworks/EPD64.framework/Versions/7.3/include/python2.7/',
	'cython': '/Library/Frameworks/EPD64.framework/Versions/7.3/lib/python2.7/site-packages/Cython/Includes/',
	'numpy': '/Library/Frameworks/EPD64.framework/Versions/7.3/lib/python2.7/site-packages/numpy/core/include/',
	'capd': '/Users/jberwald/src/capd/capdAlg/include/'
        }

link = {
	'capd': dirs['capd'],
	'c++ cython': '',#.split(), #'-llr'
	'c++': '-lprim'.split() #-llr
        }

flags = {
	'c': '-fno-strict-aliasing -fno-common -dynamic -arch x86_64 -DNDEBUG -g -O3'.split(),
	# 'c': '-arch x86_64 -pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(), # -O2
	'c++ cython': '-arch x86_64 -shared -fPIC,-dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0'.split(),
	#'-pthread -shared -Wl,-Wl,-Bsymbolic-functions'.split(),
	'c++': ''#'-D__USE_FILIB__ -lprim'.split()
        }

# gcc -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/local/lib/libfoo.1.dylib -o libfoo.1.dylib $(OBJ)
