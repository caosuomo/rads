#!/usr/bin/python

import sys
import numpy as np
import scipy.io
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor


for f in "ap1r2.mat ap2r6.mat ap3r2.mat ap3r4.mat ap4r2.mat ap5r2.mat ap5r3.mat ap6r5.mat ap7r3.mat ap8r2.mat ap8r3.mat ap9r2.mat ap9r3.mat ap11r8.mat ap12r8.mat ap13r8.mat ap14r8.mat ap15r8.mat".split(" "):
    dat = utils.load_matlab_matrix( '/home/raf/Dropbox/projects/plateaus/focm-data/'+f, matname='d')
    print f
    # the [0,0][0,1] is to get the second entry of the cell, i.e. dat.M_inv{2}
    M = dat['M_inv'][0,0][0,1]
    G = utils.cell2dict(dat['G_inv'][0,0][0,1][0])
    sof = SoficProcessor(M,G)
    sof.process()
    print sof
    print 'entropy:', sof.entropy()

# ap1r2.mat
# SoficProcessor on 886 symbols, with 1096 states and 2065 transitions
# entropy: 0.637936814478
# ap2r6.mat
# SoficProcessor on 553 symbols, with 689 states and 1294 transitions
# entropy: 0.637347707344
# ap3r2.mat
# SoficProcessor on 834 symbols, with 999 states and 1870 transitions
# entropy: 0.639166621114
# ap3r4.mat
# SoficProcessor on 649 symbols, with 728 states and 1359 transitions
# entropy: 0.639166621114
# ap4r2.mat
# SoficProcessor on 848 symbols, with 1024 states and 1914 transitions
# entropy: 0.640425157866
# ap5r2.mat
# SoficProcessor on 822 symbols, with 1003 states and 1865 transitions
# entropy: 0.643190077748
# ap5r3.mat
# SoficProcessor on 766 symbols, with 954 states and 1786 transitions
# entropy: 0.643190077748
# ap6r5.mat
# SoficProcessor on 620 symbols, with 698 states and 1317 transitions
# entropy: 0.645937179746
# ap7r3.mat
# SoficProcessor on 729 symbols, with 864 states and 1630 transitions
# entropy: 0.647414248548
# ap8r2.mat
# SoficProcessor on 847 symbols, with 1050 states and 1983 transitions
# entropy: 0.653373865392
# ap8r3.mat
# SoficProcessor on 702 symbols, with 756 states and 1433 transitions
# entropy: 0.652715212878
# ap9r2.mat
# SoficProcessor on 847 symbols, with 1035 states and 1999 transitions
# entropy: 0.671510471614
# ap9r3.mat
# SoficProcessor on 770 symbols, with 908 states and 1747 transitions
# entropy: 0.672437425592
# ap11r8.mat
# SoficProcessor on 459 symbols, with 538 states and 1066 transitions
# entropy: 0.681418978871
# ap12r8.mat
# SoficProcessor on 452 symbols, with 498 states and 983 transitions
# entropy: 0.689326254499
# ap13r8.mat
# SoficProcessor on 453 symbols, with 494 states and 979 transitions
# entropy: 0.689326254499
# ap14r8.mat
# SoficProcessor on 454 symbols, with 498 states and 988 transitions
# entropy: 0.689326254499
# ap15r8.mat
# SoficProcessor on 454 symbols, with 495 states and 982 transitions
# entropy: 0.689326254499
