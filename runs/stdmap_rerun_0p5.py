#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor


everything = scipy.io.loadmat( '/home/raf/projects/std/std_0p500_d15_MG.mat')
M = everything['M']
G = everything['G']
sof = SoficProcessor(M,utils.cell2dict(G))
sof.process()
print sof, 'entropy:', sof.entropy()
