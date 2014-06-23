#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor


everything = scipy.io.loadmat( '/home/raf/Dropbox/henon-tipping-14-run-goodruns.mat')
ind = everything['index_info']
ents_dft = everything['ents_dft']
ents_max = everything['ents_max']
ents_sof = []
for i in range(ind.shape[0]):
    sof = SoficProcessor(ind[i,1],utils.cell2dict(ind[i,0]))
    sof.process()
    ents_sof.append(sof.entropy())
    print sof, 'entropy:', ents_sof[-1]
