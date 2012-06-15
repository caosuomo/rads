
exe = {
	'cython': 'cython',
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
	'capd_int': dirs['capd'] + 'capdAlg/include/',
	'python': '/Library/Frameworks/Python.framework/Versions/Current/include/python2.7',
	'cython': '/Library/Frameworks/Python.framework/Versions/Current/lib/python2.7/site-packages/Cython/Includes/',
	'numpy': '/Library/Frameworks/Python.framework/Versions/Current/lib/python2.7/site-packages/numpy/core/include/',        
        }

capd = {
	'libs': '`' + dirs['capd'] + '--libs`',
	'flags': '`' + dirs['capd'] + '--clags`'
	}

link = {
	'capd': dirs['capd'],
	'c++ cython':'-llr',
	'c++': dirs['capd']
        }

flags = {
	'c': '-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(),
	'c++ cython': '-pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions'.split(),
	'c++': '-D__USE_FILIB__ -frounding-math -lcapd'
        }
