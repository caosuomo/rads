#!/usr/bin/python

import numpy as np
from rads.misc import utils
#from rads.symbolics import sofic_processor


# import sys
# print 'Number of arguments:', len(sys.argv), 'arguments.'
# print 'Argument List:', str(sys.argv)

gens = utils.convert_matlab_gens( 'generators.mat', genname='G' )
print
print
print
print gens
import scipy.io as spio
cell_array = spio.loadmat( 'generators.mat' )#, squeeze_me=True )
print cell_array
gens = cell_array['G'].flatten().tolist()
print gens
index_map = utils.load_matlab_matrix( 'index_map.mat', matname='M' )
print index_map
#sof = SoficProcessor( index_map, gens, debug=True, max_iter=100 )
#sof.process()
