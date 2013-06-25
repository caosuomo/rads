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
#ce.mvm.draw( node_size=200, with_labels=True )

print ""
print"Edges in outer approximation:\n", ce.mvm.edges()
print ""
print "Self loops in the outer enclosure\n", ce.mvm.graph.selfloop_edges() 

# Now, create the index pair (X,A) and compute its homology
print ""
print "Computing homology:\n"

 # at depth 5, this is the region containing the non-zero fixed point
idx = [21] 
# Uncomment the region in order to specify other regions to analyze
# ids = raw_input( "Based on the self-loops above,"\
#                  " enter one or more grid id's to analyze, separated by ',': " )

# regs = ids.split( ',' )
# idx = [ int( x ) for x in regs ]

print "Analyzing regions "
for x in idx:
        print x
print ""
print "Writing cubical files and map file to disk..."

ph = hom.ProcessHomology( ce.adj, ce.mvm, ce.tree, idx, 
                          mapname='tent', 
                          prefix='/Users/jberwald/github/local/caja-matematica/rads/src/rads/test/debug/' )


# scale all the boxes to an integer grid, and store this for writing
# index pairs to disk.
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

ph.run_homcubes( suffix='cub' )

print ""
print "The map on homology for dimensions 0 and 1:"
for d,m in ph.index_map.items():
        print "dim =", d
        print "f =", m
        print ""

print "The map on homology for d=0 is trivial."
print "For dimension 1, we read the map as follows:\n"\
        "Generator 1 is mapped negatively (-1) across generator 1 (itself).\n"\
        "More precisely, f(\gamma_1) = -1 * \gamma_1"

## Now process the index map to verify the symbolics

