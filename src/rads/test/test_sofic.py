#!/usr/bin/python

import sys
import numpy as np
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor

if len(sys.argv) < 3:
    print "usage: python test_sofic.py <data file> [<index_map> <generators>]"
    print "where <index_map> and <generators> default to M and G"
    genn = 'G'
    mapn = 'M'
else:
    genn = sys.argv[2]
    mapn = sys.argv[3]
filename = sys.argv[1]

gens = utils.convert_matlab_gens( filename, genname=genn )
index_map = utils.load_matlab_matrix( filename, matname=mapn ).astype(int)
sof = SoficProcessor( index_map, gens, debug=False )
sof.process()
print sof
print sof.entropy()


# for i in range(ca.shape[0]):
#     gens = utils.cell2dict(ca[i,0])
#     sof = SoficProcessor(ca[i,1],gens)
#     sof.process()
#     print sof
#     print sof.entropy()

# ca = utils.load_matlab_matrix( '/home/raf/Dropbox/henon-tipping-14-run-goodruns.mat', matname='e' )
# ents = []
# for i in range(ca.shape[0]):
#     sof = SoficProcessor(ca[i,1],utils.cell2dict(ca[i,0]))
#     sof.process()
#   ents.append(sof.entropy())
#     print sof
#     print ents[-1]
