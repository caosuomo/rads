#!/usr/bin/python
######################################################################
#
# makeit -- build RADS
#
# Right now, the directory structure is relatively flat:
# 
# src/
# src/rads -- all cython and python files
# src/rads/maps -- where the maps go
# src/cpp -- all c++ files
#
######################################################################
from fabricate import *
from subprocess import Popen, PIPE, STDOUT
import sys, os, stat, commands
import config
from shutil import copy2,copytree, ignore_patterns
### EXAMPLE: copytree(source, destination, ignore=ignore_patterns('*.pyc', 'tmp*'))
# We might have to find the OS for the proper extension. There is no
# Windows option.
extension = '.so'

dependencies = { # dependencies for cython (or c++!) files
	'cyboxset': ['box','boxset'],
	'cytree': ['tree','box','boxset','treedata'],
	'cyutils': ['box'],
	'cymapper': ['mapper','box','boxset'],
	'cycombenc': ['tree','treedata','mapper','box','boxset'],
	'treetest': ['tree','treedata','box','boxset'],
	'debugtree': ['tree','treedata','box','boxset'],
}

cpp_progs = ['treetest','debugtree'] # c++ programs

tmp_dir = 'build/tmp/'
lib_dir = 'build/lib/'
map_dir = 'src/rads/maps'
cpp_source_dir = 'src/cpp/' # look for c++ files here (in the current dir)
cpp_object_dir = tmp_dir+'cpp/' # compile them to object files here

				
def list_dirs(path):
	"""Returns a list of all top-level subdirectories in directory 'path'"""
	return [ name for name in os.listdir(path)
		 if os.path.isdir(os.path.join(path, name)) ]

def copydirs(src, dst, ext=''):
	"""
	Recursively copies all files with extension 'ext' from src to dst,
	creating the directories in dst as necessary.
	"""
	names = os.listdir(src)
	try:
		os.makedirs(dst)
	except OSError:
		pass
	errors = []
	for name in names:
		srcname = os.path.join(src, name)
		dstname = os.path.join(dst, name)
		try:
			if os.path.isdir(srcname):	# directory: recurse
				copydirs(srcname, dstname, ext)
			elif name.endswith(ext):	# file.ext: copy
				print srcname, '-->', dstname
				copy2(srcname, dstname)
		except (IOError, os.error) as why:
			errors.append((srcname, dstname, str(why)))
		# catch the Error from the recursive copydirs so that we can
		# continue with other files
		except Exception as err:
			errors.extend(err.args[0])
	if errors:
		raise Exception(errors)	

# top-level directories in rads
rads_dirs = ['src/rads/'+d for d in list_dirs('src/rads')+['','../cpp/']]
# all include directories!
# removed config.include['capd'] from list, since this should be handled by capd-config
includes = [config.include['cython']+'libc/',
	    config.include['cython']+'libcpp/',
	    config.include['numpy'],
	    config.include['python']] + rads_dirs
	    #	    config.include['capd']] + rads_dirs

# grab CAPD cflags and lib paths
# p_flags = Popen( config.dirs['capd']+'/bin/capd-config --cflags',
# 		 shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
# p_libs = Popen( config.dirs['capd']+'/bin/capd-config --libs',
# 		shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)

# capd_flags = p_flags.stdout.read().split()
# capd_libs = p_libs.stdout.read().split()

# print "capd_flags"
# print capd_flags
# print ""
# print "capd_libs"
# print capd_libs
# print ""

# # now split the output, cython does not like D__USE_FILIB flag
# capd_flags = [ i for i in capd_flags ]
# #capd_links = capd_libs[-1]
# capd_libs = capd_libs #[ i for i in capd_libs ]#.append( capd_libs[:-2][:] )

def compile_cpp(inputs,output):
	run(config.exe['c'],
	    capd_flags,	   
	    config.flags['c'],
	    ['-I'+i for i in includes],
	    '-w',
	    '-c', inputs,'-o', output )

def compile_cython(inputs,output):
	run(config.exe['cython'], ['-I'+i for i in includes],
		'-a', '--cplus', inputs, '-o', output)
	
def link_cython(inputs,output):
	run(config.exe['c++'],
	    config.flags['c++ cython'],
	    '-o', output,
	    ['-g'],
	    inputs,
	    capd_libs,
	    config.link['c++ cython'])#+ [capd_links] )
		
def link_cpp(inputs,output):
	"""
	When using CAPD, we remove the linker flags, since the CAPD
	capd-config script does this automagically(?).
	"""
	run(config.exe['c++'], #config.flags['c++'], # cannot be empty (!?)
			       #config.link['c++ cython'],
	    config.link['c++'],
	    '-o', output,
	    ['-g'],
	    capd_flags,
	    capd_libs,
	    config.flags['c++ cython'],
            inputs)
		

def scandir(dir, files=[], ext='.py'):
	"""
	Return all paths (dir/dir2/dir3/file) of files in 'dir' (including
	subdirectories) with extension 'ext'
	"""
	for file in os.listdir(dir):
		path = os.path.join(dir, file)
		if os.path.isfile(path) and path.endswith( ext ):
			# print path
			# files.append(path.replace(os.path.sep, '.')[:-len(ext)-1])
			files.append(path[:-len(ext)]) # kill extension
		elif os.path.isdir(path):
			scandir(path, files, ext)
	return files

def build_cpp_deps(f):
	"""
	Builds each dependency of f (not including f).	Returns the
	list of destinations so that the linker can find where these ended
	up.
	"""
	name = f.split('/')[-1]				# name.cpp (or name.pyx)
	if name not in dependencies:
		return []							# nothing to do here...

	for dep in dependencies[name]:
		compile_cpp(cpp_source_dir+dep+'.cpp',cpp_object_dir+dep+'.o')
	objects = [cpp_object_dir + d for d in [f]+dependencies[name]]
	return objects # the destinations, include f here


def build_cyfile(sourcepath):
	"""
	NOTE: using a flat namespace for Cython for right now to easy the dependencies...
	1. Compile (Cython)
		 src/rads/...../file.pyx --> <cpp_source_dir>/file_from_cython.cpp
		 
	2. Compile (C++)
		 <cpp_source_dir>/file_from_cython.cpp --> <cpp_object_dir>/file.o
		 
	3. Look up dependencies[file] and compile each of those
		 <cpp_object_dir>/dep.o
		 
	4. Finally, link all of the above
		 <lib_dir>/rads/...../file.so
	"""
	filename = sourcepath.split(os.path.sep)[-1]			   # filename.pyx
	tempfile = tmp_dir+filename+'_from_cython.cpp' # the temp cpp file name
	finalpath = lib_dir+sourcepath.replace('src/','') # the final resting place = rads/../..
	# compile to temp dir, but flat (notice tempfile instead of sourcepath)
	compile_cython(sourcepath+'.pyx', tempfile)
	compile_cpp(tempfile, cpp_object_dir+filename+'.o')
	deps = build_cpp_deps(filename) # c++ dependencies, including self
	objects = [d+'.o' for d in deps]
	link_cython(objects, finalpath + extension )


def build_map(sourcepath):
	"""
	NOTE: using a flat namespace for Cython for right now to easy the dependencies...
	1. Compile (Cython, C++)
		 src/rads/maps/map.pyx --> <cpp_source_dir>/map_cython_mapfile.cpp
		 <cpp_source_dir>/map_cython_mapfile.cpp --> <cpp_object_dir>/map_cython_mapfile.o

	2. Compile (C++)
		 src/rads/maps/map_cpp.cpp --> <cpp_object_dir>/map_cpp_mapfile.o

	3. Finally, link all of the above
		 <lib_dir>/rads/maps/map.so
	"""
	filename = sourcepath.split(os.path.sep)[-1]			   # filename.pyx
	finalpath = lib_dir+sourcepath.replace('src/','') # the final resting place = rads/../..
	cython_cpp = tmp_dir+filename+'_cython_mapfile.cpp'
	cython_o = cpp_object_dir+filename+'_cython_mapfile.o'
	cpp_o = cpp_object_dir+filename+'_cpp_mapfile.o'
	compile_cython(sourcepath+'.pyx', cython_cpp)
	compile_cpp(cython_cpp, cython_o)
	compile_cpp(sourcepath+'_cpp.cpp', cpp_o)

	link_cython([cython_o, cpp_o], finalpath + extension )


def build_cpp(name):
	"""Build a c++ file."""
	compile_cpp(cpp_source_dir+name+'.cpp', cpp_object_dir+name+'.o')
	deps = build_cpp_deps(name)
	link_cpp([d+'.o' for d in deps],lib_dir+name)


def build():
	# copy python files over, preserving directory structure
	copydirs('src/rads/', lib_dir+'rads/', ext='.py')	
	# os.chdir('src' )
	# run('python ../buildpy.py build --build-base=../build'.split())
	# os.chdir('..')

	# make temp directories for objects, etc
	try:
		os.makedirs(cpp_object_dir)
		os.makedirs(tmp_dir+'src/')
	except OSError:
		pass

	# all cython files to compile
	files = scandir('src/rads',ext='.pyx')
	for path in files:					# for each cython file
		if path.startswith(map_dir):	# this is a user-defined map
			build_map(path)				# treat maps differently
		else:
			build_cyfile(path)			# normal cython file

	for name in cpp_progs:				# c++ programs...
		build_cpp(name)

def test():
	build()
	os.chdir(lib_dir)
	run('python', 'rads/test/test_graphs.py')
	#run('ipython', '-pylab', 'rads/test/test_tree.py')
	run('ipython', '-pylab', 'rads/test/test_henon.py')
	
def clean():
	run('rm -r build'.split())
	autoclean()

if __name__ == "__main__":

        main()
