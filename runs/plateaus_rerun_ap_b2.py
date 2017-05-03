#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor


datcell = utils.load_matlab_matrix('/home/raf/Dropbox/projects/plateaus/focm-data/results-9-2-2007/results_apboxprm_b2_d22.mat', matname='D')

ents = np.zeros(datcell.shape)

for i in range(datcell.shape[0]):
    for j in range(datcell.shape[1]):
        M = datcell[i,j][0,2]
        G = utils.cell2dict(datcell[i,j][0,1][0])
        sof = SoficProcessor(M,G)
        sof.process()
        print sof
        ents[i,j] = sof.entropy()
        print 'entropy:', ents[i,j]

np.savetxt('plateaus_ap_b2_22.txt',ents)

# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# /home/raf/.local/lib/python2.7/site-packages/rads/symbolics/sofic_processor.py:29: RuntimeWarning: divide by zero encountered in log
#   return np.log( np.absolute( v ).max() )
# entropy: -inf
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# entropy: -inf
# SoficProcessor on 5 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 16 symbols, with 26 states and 56 transitions
# entropy: 0.637160267433
# SoficProcessor on 14 symbols, with 27 states and 56 transitions
# entropy: 0.637160267433
# SoficProcessor on 13 symbols, with 23 states and 48 transitions
# entropy: 0.637160267433
# SoficProcessor on 11 symbols, with 18 states and 38 transitions
# entropy: 0.589844146079
# SoficProcessor on 11 symbols, with 18 states and 37 transitions
# entropy: 0.589844146079
# SoficProcessor on 9 symbols, with 15 states and 31 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 39 symbols, with 55 states and 103 transitions
# entropy: 0.644687552555
# SoficProcessor on 31 symbols, with 48 states and 95 transitions
# entropy: 0.644687552555
# SoficProcessor on 30 symbols, with 39 states and 78 transitions
# entropy: 0.644687552555
# SoficProcessor on 26 symbols, with 34 states and 65 transitions
# entropy: 0.644687552555
# SoficProcessor on 21 symbols, with 33 states and 70 transitions
# entropy: 0.637160267433
# SoficProcessor on 23 symbols, with 34 states and 70 transitions
# entropy: 0.637160267433
# SoficProcessor on 19 symbols, with 31 states and 66 transitions
# entropy: 0.637160267433
# SoficProcessor on 18 symbols, with 29 states and 61 transitions
# entropy: 0.637160267433
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# entropy: -inf
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# entropy: -inf
# SoficProcessor on 6 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 15 symbols, with 25 states and 53 transitions
# entropy: 0.637160267433
# SoficProcessor on 15 symbols, with 29 states and 61 transitions
# entropy: 0.637160267433
# SoficProcessor on 12 symbols, with 22 states and 48 transitions
# entropy: 0.637160267433
# SoficProcessor on 12 symbols, with 20 states and 43 transitions
# entropy: 0.637160267433
# SoficProcessor on 11 symbols, with 19 states and 41 transitions
# entropy: 0.637160267433
# SoficProcessor on 11 symbols, with 19 states and 41 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 39 symbols, with 54 states and 104 transitions
# entropy: 0.677444091135
# SoficProcessor on 37 symbols, with 48 states and 102 transitions
# entropy: 0.677444091135
# SoficProcessor on 35 symbols, with 50 states and 104 transitions
# entropy: 0.677444091135
# SoficProcessor on 34 symbols, with 42 states and 86 transitions
# entropy: 0.677444091135
# SoficProcessor on 27 symbols, with 35 states and 69 transitions
# entropy: 0.677444091135
# SoficProcessor on 25 symbols, with 35 states and 69 transitions
# entropy: 0.677444091135
# SoficProcessor on 17 symbols, with 32 states and 66 transitions
# entropy: 0.637160267433
# SoficProcessor on 14 symbols, with 26 states and 54 transitions
# entropy: 0.637160267433
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 7 states and 12 transitions
# entropy: 0.528048909513
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 11 states and 19 transitions
# entropy: 0.528048909513
# SoficProcessor on 17 symbols, with 24 states and 49 transitions
# entropy: 0.657833419371
# SoficProcessor on 17 symbols, with 23 states and 47 transitions
# entropy: 0.657833419371
# SoficProcessor on 14 symbols, with 19 states and 41 transitions
# entropy: 0.657833419371
# SoficProcessor on 14 symbols, with 20 states and 43 transitions
# entropy: 0.657833419371
# SoficProcessor on 11 symbols, with 19 states and 41 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 18 states and 40 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 8 symbols, with 14 states and 29 transitions
# entropy: 0.589844146079
# SoficProcessor on 41 symbols, with 54 states and 104 transitions
# entropy: 0.677444091135
# SoficProcessor on 41 symbols, with 51 states and 98 transitions
# entropy: 0.677444091135
# SoficProcessor on 36 symbols, with 45 states and 90 transitions
# entropy: 0.677444091135
# SoficProcessor on 34 symbols, with 46 states and 90 transitions
# entropy: 0.677444091135
# SoficProcessor on 31 symbols, with 36 states and 71 transitions
# entropy: 0.677444091135
# SoficProcessor on 25 symbols, with 34 states and 67 transitions
# entropy: 0.677444091135
# SoficProcessor on 19 symbols, with 24 states and 49 transitions
# entropy: 0.657833419371
# SoficProcessor on 17 symbols, with 23 states and 48 transitions
# entropy: 0.657833419371
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 19 symbols, with 24 states and 49 transitions
# entropy: 0.657833419371
# SoficProcessor on 15 symbols, with 18 states and 38 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 19 states and 41 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 17 states and 36 transitions
# entropy: 0.657833419371
# SoficProcessor on 12 symbols, with 20 states and 45 transitions
# entropy: 0.657833419371
# SoficProcessor on 10 symbols, with 18 states and 39 transitions
# entropy: 0.637160267433
# SoficProcessor on 8 symbols, with 16 states and 34 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 16 states and 33 transitions
# entropy: 0.637160267433
# SoficProcessor on 43 symbols, with 57 states and 113 transitions
# entropy: 0.677444091135
# SoficProcessor on 41 symbols, with 51 states and 99 transitions
# entropy: 0.677444091135
# SoficProcessor on 38 symbols, with 48 states and 93 transitions
# entropy: 0.677444091135
# SoficProcessor on 31 symbols, with 37 states and 73 transitions
# entropy: 0.677444091135
# SoficProcessor on 32 symbols, with 38 states and 75 transitions
# entropy: 0.677444091135
# SoficProcessor on 25 symbols, with 31 states and 62 transitions
# entropy: 0.677444091135
# SoficProcessor on 22 symbols, with 29 states and 61 transitions
# entropy: 0.677444091135
# SoficProcessor on 19 symbols, with 27 states and 57 transitions
# entropy: 0.677444091135
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 8 symbols, with 16 states and 34 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 19 symbols, with 30 states and 60 transitions
# entropy: 0.657833419371
# SoficProcessor on 15 symbols, with 20 states and 43 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 17 states and 36 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 19 states and 41 transitions
# entropy: 0.657833419371
# SoficProcessor on 11 symbols, with 19 states and 41 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 8 symbols, with 14 states and 29 transitions
# entropy: 0.589844146079
# SoficProcessor on 48 symbols, with 59 states and 116 transitions
# entropy: 0.677444091135
# SoficProcessor on 41 symbols, with 52 states and 100 transitions
# entropy: 0.677444091135
# SoficProcessor on 36 symbols, with 46 states and 90 transitions
# entropy: 0.677444091135
# SoficProcessor on 33 symbols, with 46 states and 92 transitions
# entropy: 0.677444091135
# SoficProcessor on 27 symbols, with 33 states and 66 transitions
# entropy: 0.677444091135
# SoficProcessor on 23 symbols, with 28 states and 56 transitions
# entropy: 0.677444091135
# SoficProcessor on 21 symbols, with 28 states and 60 transitions
# entropy: 0.677444091135
# SoficProcessor on 18 symbols, with 24 states and 51 transitions
# entropy: 0.657833419371
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# entropy: -inf
# SoficProcessor on 4 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 15 symbols, with 25 states and 53 transitions
# entropy: 0.657833419371
# SoficProcessor on 17 symbols, with 23 states and 47 transitions
# entropy: 0.657833419371
# SoficProcessor on 14 symbols, with 17 states and 35 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 17 states and 36 transitions
# entropy: 0.657833419371
# SoficProcessor on 12 symbols, with 20 states and 45 transitions
# entropy: 0.657833419371
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 8 symbols, with 14 states and 29 transitions
# entropy: 0.589844146079
# SoficProcessor on 43 symbols, with 60 states and 118 transitions
# entropy: 0.677444091135
# SoficProcessor on 41 symbols, with 55 states and 106 transitions
# entropy: 0.677444091135
# SoficProcessor on 35 symbols, with 43 states and 83 transitions
# entropy: 0.677444091135
# SoficProcessor on 34 symbols, with 44 states and 87 transitions
# entropy: 0.677444091135
# SoficProcessor on 31 symbols, with 44 states and 88 transitions
# entropy: 0.677444091135
# SoficProcessor on 23 symbols, with 30 states and 62 transitions
# entropy: 0.677444091135
# SoficProcessor on 20 symbols, with 27 states and 57 transitions
# entropy: 0.677444091135
# SoficProcessor on 16 symbols, with 24 states and 52 transitions
# entropy: 0.657833419371
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 1 symbols, with 3 states and 2 transitions
# entropy: -inf
# SoficProcessor on 5 symbols, with 11 states and 18 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 8 symbols, with 17 states and 36 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 15 symbols, with 26 states and 56 transitions
# entropy: 0.657833419371
# SoficProcessor on 16 symbols, with 21 states and 45 transitions
# entropy: 0.657833419371
# SoficProcessor on 14 symbols, with 18 states and 39 transitions
# entropy: 0.657833419371
# SoficProcessor on 13 symbols, with 19 states and 41 transitions
# entropy: 0.657833419371
# SoficProcessor on 11 symbols, with 19 states and 41 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 44 symbols, with 59 states and 115 transitions
# entropy: 0.677444091135
# SoficProcessor on 40 symbols, with 51 states and 100 transitions
# entropy: 0.677444091135
# SoficProcessor on 37 symbols, with 48 states and 92 transitions
# entropy: 0.677444091135
# SoficProcessor on 33 symbols, with 43 states and 86 transitions
# entropy: 0.677444091135
# SoficProcessor on 29 symbols, with 33 states and 66 transitions
# entropy: 0.677444091135
# SoficProcessor on 25 symbols, with 32 states and 64 transitions
# entropy: 0.677444091135
# SoficProcessor on 20 symbols, with 27 states and 57 transitions
# entropy: 0.677444091135
# SoficProcessor on 19 symbols, with 26 states and 56 transitions
# entropy: 0.677444091135
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 4 symbols, with 9 states and 15 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 8 states and 13 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 2 symbols, with 6 states and 10 transitions
# entropy: 0.528048909513
# SoficProcessor on 3 symbols, with 7 states and 11 transitions
# entropy: 0.528048909513
# SoficProcessor on 8 symbols, with 16 states and 34 transitions
# entropy: 0.637160267433
# SoficProcessor on 7 symbols, with 13 states and 22 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 7 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 6 symbols, with 12 states and 20 transitions
# entropy: 0.528048909513
# SoficProcessor on 5 symbols, with 10 states and 17 transitions
# entropy: 0.528048909513
# SoficProcessor on 16 symbols, with 24 states and 52 transitions
# entropy: 0.677444091135
# SoficProcessor on 16 symbols, with 22 states and 46 transitions
# entropy: 0.677444091135
# SoficProcessor on 13 symbols, with 20 states and 39 transitions
# entropy: 0.657833419371
# SoficProcessor on 11 symbols, with 18 states and 40 transitions
# entropy: 0.657833419371
# SoficProcessor on 11 symbols, with 18 states and 40 transitions
# entropy: 0.657833419371
# SoficProcessor on 10 symbols, with 18 states and 39 transitions
# entropy: 0.637160267433
# SoficProcessor on 9 symbols, with 17 states and 35 transitions
# entropy: 0.637160267433
# SoficProcessor on 8 symbols, with 17 states and 36 transitions
# entropy: 0.637160267433
# SoficProcessor on 43 symbols, with 55 states and 106 transitions
# entropy: 0.69314718056
# SoficProcessor on 43 symbols, with 56 states and 108 transitions
# entropy: 0.69314718056
# SoficProcessor on 35 symbols, with 41 states and 80 transitions
# entropy: 0.69314718056
# SoficProcessor on 36 symbols, with 44 states and 90 transitions
# entropy: 0.69314718056
# SoficProcessor on 29 symbols, with 37 states and 72 transitions
# entropy: 0.677444091135
# SoficProcessor on 26 symbols, with 38 states and 76 transitions
# entropy: 0.677444091135
# SoficProcessor on 19 symbols, with 27 states and 55 transitions
# entropy: 0.677444091135
# SoficProcessor on 21 symbols, with 29 states and 60 transitions
# entropy: 0.677444091135
# raf@havock:~/projects/rads/runs$ python plateaus_rerun_ap_b2.py 
# SoficProcessor on 91 symbols, with 123 states and 235 transitions
# entropy: 0.650100370634
# SoficProcessor on 73 symbols, with 97 states and 190 transitions
# entropy: 0.650100370634
# SoficProcessor on 65 symbols, with 87 states and 169 transitions
# entropy: 0.650100370634
# SoficProcessor on 59 symbols, with 78 states and 149 transitions
# entropy: 0.650100370634
# SoficProcessor on 57 symbols, with 72 states and 134 transitions
# entropy: 0.650100370634
# SoficProcessor on 53 symbols, with 65 states and 122 transitions
# entropy: 0.650100370634
# SoficProcessor on 52 symbols, with 67 states and 122 transitions
# entropy: 0.650100370634
# SoficProcessor on 47 symbols, with 62 states and 113 transitions
# entropy: 0.644687552555
# SoficProcessor on 99 symbols, with 123 states and 239 transitions
# entropy: 0.677444091135
# SoficProcessor on 87 symbols, with 103 states and 210 transitions
# entropy: 0.677444091135
# SoficProcessor on 73 symbols, with 86 states and 174 transitions
# entropy: 0.677444091135
# SoficProcessor on 70 symbols, with 78 states and 151 transitions
# entropy: 0.677444091135
# SoficProcessor on 67 symbols, with 72 states and 139 transitions
# entropy: 0.677444091135
# SoficProcessor on 64 symbols, with 70 states and 134 transitions
# entropy: 0.677444091135
# SoficProcessor on 60 symbols, with 67 states and 131 transitions
# entropy: 0.677444091135
# SoficProcessor on 56 symbols, with 65 states and 124 transitions
# entropy: 0.677444091135
# SoficProcessor on 93 symbols, with 111 states and 219 transitions
# entropy: 0.677444091135
# SoficProcessor on 90 symbols, with 105 states and 212 transitions
# entropy: 0.677444091135
# SoficProcessor on 80 symbols, with 93 states and 184 transitions
# entropy: 0.677444091135
# SoficProcessor on 71 symbols, with 80 states and 153 transitions
# entropy: 0.677444091135
# SoficProcessor on 68 symbols, with 72 states and 140 transitions
# entropy: 0.677444091135
# SoficProcessor on 64 symbols, with 72 states and 141 transitions
# entropy: 0.677444091135
# SoficProcessor on 59 symbols, with 66 states and 131 transitions
# entropy: 0.677444091135
# SoficProcessor on 57 symbols, with 63 states and 124 transitions
# entropy: 0.677444091135
# SoficProcessor on 92 symbols, with 110 states and 219 transitions
# entropy: 0.677444091135
# SoficProcessor on 89 symbols, with 107 states and 215 transitions
# entropy: 0.677444091135
# SoficProcessor on 68 symbols, with 78 states and 156 transitions
# entropy: 0.677444091135
# SoficProcessor on 71 symbols, with 80 states and 156 transitions
# entropy: 0.677444091135
# SoficProcessor on 67 symbols, with 77 states and 152 transitions
# entropy: 0.677444091135
# SoficProcessor on 64 symbols, with 71 states and 141 transitions
# entropy: 0.677444091135
# SoficProcessor on 59 symbols, with 68 states and 134 transitions
# entropy: 0.677444091135
# SoficProcessor on 55 symbols, with 65 states and 130 transitions
# entropy: 0.677444091135
# SoficProcessor on 103 symbols, with 126 states and 245 transitions
# entropy: 0.677444091135
# SoficProcessor on 85 symbols, with 103 states and 206 transitions
# entropy: 0.677444091135
# SoficProcessor on 75 symbols, with 85 states and 167 transitions
# entropy: 0.677444091135
# SoficProcessor on 72 symbols, with 80 states and 156 transitions
# entropy: 0.677444091135
# SoficProcessor on 65 symbols, with 79 states and 157 transitions
# entropy: 0.677444091135
# SoficProcessor on 61 symbols, with 71 states and 142 transitions
# entropy: 0.677444091135
# SoficProcessor on 60 symbols, with 69 states and 137 transitions
# entropy: 0.677444091135
# SoficProcessor on 57 symbols, with 65 states and 130 transitions
# entropy: 0.677444091135
# SoficProcessor on 101 symbols, with 126 states and 245 transitions
# entropy: 0.677444091135
# SoficProcessor on 85 symbols, with 103 states and 206 transitions
# entropy: 0.677444091135
# SoficProcessor on 77 symbols, with 88 states and 172 transitions
# entropy: 0.677444091135
# SoficProcessor on 71 symbols, with 78 states and 152 transitions
# entropy: 0.677444091135
# SoficProcessor on 65 symbols, with 73 states and 145 transitions
# entropy: 0.677444091135
# SoficProcessor on 63 symbols, with 70 states and 138 transitions
# entropy: 0.677444091135
# SoficProcessor on 62 symbols, with 69 states and 137 transitions
# entropy: 0.677444091135
# SoficProcessor on 57 symbols, with 66 states and 133 transitions
# entropy: 0.677444091135
# SoficProcessor on 99 symbols, with 128 states and 247 transitions
# entropy: 0.677444091135
# SoficProcessor on 85 symbols, with 101 states and 202 transitions
# entropy: 0.677444091135
# SoficProcessor on 78 symbols, with 89 states and 174 transitions
# entropy: 0.677444091135
# SoficProcessor on 71 symbols, with 78 states and 152 transitions
# entropy: 0.677444091135
# SoficProcessor on 67 symbols, with 75 states and 147 transitions
# entropy: 0.677444091135
# SoficProcessor on 63 symbols, with 70 states and 139 transitions
# entropy: 0.677444091135
# SoficProcessor on 59 symbols, with 68 states and 135 transitions
# entropy: 0.677444091135
# SoficProcessor on 56 symbols, with 65 states and 130 transitions
# entropy: 0.677444091135
# SoficProcessor on 105 symbols, with 127 states and 253 transitions
# entropy: 0.69314718056
# SoficProcessor on 89 symbols, with 105 states and 214 transitions
# entropy: 0.69314718056
# SoficProcessor on 83 symbols, with 90 states and 176 transitions
# entropy: 0.69314718056
# SoficProcessor on 79 symbols, with 85 states and 165 transitions
# entropy: 0.69314718056
# SoficProcessor on 62 symbols, with 68 states and 136 transitions
# entropy: 0.69314718056
# SoficProcessor on 68 symbols, with 76 states and 152 transitions
# entropy: 0.69314718056
# SoficProcessor on 64 symbols, with 74 states and 147 transitions
# entropy: 0.69314718056
# SoficProcessor on 62 symbols, with 72 states and 145 transitions
# entropy: 0.69314718056

