#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor


dat = utils.load_matlab_matrix( '/home/raf/projects/std/std-eps2-p2-runs/std-eps2-p2-plus34-run-dat.mat', matname='dat')

# the [0,0][0,1] is to get the second entry of the cell, i.e. dat.M_inv{2}
M = dat['M_inv'][0,0][0,1]
G = utils.cell2dict(dat['G_inv'][0,0][0,1][0])
sof = SoficProcessor(M,G)
sof.process()
print sof
print 'entropy:', sof.entropy()
# SoficProcessor on 41 symbols, with 68 states and 127 transitions
# entropy: 0.571595590383
# up from STD: Theorem 4.4. The topological entropy for the standard map f ε for ε = 2 is bounded below by 0.54518888942276.
