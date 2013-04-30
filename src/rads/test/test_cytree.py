#!/usr/bin/python

import sys
sys.path.append("cython/build/")

#import cyboxset
from rads.enclosure import UBoxSet
#import cytree
from rads.enclosure import Tree
from rads.misc import gfx
import numpy as np


box = np.array([[0.0,0],[8,8]])
t = Tree(box)
t.subdivide()
t.subdivide()
t.subdivide()
print t.insert(box/8 + 0.5)
print t.insert(np.array([[4,4],[1,1]]))
print t.insert(np.array([3,3]))
print t.search(np.array([3,3]))
ubs = t.boxes()
print ubs

gfx.show_uboxes(ubs, col='c', ecol='b')
