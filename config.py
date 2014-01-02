from distutils.sysconfig import get_python_lib, get_python_inc
import numpy as np

exe = {
	'cython': 'cython --cplus'.split(), 
	'c++': 'g++',
	'c': 'gcc',
	}


dirs = {
	'base': '',
        }

include = {
	'python': '',
	'cython': '',
        'numpy' : '',
        'cxsc' : ''
        }

# change to override defaults below
link = {
	'c++ cython': '',
	'c++': ''
	}
        
flags = {
	'c': '-fno-strict-aliasing -fno-common -DNDEBUG -O1'.split(),
	 'c++ cython': '-undefined dynamic_lookup'.split()
        }

def make_links():    
    # populate with defaults unless altered above
    if link['c++ cython'] == '':        
        link['c++ cython'] = '-L'+ get_python_lib() + '-l python2.7'#.split()
        link['c++ cython'] = link['c++ cython'].split()
    if link['c++'] == '':
        link['c++'] = link['c++ cython']

def make_includes():
    if include['python'] == '':
        include['python'] = get_python_inc()
    if include['cython'] == '':
        include['cython'] = get_python_inc() + '/Cython/Includes/'
    if include['numpy'] == '':
        include['numpy'] = np.get_include()

def fiddle_with_flags():
    """
    Mostly to account for some jacked up s*!t on Mac's
    """
    flags['c++'] += '-arch x86_64 -bundle'
    flags['c'] += '-arch x86_64'
