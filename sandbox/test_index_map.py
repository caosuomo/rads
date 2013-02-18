import numpy
#from rads.symbolics.index_map_new import IndexMap
from rads.symbolics.index_map_processor import IndexMapProcessor,IndexMap
from rads.graphs import DiGraph
from rads.misc import utils
from rads.graphs import algorithms
import matplotlib.pyplot as plt
import networkx as nx


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
if 0:

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

    symbols = numpy.matrix( [[0,1,1],
                             [1,0,0],
                             [0,1,1]]
                            )
    map_on_regions = DiGraph()
    map_on_regions.from_numpy_matrix( symbols )

if 0:

    hom_matrix = utils.load_matlab_matrix( 'henon_index.mat', 'hom_matrix' )
    region2gen = utils.convert_matlab_gens( 'henon_gens.mat' )
    map_on_regions = utils.index_map_to_region_map( hom_matrix, region2gen, shift=-1)

if 1:
    hom_matrix = utils.load_matlab_matrix( 'leslie_index.mat', 'hom_matrix' )
    region2gen = utils.convert_matlab_gens( 'leslie_gens.mat' )
    map_on_regions = utils.index_map_to_region_map( hom_matrix, region2gen, shift=-1)


    ########################
    #  
    # APPEARS THAT BAD EDGE SETS ARE BEING THROWN INTO ONE (1) SET. CHECK THIS!!
    #
    ########################

debug = False
verbose = False

scc_list, scc_components, recurrent_regions = algorithms.graph_mis( map_on_regions )
# separate nodes by recurrent component
nbunch = [ scc_components[i] for i in recurrent_regions ]
recurrent_subgraphs = []
for n in nbunch:
    # ignore self-loops, no entropy
    if len( n ) == 1:
        continue
    G = DiGraph()
    G.graph = map_on_regions.graph.subgraph( n )
    recurrent_subgraphs.append( G )

# compute entropy for each scc in the symbolic system
all_regions = []
for scc in recurrent_subgraphs:
    all_regions.append( IndexMapProcessor( hom_matrix, region2gen,
                                           scc, debug=debug, verbose=verbose ) )

# PARALLELIZE ME!
for job in all_regions:
    job.find_bad_edge_sets( 4 )  # argument == max path length
    job.cut_bad_edge_sets()

# print the best entropy for each SCC
for imp in all_regions:
    print imp.__repr__()
    print "  Entropy for this region", imp.entropy
    print ""

   #  print ""
#     print "  Drawing both transition graphs..."

#     fig1 = plt.figure()
#     ax1 = fig1.gca()
#     ax1.set_title( "Original transition map on regions (subshift)" )
#     pos1 = utils.nx.graphviz_layout( imp.map.graph )
#     imp.unverified_symbolic_system.draw( ax=ax1, pos=pos1 )


#     fig2 = plt.figure()
#     ax2 = fig2.gca()
#     ax2.set_title( "Best subshift found after trimming edges" )
#     pos2 = utils.nx.graphviz_layout( imp.semi_conjugate_subshift.graph )
#     imp.verified_symbolic_system.draw( ax=ax2, pos=pos2 )

# plt.show()

#IP = IndexMapProcessor( IM, debug=debug )

# nbunch = [ scc_components[i] for i in recurrent_regions[:5] ] # just plot 5 SCC's
# color = ['r', 'g', 'b', 'm', 'c']
# for i,n in enumerate(nbunch):
#     fig=plt.figure( i )
#     ax = fig.gca()
#     sg = IP.unverified_symbolic_system.graph.subgraph( nbunch=n )
#     pos = nx.graphviz_layout( sg )
#     print "s[",i,"] =", scc_components[i]
#     nx.draw_networkx(  sg, pos=pos, node_color=color[i], ax=ax )
#     fig.savefig( './figures/henon_symbol_map_scc'+str(i)+'.png' )
#     #    IP.unverified_symbolic_system.draw( nodelist=n, node_color=color[i] )

