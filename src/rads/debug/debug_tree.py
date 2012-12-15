#!/usr/bin/python

import sys
sys.path.append("cython/build/")

from rads import *
from rads import gfx
import numpy as np
import numpy.random as random
import matplotlib.pylab as plt

box = np.array([[0.0,0],[8,8]])
tree = Tree(box)
tree.insert(np.array([1,1]))
tree.subdivide()
# tree.insert(np.array([1,1]))
# tree.insert(np.array([5,1]))
# tree.insert(np.array([1,5]))
# tree.insert(np.array([5,5]))

boxes = tree.boxes()
print tree
print boxes
s = tree.search(np.array([[1,1],[1,4]]))
print "tree.search(np.array([[1,1],[1,4]])) =", s
print "should be [0,2]"
s = tree.search(np.array([[1,1],[4,1]]))
print "tree.search(np.array([[1,1],[4,1]])) =", s
print "should be [0,1]"

for i in range(0):
	v = random.rand(2)*4 - 1
	w = random.rand(2)*3
	box = np.array([v,w])
	s = tree.search(box)
	print v,w,':',s
	gfx.show_uboxes(boxes)
	gfx.show_uboxes(boxes, S=s, col='r')
	gfx.show_box(box,col='g')
	plt.show()
	plt.draw()
	if raw_input("Press enter when done!")==0:
		break
	plt.close()

