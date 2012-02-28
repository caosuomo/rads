#!/usr/bin/python

import numpy as np
import numpy.random as nprand
import matplotlib.pylab as plt
from rads import CombEnc,Tree
from rads.maps import HenonMapper

box = np.array([[0.0,0],[8,8]])
tree = Tree(box,depth=6,full=True)
m = HenonMapper()
ce = CombEnc(tree,m)

ce.update()
print ce.adj
print ce.mvm

