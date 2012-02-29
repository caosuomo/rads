
exe = {
	'cython': 'cython',
	'c++': 'g++',
	'c': 'gcc',
}

dirs = {
	'base': '/home/raf/projects/rads/',
	'profil': '/home/raf/projects/rads/Profil-2.0.8/',
}

include = {
	'sage': '/usr/local/share/sage-4.2-linux-ubuntu9.10-i686-Linux/devel/sage-main/',
	'sage c': '/usr/local/share/sage-4.2-linux-ubuntu9.10-i686-Linux/devel/sage-main/c_lib/include/',
	'profil': dirs['profil']+'include/',
	'python': '/usr/include/python2.7',
	'cython': '/usr/local/lib/python2.7/dist-packages/Cython/Includes/',
	'numpy': '/usr/local/lib/python2.7/dist-packages/numpy/core/include/',
}

link = {
	'profil': dirs['profil']+'lib/',
	'c++ cython': '-lProfil -llr -lBias -o'.split(),
	'c++': '-lProfil -llr -lBias -o'.split(),
}

flags = {
	'c': '-pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -fPIC'.split(),
	'c++ cython': '-pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions'.split(),
	'c++': '',
}
