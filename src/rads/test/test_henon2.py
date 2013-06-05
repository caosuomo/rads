#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps import HenonMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology
from rads.graphs import DiGraph
import sys
from itertools import combinations

if len( sys.argv ) == 1:
	depth = 7
else:
	depth = int( sys.argv[1] )


max_cycle = 2

# main bounding box
box = np.array([[-2.0,-2],[4,4]])

# our tree, mapper, enclosure
tree = Tree(box,full=True)
m = HenonMapper()

p = m.get_params()
p['a'][:] = 1.4
p['b'][:] = 0.3
m.set_params( p )

ce = CombEnc(tree,m)

for d in range(depth):
	print 'at depth', d
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I,s,r = graph_mis(ce.mvm, return_rsccs=True)
	
	print 'len(I) = ', len(I)
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	#ce.tree.remove(list(nodes-set(I)))
	print ""

# remove nodes not in I
#ce.mvm.remove_nodes_from( list(nodes-set(I)) )

# If len(r)>1, then there must be two SCC's. For now we ignore the
# fact that SCC's of size==1 will have zero entropy and just create a
# mapping between r and s:
components = dict().fromkeys( r )
for u in components:
	# subgraph of the MVM containing nodes from the SCC
	G = ce.mvm.subgraph( s[u] )
	# H = DiGraph()
	# H.graph = G
	components[u] = G


# now display the tree!
boxes = ce.tree.boxes()
gfx.show_uboxes(boxes, col='c', ecol='b')


# for each component find some cycles and connect them
#cycles = dict().fromkeys( components.keys() )
for u,subH in components.iteritems():
	#C = homology.get_cycles( ce.mvm, 4 )

	# Don't bother with single node SCC's
	if len( subH ) == 1:
		continue
	cycles = homology.get_cycles( subH, max_cycle )
	cycle_combs = combinations( cycles, 2 )
	
	# Here we need to explore various combinations of cycle
	# lengths and nodes to connect between the sets
	cycles_conns = set()
	for comb in cycle_combs:
		c = homology.connect_cycles( cycles[comb[0]],
					     cycles[comb[1]],
					     subH )
		cycles_conns.update( c )

	for c in cycles.values():
		cycles_conns.update( c ) 

	# grow isolated invariant set
	N = homology.grow_iso( cycles_conns, ce.adj, ce.mvm )

	# # build index pair
	X, A,oI = homology.build_ip( ce.mvm, ce.adj , N )

	# # construct Y and B as well
	Y, B = homology.computeYB( X, N, ce.mvm )

# path = '/data/entropy/sandbox/'
# homology.run_homcubes( 'test_henon', full_path=path )
