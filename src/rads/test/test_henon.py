#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps import HenonMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx

depth = 11

# main bounding box
box = np.array([[-2.0,-2],[4,4]])

# our tree, mapper, enclosure
tree = Tree(box,full=True)
m = HenonMapper()
ce = CombEnc(tree,m)

for d in range(depth):
	print 'at depth', d
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I = graph_mis(ce.mvm)
	print 'len(I) = ', len(I)
	# now remove all boxes not in I (the maximal invariant set)
	ce.tree.remove(list(set(range(ce.tree.size))-set(I)))

# now display the tree!
boxes = ce.tree.boxes()
gfx.show_uboxes(boxes, col='c', ecol='b')

