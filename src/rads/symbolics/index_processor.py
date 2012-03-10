"""
index_processor.py

Author: Jesse Berwald

Opened: Feb. 27, 2012

NOT SURE IF THE INHERITANCE BELOW IS THE FINAL ROAD MAP. 
"""


class EdgeSet( object ):
    """
    Given s,t \in V(G), each edge set E \subset E(G) consists of all
    length k path from s -> t.

    An EdgeSet is a container for a collection of dictionaries keyed
    by edges. Dictionary values . Matrix product for resulting
    path
    """
    def __init__( self, region_s, region_t, path_length, **kwargs ):
        """
        Parameters
        -----------

        region_{s,t} : Neighborhood N_*, represented by node s or t in the map
        in regions graph object.

        path_length : The length of paths that we are considering.
        """
        self.s = region_s
        self.t = region_t
        self.length = path_length
        self.edges = [] # empty list of edges

    def __repr__( self ):
        s = "EdgeSet beginning at "+str( self.s )+\
            ", ending at "+str( self.t )+\
            ", with "+str(len( self.edges ))+\
            " edges."
        return s

    def __add__( self, ebunch ):
        """
        Overload the addition operator for adding edges
        """
        self.edges.append( ebunch )


class ProhibitedEdges( object ):
    """
    Container object for edge sets at homology level k.

    Given a max_path_length, K, the edge_sets attribute is a
    dictionary initialized with keys 0,...,K-1. Each value at path
    length 0 < j < K, contains list of EdgeSet's initialized for start
    and end nodes, s and t, resp, of the p=(s,...,t).
    """
    def __init__( self, max_path_length ):
        self.max = max_path_length
        self.edge_sets = {}
        # init lists to hold edges sets
        for i in range( self.max ):
            self.edge_sets.setdefault( i, {} )

    def __repr__( self ):
        return "Prohibited edges for paths up to length "+\
            str( self.max )

    def __getitem__( self, k ):
        return self.edge_sets[k]

    def add_edge_set( self, k, start, end ):
        """
        Append an empty EdgeSet to hold paths from start->end of
        length k.
        """
        self.edge_sets[k][(start,end)] =\
            EdgeSet(start,end,k) 

    def append_to_edge_set( self, k, start, end, edgeset ):
        """
        Append a new collection of edges to EdgeSet for endpoints
        (start,end).

        Parameters
        -----------

        k : length of path

        start : start of the path

        end : end of the path

        edgeset : iterable collection of edge tuples
        
        """
        try:
            for edge in edgeset:
                self.edge_sets[k][(start,end)] + edge
        except KeyError:
            print "key", (start,end), "not found"

class IndexMapProcessor( object ):
    """
    Process and reduce the index map to a symbolic system with
    verified cycles.
    """
    def __init__( self, index_map, generator_map, adj_matrix, **kwargs ):
        """
        Class to perform operations on maps on generators. Reduces the
        map on generators to a smaller, verified set of a generator
        cycles.

        Parameters:
        -----------

        index_map : DiGraph object. Index map between generators

        generator_map : dictionary keyed by regions 1,...,m, with each
        value a list of generators withing regions r.
        """
        fargs = {'debug': False}
        fargs.update( kwargs )
        self.index_map = index_map
        self.generators = generator_map
        self.adj_matrix = adj_matrix
    
    
    def verify_cycles( self, index_map, generators, max_length=10 ):
        """
        Let N = cl(P\P'), where (P,P') is the index pair, and N=\cup
        N_i, where the N_i are disjoint regions in N, m=1,...,m. Let
        G=<V,E> represents admissible transitions between the regions
        N_i, where V={1,...,m} and (i,j) \in E if F(N_i) \cap N_j \ne
        \emptyset.
        
        We begin with the (sub)graph computed in
        shift_equivalent(). This allows one to produce a blockmodel
        graph that groups generators by region, and edges by
        admissible transition between region, G =
        self.map_on_regions. Some regions will have multiple
        generators.

        A cycle c is verified whenever tr(f_{Pk}^{c}) \ne 0. That is,
        we verify that the Conley index is nontrivial, meaning that
        the invariant set corresponding to the trajectory prescribed
        by c is nonempty. Thus, the preimage of the iterinary
        function, \rho^{-1}(c), is nonempty, giving a semi-conjugacy
        of the map f with the subshift of finite type, \sigma, defined
        on the symbol space of m symbols and transition matrix T==G.

        The index map is represented by an n x n matrix (n>=m), with
        the submatrix of size nj X ni representing the linear map on
        generators from the Ni to Nj. Thus, f_{Pk}^{c} boils down to a
        product of matrices. To verify nonemptiness of the invariant
        set one then checks that the trace of the product of these
        matrices is nonzero.

        Procedure:

            1. Partition generators by regions ('blocks')

            2. Contract==condense index map onto those
            regions. Maintains directed graph structure, but only
            between the coarser regions.

            3. Verify cycles: Checks cycles of length <=
            max_path_length and flags those edges/edge sets that are
            prohibited.

            4. more...

        Parameters:
        -----------

        index_map : an IndexMap object with index_map and adj_matrix
        attributes.

        max_path_length : (optional, but default set very low)
        """
        # invert the generator map so we can get the blocks below
        inv_map = convert.invert_dictionary( self.generators )
        if self.debug: print "inv gens", inv_gens
        # list of lists. blocks = [blocks[i]] =[[gen0,gen1],[gen2],...]
        blocks = list( chain(inv_map.itervalues()) )
        if self.debug: print "blocks", blocks
        # condensed matrix -- collapsed onto regions
        self.map_on_regions = algorithms.blockmodel( self.index_map, blocks )
        self.verified_cycles = algorithms.verified_cycles( self.index_map,
                                                           self.adj_matrix,
                                                           dmax_path_length )
        


    def verified_cycles( self map_on_regions, index_map, adj_matrix, max_path_length ):
	"""
	Induction on the path length.

	map_on_regions
	"""
	# base case. Build collection of prohibited edges 
        pass
    
