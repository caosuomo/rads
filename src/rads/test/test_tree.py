#!/usr/bin/python


import numpy as np
import numpy.random as nprand
import matplotlib.pylab as plt
from rads.enclosure import UBoxSet,Tree
from rads.misc import gfx

box = np.array([[0.0,0],[8,8]])
tree = Tree(box)
for i in range(6):
	tree.subdivide()


nprand.seed( 123 )

def test_one_box(box,tree,graphics=False,callback=None):#,f):
	print 'box',box[0],box[1],':',
	s = tree.search(box)
	print ""
	print "box search:", s
	print "len(s):", len( s )
	boxes = tree.boxes()
	if graphics:
		plt.close()
		gfx.show_uboxes(boxes)
		gfx.show_uboxes(boxes, S=s, col='r')
	if len(s) < ((tree.dim**tree.depth)/2): # dim^depth/2
		t = tree.insert(box)
		if graphics:
			boxes = tree.boxes()
			gfx.show_uboxes(boxes, S=t, col='c')
		print 'ins:',t
 	else:
 		t = tree.remove(s)
		print 'rem:',t

	if graphics:
		gfx.show_box(box,col='g',alpha=0.5)
		if callback:
			plt.gcf().canvas.mpl_connect('button_press_event', callback)
		plt.show()

		#raw_input("hit enter...")
	

def rand_test(event=None):
	v = nprand.rand(2)*4 - 1
#	w = nprand.rand(2)*0.1
	w = nprand.rand(2)*3
	box = np.array([v,w])
	test_one_box(box,tree,True,rand_test_int)

def rand_test_int(event=None):
	v = nprand.randint(-1,8,size=2)
#	w = nprand.randint(1,2,size=2)
	w = nprand.randint(1,6,size=2)
	box = np.array([v,w])
	test_one_box(box,tree,True,rand_test)

rand_test()

# for i in range(500):
# #	print "rand_test"
# 	rand_test()
# #	print "rand_test_int"
# 	rand_test_int()

