#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps.henon import HenonMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx

depth = 5

# main bounding box
box = np.array([[-2.0,-2],[4,4]])

# our tree, mapper, enclosure
tree = Tree(box,full=True)
m = HenonMapper()

p = m.get_params()
p['a'][:] = 1.2
p['b'][:] = 0.2
m.set_params( p )

ce = CombEnc(tree,m)

for d in range(depth):
	print 'at depth', d
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I = graph_mis(ce.mvm)
	print 'len(I) = ', len(I[0]) # fix the extra stuff returned by graph_mis
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	ce.tree.remove(list(nodes-set(I)))

# remove nodes not in I
ce.mvm.remove_nodes_from( list(nodes-set(I)) )

# now display the tree!
boxes = ce.tree.boxes()
gfx.show_uboxes(boxes, col='c', ecol='b')

