
exe = {
	'cython': 'cython --cplus'.split(), 
	'c++': 'g++',
	'c': 'gcc -std=c99'.split(),
	}

dirs = {
	'base': '/home/rafael/rads/',
	'cxsc': '/home/rafael/rads/cxsc/'
        }

include = {
	'python': '/usr/include/python2.7/',
	'cython': '/usr/lib/python2.7/site-packages/Cython/Includes/',
	'numpy': '/usr/lib/python2.7/site-packages/numpy/core/include/',
	'cxsc': dirs['cxsc']+'include/'
        }

link = {
	'cxsc': ['-L'+dirs['cxsc']+'lib/','-Wl,-R'+dirs['cxsc']+'lib/','-lcxsc'],
	'c++ cython': '-lpython2.7',
	'c++': '-lpython2.7'
	}
        

flags = {'c': '-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(),
         'c++ cython': '-O2 -frounding-math  -pthread -shared -Bsymbolic-functions'.split(),
         }
