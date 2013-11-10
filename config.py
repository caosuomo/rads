
exe = {
	'cython': 'cython --cplus'.split(), 
	'c++': 'g++',
	'c': 'gcc -std=c99'.split(),
	}

# CAPD include and lib directories are loaded using the capd-config script
dirs = {
	'base': '/ima/imausr1/jberwald/github/local/caja-matematica/rads/',
	#'capd_config': '/Users/jberwald/src/capd/bin/capd-config ', #--cflags --libs`'
	'capd_config': '/ima/imausr1/jberwald/local/capd/bin/capd-config ',
	'capd':  '/ima/imausr1/jberwald/local/capd/'
        }

include = {
	'python': '/ima/imausr1/jberwald/sage/local/include/python2.7/',
	'cython': '/ima/imausr1/jberwald/sage/local/lib/python2.7/site-packages/Cython/Includes/',
	'numpy': '/ima/imausr1/jberwald/sage/local/lib/python2.7/site-packages/numpy/core/include/',
	'capd': '/ima/imausr1/jberwald/local/capd/include/'
        }

link = {
	'capd': dirs['capd'],
	'c++ cython': '-L/ima/imausr1/jberwald/sage/local/lib -lpython2.7'.split(),
	'c++': '-L/ima/imausr1/jberwald/sage/local/lib -lpython2.7'.split()
	}
        

flags = {'c': '-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(),
         'c++ cython': '-O2 -frounding-math  -pthread -shared -Bsymbolic-functions'.split(),
    #'c': '-O2 -Wall -fPIC'.split(),
        # 'c++ cython': ''
         }
