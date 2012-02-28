from distutils.core import setup
import os

print "in buildpy.py!!"

def list_dirs(path):
	"""Returns a list of all top-level subdirectories in directory 'path'"""
	return [ name for name in os.listdir(path) if os.path.isdir(os.path.join(path, name)) ]

rads_pkg = ['rads.'+folder for folder in list_dirs('rads')]

print rads_pkg

setup(
  name="rads",
  packages=['rads']+rads_pkg,
)

