#!/usr/bin/python
import numpy as np
import networkx as nx
from rads.enclosure import CombEnc,Tree
from rads.maps.rescaled import RescaledMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom

#---------------------
# set parameters here
#---------------------

# subdivision
depth = 6

# slope of the line
r = 2.0
a = 1.0
b = 0.0

# toggle some of the messages
chatter = False

mapname = 'rescaled'
#prefix = '/Users/clairebobst/Desktop/Research/data/'
prefix = '/Users/jberwald/github/local/caja-matematica/rads/src/rads/test/debug/'

#---------------------
# 
#---------------------
    
# main bounding box -- defined by lower left corner (row 0) and width
# of each dimension (row 2). For the tent map, this should start out
# as np.array([[0.0],[1]])
#box = np.array([[0.],[1.]])
box = np.array([[-.25],[1.25]])

# our tree (with root box), mapper, enclosure
tree = Tree( box, full=True )

# initialize the tent map
mapper = RescaledMapper()

# set the parameter
p = mapper.get_params()
p['r'][:] = r
p['a'][:] = a
p['b'][:] = b
mapper.set_params( p )

# initialize the enclosure with the tree and the map
ce = CombEnc( tree, mapper )

for d in range(depth):
	ce.tree.subdivide()
	ce.update()
	I = graph_mis(ce.mvm)
	
	#add 1-box neighborhood
	if d == depth - 1:
	    I.append( min(I) - 1)
	    I.append( max(I) + 1)
	
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

new_mvm = nx.convert_node_labels_to_integers( ce.mvm.graph )
ce.mvm.graph = new_mvm
new_adj = nx.convert_node_labels_to_integers( ce.adj.graph )
ce.adj.graph = new_adj


# draw the outer enclosure. the 'mvm' is the MultiValued Map on the
# nodes/intervals in the subdivided grid on [0,1]. 
#ce.mvm.draw( node_size=200, with_labels=True )

print ""
print"Edges in outer approximation:\n", ce.mvm.edges()
print ""
print "Self loops in the outer enclosure\n", ce.mvm.graph.selfloop_edges() 

print ""
print "Computing self-loops and period 2 orbits..."

# Here's the transition matrix
P = ce.mvm.to_numpy_matrix()

#first, find self loops
selfloops = np.where( P.diagonal() != 0 )[1] 
# convert to set so we can do set operations below
# 0 index is due to strange matrix--> list conversion format
selfloops = set( selfloops.tolist()[0] )

print "self-loops:", selfloops

p2 = P**2
diag = p2.diagonal()
period2 = np.where( diag != 0 )[1] 
period2 = set( period2.tolist()[0] )
period2 = period2 - selfloops

print "period 2 orbits:", period2


print "** Use regions in collection of selfloops and period 2 orbits to determine "\
        "a region to pass to grow_isolating_neighborhood()."

# at depth 5, this is the region containing the non-zero fixed point
# print ""
# print "SETTING REGION TO 21. THIS IS JUST A DEFAULT AND ONLY MAKES SENSE FOR DEPTH=5!!"
print ""
region = [1] 

# **********************
# Uncomment the lines below in order to specify other regions to analyze
# **********************
# ids = raw_input( "Based on the self-loops above,"\
#                  " enter one or more grid id's to analyze, separated by ',': " )

# regs = ids.split( ',' )
# idx = [ int( x ) for x in regs ]

print "Analyzing region(s) "
for x in region:
        print x
print ""


iso = hom.grow_isolating_neighborhood( region, ce.adj, ce.mvm )

CI = hom.ComputeIndex( ce.adj, ce.mvm, ce.tree, iso, 
                       mapname=mapname, 
                       prefix=prefix )

# scale all the boxes to an integer grid, and store this for writing
# index pairs to disk.
X,A,Y,B = CI.make_index_pair( return_values=True )

# write the index pair to files
# can also access CI.X, CI.A, etc for the index pair indices
print "Writing cubical files and map file to disk..."
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
print "Writing map..."
hom.map_writer( CI.mvm, CI.scaled_boxes, CI.prefix+CI.mapname )


print ""
print "Computing induced homology..."

CI.run_homcubes( suffix='cub' )
CI.write_index_map( CI.prefix+CI.mapname+'-index' )

print "Homology: ", CI.index_map

# for convenience in ipython
C = CI 
