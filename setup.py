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

# rads root directory
ROOT = os.getcwd()
lib_dir = 'build/lib/'

# only build symbolics portion
if '--sym' in sys.argv:
    full = False
    # the '--sym' confuses setup, so get rid of it now
    sys.argv.pop( -1 )
    print sys.argv
else:
    full = True

if full:
    # compile cython libs (req == cython & g++)
    print "Compiling combinatorial enclosure library...\n"
    check_rads = os.system( './make_rads' )
    if check_rads != 0:
        print "Enclosure library failed to buld."

# build chomp/homcubes libraries (req == g++)
os.chdir( ROOT+'/src/rads/chomp' )
val = os.system( 'make' )
if val != 0:
    print "Chomp/Homcubes build failed! See output for clues."
                
# move back to root directory
os.chdir( ROOT )

# install everything into build folder
setup( name='rads',
       version='0.1',
       description='Rigorous Analysis of Dynamical Systems',
       author='Rafael Frongillo, Jesse Berwald, and Sarah Day',
       author_email='jberwald@gmail.com',
       license='BSD',
       package_dir={'' : 'src' },
       packages=[#'rads', 
                 'rads.symbolics',
                 'rads.misc', 
                 'rads.graphs',
                 'rads.test',
                 'rads.enclosure',
                 'rads.maps',
                 'rads.homology'],
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

# setup does not want to copy binary files from chomp/bin directory,
# so we'll do it manually
copydirs('src/rads/chomp/bin', lib_dir+'rads/bin', ext='')	

