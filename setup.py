from distutils.core import setup
import sys
import os
from shutil import copy2


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

def run_make_rads():
    """
    """
    # locate directories


###############################
# BEGIN BUILD SYSTEM
###############################
# rads root directory
ROOT = os.getcwd()
lib_dir = ROOT + '/build/lib/'

# reserve 'install' for just installing.  python setup.py build will
# trigger this option (this sequence denoted in README)
if 'install' in sys.argv:
    build_full = False
else:
    build_full = True
    # look for CXSC path in setup args
    for i,arg in enumerate(sys.argv):
        if 'cxsc' in arg:
            c = sys.argv[i]
            # expect either '--cxsc=/path/to/lib'
            if '=' in c:
                cs = c.split( '=' )
                cxsc_path = cs[-1]
                sys.argv.pop(i) # remove so setup doesn't get confused
            # ... or '--cxsc /path/to/lib'
            else:
                cxsc_path = sys.argv[i+1]
                sys.argv.pop(i)
                sys.argv.pop(i+1)
            break
    else:
        print sys.argv
        print "Must provide path to CXSC headers using '--cxsc' arg. Aborting..."
        sys.exit(1)
# build the CHomP library
if 'chomp' in sys.argv:
    build_chomp = True
else:
    build_chomp = False


if build_full:
    # compile cython libs (reqs == cython & g++)
    print "Compiling combinatorial enclosure library...\n"
    # call make_rads.py and check that it returned without any obvious
    # errors
    import make_rads_cx as M
    M.run_main( cxsc_path )
 #check_rads = os.system( ROOT + '/make_rads_cx.py ' + cxsc_path )
    if check_rads != 0:
        print check_rads
        print "Enclosure library failed to build."

sys.exit(1)

if build_chomp:
    print "Compiling CHomP library...\n"
    # build chomp/homcubes libraries (req == g++)
    os.chdir( ROOT+'/src/rads/chomp' )
    val = os.system( 'make' )
    if val != 0:
        print "Chomp/Homcubes build failed! See output for clues."
            

# move back to root directory
os.chdir( ROOT )
   
# setup does not want to copy binary files from chomp/bin directory,
# so we'll do it manually
copydirs('src/rads/chomp/bin', lib_dir+'rads/bin', ext='')
 
# We can get fancier later
packages = [#'rads', 
    'rads.symbolics',
    'rads.misc', 
    'rads.graphs',
    'rads.test',
    'rads.enclosure',
    'rads.maps',
    'rads.homology'],

# install everything into build folder
setup( name='rads',
       version='0.1',
       description='Rigorous Analysis of Dynamical Systems',
       author='Rafael Frongillo, Jesse Berwald, and Sarah Day',
       author_email='jberwald@gmail.com',
       license='BSD',
       package_dir={'' : 'src' },
       packages=packages,
       classifiers=['Intended Audience :: Science/Research',
                    'License :: OSI Approved :: BSD License',
                    'Natural Language :: English',
                    'Operating System :: MacOS :: MacOS X',
                    'Operating System :: POSIX :: Linux',
                    'Programming Language :: Python',
                    'Programming Language :: Cython',
                    'Programming Language :: C++',
                    'Topic :: Scientific/Engineering',
                    'Topic :: Scientific/Engineering :: Mathematics',
                    'Topic :: Software Development :: Libraries :: Python Modules']
       )

	

