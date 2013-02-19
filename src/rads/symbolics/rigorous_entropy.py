"""
index_map_processor.py

Opened: ~ Jun. 13, 2012

Authors: Jakub Gedeon and Jesse Berwald

Processes objects contained in walk_library.py for calculation of the
entropy of system semi-conjugate to the symbolic system represented by
matrix/graph in transitions (see IndexMap).
"""
from rads.misc import utils
from collections import defaultdict
from rads.graphs import DiGraph

class RigorousEntropy( object ):
    """
    Entry point for consfor working with multiple IMP's if/when there are multiple
    disjoint strongly connected components in the map among regions in
    phase space.

    Example:

    index_map = numpy.matrix( [[0,0,1,1,0],
                                [0,0,0,0,1],
                                [0,1,0,0,0],
                                [0,0,1,1,-1],
                                [0,0,-1,-1,1]]
                               ).T

    region2gen = { 0 : [0,1],
                   1 : [2],
                   2 : [3,4]
                   }

    
    From these we can construct a map on symbols (i.e. regions):

     symbols = numpy.matrix( [[0,1,1],
                             [1,0,0],
                             [0,1,1]]
                            ),

    where the region2gen.keys() = indices of symbols.
    """
    def __init__( self, verbose=False, debug=False ):
        """
        Initialize the object and check for recurrent graph structures. 

        index_map : Map on homology matrix. numpy array (2d)
        representing the map on homology (generators) induced by the
        map f : X --> X on phase space.
        
        generator_map : dictionary -- Map that associates regions in
        phase to generators. Indices in index_map must align with
        """
        self.verbose = verbose
        self.debug = debug

        # To hold disjoint regions and maximum 
        self.phase_space = []
        
    def load_from_file( self, index_fname, generators_fname, dtype=int ):

        ## try to load numpy, fall through to loadtxt
        self.index_fname = index_fname
        self.generators_fname = generators_fname
        try:
            self.index_map = utils.load_numpy( index_fname )
        except IOError:
            self.index_map = utils.loadtxt( index_fname, dtype=dtype )

        # generator map should be a dictionary, so we expect it to be
        # serialized as a pickle file
        try:
            self.region2gen = utils.load_generators( generators_fname )
        except (NameError,IOError):
            print "Problem loading generator file, ", generators_fname
            raise

    def load_from_matlab_files( self, index_fname=None,
                                generators_fname=None,
                                matname=None):
        """
        index_fname : path to matrix of map on generators (may have to
        provide internal name of the matlab matrix to extract it from
        meta data. See example below. )

        generators_fname : path to the region --> geneators map.

        Example:
        --------

           hom_matrix = utils.load_matlab_matrix( 'henon_index.mat', 'hom_matrix' )
           region2gen = utils.convert_matlab_gens( 'henon_gens.mat' )
           map_on_regions = utils.index_map_to_region_map( hom_matrix, region2gen, shift=-1)
        """
        self.index_fname = index_fname
        self.generators_fname = generators_fname
        self.index_map = utils.load_matlab_matrix( self.index_fname, matname )
        self.region2gen = utils.convert_matlab_gens( self.generators_fname )
        
        self.map_on_regions = utils.index_map_to_region_map( self.index_map,
                                                             self.region2gen,
                                                             shift=-1)
            
    def construct_regions( self ):
        """
        """
        self.map_on_regions = utils.index_map_to_region_map( self.index_map,
                                                             self.region2gen,
                                                             shift=-1)

        scc_list, scc_components, recurrent_regions = algorithms.graph_mis( self.map_on_regions )
        self.recurrent_subgraphs = []
        for n in nbunch:
            # ignore disjoint regions that have self-loops => no
            # entropy
            if len( n ) == 1:
                continue
            G = DiGraph()
            G.graph = self.map_on_regions.graph.subgraph( n )
            self.recurrent_subgraphs.append( G )

        # construct IndexMapProcessor for each disjoint region
        # asihn entropies  of -1 to each recurrent region. 
        for region in self.recurrent_subgraphs:
            self.phase_space.append( ( IndexMapProcessor( self.index_map,
                                                          self.region2gen,
                                                          region,
                                                          debug=self.debug,
                                                          verbose=self.verbose ),
                                      -1  # initial entropy for region
                                       ) )
        


    def analyze_regions_parallel( self ):
        pass

    def compute_entropy( self, max_path_length=16 ):
        """
        Loop through regions in series and compute entropy
        """
        for region in self.phase_space.keys():
            R = self.phase_space[ region ]
            R.find_bad_edge_sets( max_path_length )
            R.cut_bad_edge_sets()
            self.entropy[ region ] = R.entropy

    def maximum_entropy( self ):
        """
        Returns the maximum entropy over all recurrent regions in
        phase space.
        """
        entropies = self.entropy.values()
        self.max_entropy = max( entropies )
        return self.max_entropy
            
    # def draw( self, region=None ):
    #     """
    #     """
    #     print ""
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

    #     plt.show()

    # IP = IndexMapProcessor( IM, debug=debug )

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


if __name__ == "__main__":

    # test various methods
    
    # TEXT and NPY
    fname_npy = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.npy'
    #fname_txt = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/test_array.txt'

    reg_fname =  '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.pkl'
     
    # MAT
    fname_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.mat'
    reg_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.mat'
    matname = 'hom_matrix'

    re1 = RigorousEntropy()
    re2 = RigorousEntropy()
    
    re1.load_from_file( fname_npy, reg_fname )
    re2.load_from_matlab_files( fname_mat, reg_mat, matname )

    
