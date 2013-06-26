#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps import HenonMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom
from rads.graphs import DiGraph
import sys
from itertools import combinations
import networkx as nx

#---------------------
# set parameters here
#---------------------

if len( sys.argv ) == 1:
	depth = 7
else:
	depth = int( sys.argv[1] )

# values for Henon params
a = 1.4
b = 0.3

# recurrence/preiodicity to look for
max_cycle = 2

# toggle verbose output
chatter = True

# for ProcessHomology object below
mapname = 'henon'
# where to save cubical files and index map matrices, etc.
prefix = '/Users/jberwald/github/local/caja-matematica/rads/src/rads/test/debug/'

#---------------------

# main bounding box
box = np.array([[-1.0,-1.0],[4,4]])

# our tree, mapper, enclosure
tree = Tree(box,full=True)
mapper = HenonMapper()

# set params here
p = mapper.get_params()
p['a'][:] = a
p['b'][:] = b
mapper.set_params( p )

ce = CombEnc(tree, mapper)

for d in range(depth):
	ce.tree.subdivide()
	ce.update()
        # grab the SCC's of size > 1 (R)
	I,S,R = graph_mis(ce.mvm, return_rsccs=True)
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	ce.tree.remove(list(nodes-set(I)))
        if chatter:
        	print 'at depth', d+1
                print 'subdivided:', ce.tree.size, 'boxes'
                print 'enclosure updated'
        	print 'len(I) = ', len(I)

# update the MVM and ADJ by removing nodes not in I
ce.mvm.remove_nodes_from( nodes - set(I) )

# we have to relabel the nodes to go from 0..N
new_mvm = nx.convert_node_labels_to_integers( ce.mvm.graph )
ce.mvm.graph = new_mvm

ce.adj.remove_nodes_from( nodes - set(I) )
new_adj = nx.convert_node_labels_to_integers( ce.adj.graph )
ce.adj.graph = new_adj

# draw the outer enclosure. the 'mvm' is the MultiValued Map on the
# nodes/intervals in the subdivided grid on [0,1]. 
#ce.mvm.draw( node_size=200, with_labels=True )

# now display the tree!
boxes = ce.tree.boxes()

gfx.show_uboxes(boxes, col='c', ecol='b')

print ""
print"Edges in outer approximation:\n", len( ce.mvm.edges() )
print ""
print "Self loops in the outer enclosure\n", ce.mvm.graph.selfloop_edges() 


#===========================
# NEED TO DETERMINE A REGION TO FOCUS ON
#===========================
# If len(r)>1, then there must be two SCC's. For now we ignore the
# fact that SCC's of size==1 will have zero entropy and just create a
# mapping between r and s:
regions_of_interest = dict().fromkeys( R )
for u in regions_of_interest:
	# subgraph of the MVM containing nodes from the SCC
	G = ce.mvm.subgraph( S[u] )
	# H = DiGraph()
	# H.graph = G
	regions_of_interest[u] = G

# for each component find some cycles and connect them
#cycles = dict().fromkeys( regions_of_interest.keys() )
for u,subH in regions_of_interest.iteritems():
	#C = homology.get_cycles( ce.mvm, 4 )

        

	# Don't bother with single node SCC's
	if len( subH ) == 1:
		continue
	cycles = hom.get_cycles( subH, max_cycle, debug=chatter )
	cycle_combs = combinations( cycles, 2 )
	
	# Here we need to explore various combinations of cycle
	# lengths and nodes to connect between the sets
	cycles_conns = set()
	for comb in cycle_combs:
		c = hom.connect_cycles( cycles[comb[0]],
                                        cycles[comb[1]],
                                        subH )
		cycles_conns.update( c )

	for c in cycles.values():
		cycles_conns.update( c ) 


# Now create the homology processing factory.
# Focus on the cycles + connections found above
ph = hom.ProcessHomology( ce.adj, ce.mvm, ce.tree, cycles_conns, 
                          mapname=mapname, 
                          prefix=prefix )


# Now, create the index pair (X,A) and compute its homology
print ""
print "Computing homology:\n"

print ""
print "Writing cubical files and map file to disk..."

# First, scale all the boxes to an integer grid, and store this for
# writing index pairs to disk.
ph.scale_boxes()

ph.grow_isolating_neighborhood()# idx, ce.adj, ce.mvm )
X,A,Y,B = ph.make_index_pair( return_values=True )

# write the index pair to files
# can also access ph.X, ph.A, etc for the index pair indices
mapname_path = ph.prefix + ph.mapname
for boxes,box_name in [(X,'X'), (A,'A'), (Y,'Y'), (B,'B')]:
        ph.box2cub( boxes, mapname_path +'-'+ box_name + '.cub' )

# write the map to file
# NOTE: map file has the suffix '.map' by default
ph.map_writer()


print ""
print "Computing induced homology..."

ph.run_homcubes( suffix='cub', debug=chatter )

print ""
print "The map on homology for dimensions 0 and 1:"
for d,m in ph.index_map.items():
        print "dim =", d
        print "f =", m
        print ""

hom.index_map_to_matrix( ph.index_map, ph.prefix+ph.mapname+'-idx' )

