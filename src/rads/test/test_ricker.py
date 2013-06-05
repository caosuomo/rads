#!/usr/bin/python

import numpy as np
from rads.enclosure import CombEnc,Tree
<<<<<<< HEAD
from rads.maps.ricker import RickerMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx

depth = 5

# main bounding box
box = np.array([[0.0,0],[4,4]])
=======
from rads.maps.ricker import RickerMapper # for some reason must have .ricker (??)
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx, utils

depth = 6
rval = 12

# main root box -- make sure that dimension align with that in
# ricker_cpp.cpp. Eg. if Mapper( dim, 'ricker' ), then system
# dimension is dim => if dim=2, just two regions. But for larger
# simulations with dispersal > 0, assumption is that the region is
# square, so dim must be a square.
#box = np.array([[0.],[4]])
box = np.array( [[0.1,0.1],[6,6]] )

print "Initial bounding box"
print box
>>>>>>> adding-capd

# our tree, mapper, enclosure
tree = Tree(box,full=True)
m = RickerMapper()
<<<<<<< HEAD
=======

# change params -- leave d alone for now
p = m.get_params()
p['r'][:] = rval
m.set_params( p )

>>>>>>> adding-capd
ce = CombEnc(tree,m)

for d in range(depth):
	print 'at depth', d
	ce.tree.subdivide()
	print 'subdivided:', ce.tree.size, 'boxes'
	ce.update()
	print 'enclosure updated'
	I = graph_mis(ce.mvm)
	print 'len(I) = ', len(I) # fix the extra stuff returned by graph_mis
<<<<<<< HEAD
	# now remove all boxes not in I (the maximal invariant set)
	
	ce.tree.remove(list(set(range(ce.tree.size))-set(I[0])))

# now display the tree!
boxes = ce.tree.boxes()
gfx.show_uboxes(boxes, col='c', ecol='b')

=======

	# now remove all boxes not in I (the maximal invariant set)
	nodes = set(range(ce.tree.size))
	ce.tree.remove(list(nodes-set(I)))

# remove nodes not in I from the mvm
ce.mvm.remove_nodes_from( list(nodes-set(I)) )
boxes = ce.tree.boxes()

# now display the tree!
if box.shape[1] == 2:
	gfx.show_uboxes(boxes, col='c', ecol='b')

P = ce.mvm.to_numpy_matrix()
p2 = P*P
p2diag = p2.diagonal()
w = np.where( p2diag != 0 )[1] 
fp = boxes.corners[ w ][0]
fig = gfx.show_uboxes_corners( fp, boxes.width )
fig.savefig( 'ricker_per2_r'+str( int(rval) ) + '_depth' + str( depth ) + '.png' )


utils.array2chomp( fp,
		   'ricker_per2_r'+str( int(rval) ) + '_depth' + str( depth ) + '.cub' )


# check homology of the 
# utils.array2chomp( boxes.corners,
# 		   'ricker_r'+str( int(rval) ) + '_depth' + str( depth ) + '.cub' )
		   
		
>>>>>>> adding-capd
