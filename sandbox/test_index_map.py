import numpy
#from rads.symbolics.index_map_new import IndexMap
from rads.symbolics.index_map_processor import IndexMapProcessor,IndexMap
from rads.misc import utils
import matplotlib.pyplot as plt


if 0:
      # column mapping: col idx --> row idx 
        generators = numpy.matrix( [[0,0,0,1,0,0,0,0],
                                    [0,0,0,0,1,0,0,0],
                                    [0,0,0,0,1,0,0,0],
                                    [0,0,0,0,0,1,0,0],
                                    [-1,-1,0,0,0,0,0,0],
                                    [0,0,0,0,0,0,-1,1],
                                    [0,0,1,0,0,0,0,0],
                                    [-1,0,0,0,0,0,0,0]]
                                   ).T

        # modified version 
        # generators = numpy.matrix( [[0,0,0,1,0,0,0,0],
        #                             [0,0,0,0,0,0,0,0],
        #                             [0,0,0,0,1,0,0,0],
        #                             [0,0,0,0,0,1,0,0],
        #                             [-1,-1,0,0,0,0,0,0],
        #                             [0,0,0,0,0,0,-1,1],
        #                             [0,0,0,0,0,0,0,0],
        #                             [-1,0,0,0,0,0,0,0]]
        #                            ).T

        regions = { 0 : [0],
                    1 : [1,2],
                    2 : [3],
                    3 : [4],
                    4 : [5],
                    5 : [6,7]
                    }

        map_on_regions = numpy.matrix( [[0,0,1,0,0,0],
                                        [0,0,0,1,0,0],
                                        [0,0,0,0,1,0],
                                        [1,1,0,0,0,0],
                                        [0,0,0,0,0,1],
                                        [1,1,0,0,0,0]]
                                       )
if 1:

    hom_matrix = numpy.matrix( [[0,0,1,1,0],
                                [0,0,0,0,1],
                                [0,1,0,0,0],
                                [0,0,1,1,-1],
                                [0,0,-1,-1,1]]
                               ).T

    # A <--> 0, B <--> 1, etc.
    region2gen = { 0 : [0,1],
                   1 : [2],
                   2 : [3,4]
                   }

    map_on_regions = numpy.matrix( [[0,1,1],
                                    [1,0,0],
                                    [0,1,1]]
                                   )

debug = False
IM = IndexMap( hom_matrix, region2gen, map_on_regions, debug=debug )
IP = IndexMapProcessor( IM, debug=debug )
IP.find_bad_edge_sets( 4 )  # argument == max path length
IP.cut_bad_edge_sets()
print "Best entropy after cutting edges", IP.entropy
print "  Original system (as a matrix):"
print IP.map.to_numpy_matrix()
print ""
print "  Semi-conjugate system with best entropy:"
print IP.semi_conjugate_subshift.to_numpy_matrix()
print ""
print "  Drawing both transition graphs..."

fig1 = plt.figure()
ax1 = fig1.gca()
ax1.set_title( "Original transition map on regions (subshift)" )
IP.map.draw( ax=ax1 )


fig2 = plt.figure()
ax2 = fig2.gca()
ax2.set_title( "Best subshift found after trimming edges" )
IP.semi_conjugate_subshift.draw( ax=ax2 )

plt.show()
