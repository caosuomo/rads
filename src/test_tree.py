#!/usr/bin/python

import sys
sys.path.append("cython/build/")

from rads import UBoxSet,Tree
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

from rads import Mapper
from rads.maps import HenonMapper

m = HenonMapper()
print m.get_params()

from rads import CombEnc

ce = CombEnc()
ce.mapper = m
ce.tree = t
print ce
ce.update()
