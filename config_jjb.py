
exe = {
	'cython': 'cython --cplus'.split(), 
	'c++': 'g++',
	'c': 'gcc',
       }

# CAPD include and lib directories are loaded unsing the capd-config script
dirs = {
	'base': '/Users/jberwald/github/local/caja-matematica/rads/',
	'capd_config': '/Users/jberwald/src/capd/bin/capd-config ', # used with Popen call along with `--cflags --libs`'
	'capd':  '/Users/jberwald/src/capd/'
        }

include = {
	'sage': '/Applications/sage/devel/sage-main/',
	'sage c': '/Applications/sage/devel/sage-main/c_lib/include/',
	'python': '/Library/Frameworks/EPD64.framework/Versions/Current/include/python2.7/',
	'cython': '/Library/Frameworks/EPD64.framework/Versions/Current/lib/python2.7/site-packages/Cython/Includes/',
	'numpy': '/Library/Frameworks/EPD64.framework/Versions/Current/lib/python2.7/site-packages/numpy/core/include/',
	'capd': '/Users/jberwald/src/capd/capdAlg/include/'
        }

link = {
	'capd': dirs['capd'],
	'c++ cython': '-L/Library/Frameworks/EPD64.framework/Versions/Current/lib -lpython2.7'.split(),
	'c++': '-L/Library/Frameworks/EPD64.framework/Versions/Current/lib -lpython2.7'.split()
	}
        

flags = {
	'c': '-fno-strict-aliasing -fno-common -arch x86_64 -DNDEBUG -O1'.split(),
	 'c++ cython': '-arch x86_64 -bundle -undefined dynamic_lookup'.split()
        }
