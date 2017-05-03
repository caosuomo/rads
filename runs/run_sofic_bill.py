#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor

index_map = scipy.io.loadmat('/home/raf/projects/tmp/index.mat')['hom_matrix']
reg2gen_cell = scipy.io.loadmat('/home/raf/projects/tmp/generators.mat')
reg2gen = utils.cell2dict(reg2gen_cell['generators'])

print index_map
print reg2gen

sof = SoficProcessor(index_map,reg2gen,debug=True)
sof.process()
print sof
print 'entropy:', sof.entropy()
