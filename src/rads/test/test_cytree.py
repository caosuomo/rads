#!/usr/bin/python

import sys
sys.path.append("cython/build/")

from rads.enclosure import UBoxSet # cyboxset
from rads.enclosure import Tree # cytree
import numpy as np

box = np.array([[0.0,0],[8,8]])
t = Tree(box)
t.subdivide()
t.subdivide()
t.subdivide()
print "insert", t.insert(box/8 + 0.5)
print "insert", t.insert(np.array([[4,4],[1,1]]))
print "insert [3,3]", t.insert(np.array([3,3]))
print "search [3,3]", t.search(np.array([3,3]))
ubs = t.boxes()
print ubs
