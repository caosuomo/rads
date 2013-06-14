#!/usr/bin/python
import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps.tent import TentMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx

depth = 2

# main bounding box -- defined by lower left corner (row 0) and width
# of each dimension (row 2). For the tent map, this should start out
# as np.array([[0.0],[1]])
box = np.array([[0.0],[1]])

# our tree (with root box), mapper, enclosure
tree = Tree( box, full=True )
tent_map = TentMapper()

p = tent_map.get_params()
p['r'][:] = 2.0
tent_map.set_params( p )

ce = CombEnc( tree, tent_map )

for d in range(depth):
	print 'at depth', d+1
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I = graph_mis(ce.mvm)
	print 'len(I) = ', len(I)
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	#ce.tree.remove(list(nodes-set(I)))

# draw the outer enclosure. the 'mvm' is the MultiValued Map on the
# nodes/intervals in the subdivided grid on [0,1]. 
ce.mvm.draw( node_size=200, with_labels=True )
print "self loops in the outer enclosure", ce.mvm.graph.selfloop_edges() 
