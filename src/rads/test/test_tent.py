#!/usr/bin/python
import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps.tent import TentMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom

# depth=2 ==> 4 subintervals
depth = 5 

# main bounding box -- defined by lower left corner (row 0) and width
# of each dimension (row 2). For the tent map, this should start out
# as np.array([[0.0],[1]])

# Test out 
box = np.array([[0.],[1]]) # <-- Are there some edges missing. In
                           # particular, at depth 2 shouldn't node 1
                           # have a self loop plus more out edges??

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

print"   Edges in outer approximation:\n", ce.mvm.edges()
print "  Self loops in the outer enclosure\n", ce.mvm.graph.selfloop_edges() 

# Now, create the index pair (X,A) and compute its homology
print ""
print "Computing homology: We only select "
ids = raw_input( "Based on the self-loops, enter one or more grid id's to analyze, separated by ',': " )

regs = ids.split( ',' )
idx = [ int( x ) for x in regs ]

print "Analyzing regions "
for x in idx:
        print x

iso = hom.grow_isolating_neighborhood( idx, ce.adj, ce.mvm )
X,A,Y,B = hom.make_index_pair( iso, ce.adj, ce.mvm )
