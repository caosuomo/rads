#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps.ricker import RickerMapper # for some reason must have .ricker (??)
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx

depth = 3

# main bounding box
box = np.array([[1.0,1.0],[4,4]])

print "Initial bounding box", box

# our tree, mapper, enclosure
tree = Tree(box,full=True)
m = RickerMapper()
ce = CombEnc(tree,m)

for d in range(depth):
	print 'at depth', d
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I = graph_mis(ce.mvm)
	print 'len(I) = ', len(I) # fix the extra stuff returned by graph_mis
	# now remove all boxes not in I (the maximal invariant set)
	
	ce.tree.remove(list(set(range(ce.tree.size))-set(I)))

# now display the tree!
boxes = ce.tree.boxes()
gfx.show_uboxes(boxes, col='c', ecol='b')

