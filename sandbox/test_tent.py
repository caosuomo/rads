#!/usr/bin/python
import numpy as np
import networkx as nx
from rads.enclosure import CombEnc,Tree
from rads.maps.tent import TentMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom

#---------------------
# set parameters here
#---------------------

# subdivision
depth = 4

# slope of the line
r = 2

# toggle some of the messages
chatter = False

mapname = 'tent'
prefix = '/Users/clairebobst/Desktop/Research/data/'

#---------------------
# 
#---------------------
    
# main bounding box -- defined by lower left corner (row 0) and width
# of each dimension (row 2). For the tent map, this should start out
# as np.array([[0.0],[1]])

box = np.array([[-0.25],[1.25]])

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

# subdivide and update the combinatorial enclosure at each depth
for d in range(depth):
	ce.tree.subdivide()
	ce.update()
	I = graph_mis(ce.mvm)
	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))

        # if final depth, wait until we grow a one-box 'hood.
        if not d == depth-1:
                ce.tree.remove(list(nodes-set(I)))
        if chatter:
        	print 'at depth', d+1
                print 'subdivided:', ce.tree.size, 'boxes'
                print 'enclosure updated'
        	print 'len(I) = ', len(I)

# reset I by growing a one-box neighborhood around the final invariant
# set
oI = hom.get_onebox( I, ce.adj )
I = oI

# keep track of 'physical' box corners stored in B
box_corners = ce.tree.boxes().corners
idx_mapping = {}
for i in I:
        idx_mapping[i] = box_corners[i]

# now trim the tree, which will reorder/renumber the corners
ce.tree.remove( list( nodes - set(I) ) )

# record this reordering as node attributes by searching for the index
# of each box
for i,u in idx_mapping.items():
        ce.mvm.graph.node[i]['corner'] = ce.tree.search( u )


# update the MVM and adjacency graphs by removing nodes not in I
ce.mvm.remove_nodes_from( nodes - set(I) )
ce.adj.remove_nodes_from( nodes - set(I) )


#box_corners = ce.tree.boxes().corners

# draw the outer enclosure. the 'mvm' is the MultiValued Map on the
# nodes/intervals in the subdivided grid on root box. 
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

p4 = P**4
diag = p4.diagonal()
period4 = np.where( diag != 0 )[1] 
period4 = set( period4.tolist()[0] )
period4 = period4 - period2 - selfloops

print "period 4 orbits:", period4

print "** Use regions in collection of selfloops and period 2 orbits to determine "\
        "a region to pass to grow_isolating_neighborhood()."


print ""

region = [8, 13] 

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
print "Wrting map..."
hom.map_writer( CI.mvm, CI.scaled_boxes, CI.prefix+CI.mapname )


print ""
print "Computing induced homology..."

CI.run_homcubes( suffix='cub' )
CI.write_index_map( CI.prefix+CI.mapname+'-index' )


# for convenience in ipython
C = CI 

