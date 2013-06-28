#!/usr/bin/python
import numpy as np
from rads.enclosure import CombEnc,Tree
from rads.maps.tent import TentMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom

#---------------------
# set parameters here
#---------------------

# subdivision
depth = 5

# slope of the line
r = 2.0

# toggle some of the messages
chatter = False

mapname = 'tent'
prefix = '/Users/jberwald/github/local/caja-matematica/rads/src/rads/test/debug/'

#---------------------
# 
#---------------------
    
# main bounding box -- defined by lower left corner (row 0) and width
# of each dimension (row 2). For the tent map, this should start out
# as np.array([[0.0],[1]])
box = np.array([[0.],[1]])

# our tree (with root box), mapper, enclosure
tree = Tree( box, full=True )

# initialize the tent map
mapper = TentMapper()

# set the parameter
p = mapper.get_params()
p['r'][:] = r
mapper.set_params( p )

# initialize the enclosure with the tree and the map
ce = CombEnc( tree, mapper )

for d in range(depth):
	ce.tree.subdivide()
	ce.update()
	I = graph_mis(ce.mvm)
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	ce.tree.remove(list(nodes-set(I)))
        if chatter:
        	print 'at depth', d+1
                print 'subdivided:', ce.tree.size, 'boxes'
                print 'enclosure updated'
        	print 'len(I) = ', len(I)

# update the MVM by removing nodes not in I
ce.mvm.remove_nodes_from( nodes - set(I) )
ce.adj.remove_nodes_from( nodes - set(I) )

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
region = [21] 

# Uncomment the region in order to specify other regions to analyze
# ids = raw_input( "Based on the self-loops above,"\
#                  " enter one or more grid id's to analyze, separated by ',': " )

# regs = ids.split( ',' )
# idx = [ int( x ) for x in regs ]

print "Analyzing regions "
for x in region:
        print x
print ""
print "Writing cubical files and map file to disk..."


iso = hom.grow_isolating_neighborhood( region, ce.adj, ce.mvm )

CI = hom.ComputeIndex( ce.adj, ce.mvm, ce.tree, iso, 
                       mapname=mapname, 
                       prefix=prefix )

# scale all the boxes to an integer grid, and store this for writing
# index pairs to disk.
#CI.scale_boxes()
X,A,Y,B = CI.make_index_pair( return_values=True )

# write the index pair to files
# can also access CI.X, CI.A, etc for the index pair indices
mapname_path = CI.prefix + CI.mapname
for bx,box_name in [(X,'X'), (A,'A'), (Y,'Y'), (B,'B')]:
        CI.box2cub( bx, mapname_path +'-'+ box_name + '.cub' )

# draw the index pair. See gfx.py module for more details.
boxes = ce.tree.boxes()
fig = gfx.show_boxes1D( boxes, S=X, label='X', annotate=True )
fig = gfx.show_boxes1D( boxes, S=A, label='A', col='y', fig=fig )
fig.show()

# write the map to file
# NOTE: map file has the suffix '.map' by default
print ""
print "Wrting map..."
hom.map_writer( CI.mvm, CI.scaled_boxes, CI.prefix+CI.mapname )


print ""
print "Computing induced homology..."

CI.run_homcubes( suffix='cub' )
CI.write_index_map( CI.prefix+CI.mapname+'-index' )

