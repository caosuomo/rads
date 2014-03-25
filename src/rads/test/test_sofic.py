#!/usr/bin/python

import sys
import numpy as np
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor

if len(sys.argv) < 3:
	print "usage: python test_sofic.py <generators>.mat <index map>.mat"
	print "defaulting to 'generators.mat' and 'index_map.mat'"
	genfile = 'generators.mat'
	mapfile = 'index_map.mat'
else:
	genfile = sys.argv[1]
	mapfile = sys.argv[2]

gens = utils.convert_matlab_gens( genfile, genname='G' )
index_map = utils.load_matlab_matrix( mapfile, matname='M' ).astype(int)
sof = SoficProcessor( index_map, gens, debug=False, max_iter=1000 )
sof.process()
print sof.mgraph.edges()
print sof.entropy()
