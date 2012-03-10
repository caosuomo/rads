"""
index_map.py

Author: Jesse Berwald

Opened: Feb. 23, 2012

High-level interface to create and modify an index map on
homology.

Usage:
------

   In [1]: import index_map as im
   Out [1]: IM = im.IndexMap( data='my_index_map.npy' )

   In [2]: IM.shift_equivalence()

   In [3]: IM.....

   In [4]: IM.compute_entropy()
"""
from rads.graphs import digraph
from rads.graphs import algorithms
import convert
import index_processor as IP

class IndexMap( IP.IndexMapProcessor ):
    """
    Base class for graphical index map computations.

    Inherits from IP.IndexMapProcessor.

    Usage:
    ------
    
    Construction of an IndexMap representing the map on homology for a
    kixed k

         f: H_{k}(P,P') -> H_{k}(P,P')

    for the index pair (P,P'):

    Two objects are necessary, a directed graph (equivalently an
    adjacency matrix) and a representation of generators by disjoint
    regions within the index pair.

    Initialization:
    ---------------

    1) One can create an empty IndexMap. Later, populate it with nodes
    (generators) and edges (maps between generators). A map from
    disjoint regions in the invariant set to the generators within
    those regions must also be provided.

    2) Using the kwarg 'indexmap', the user can provide an adjacency
    matrix defining the map on generators the graph structure.

    2') The kwarg 'genmap' provides a dictionary keyed by region that
    maps region N_i to the generators on that region. 
    
    args:
    ----

    indexmap : adjacency matrix (array or path to file)
               Represents map on homology.

    genmap : map from disjoint regions in index pair to the generators
             that live on those regions.

    """
    def __init__( self, **kwargs ):
        # Inherit from IndexMapProcessor: functions for reducing the
        # number of symbols, verifying cycles, etc.
        fargs = {'debug': False,
                 'indexmap': None,
                 'genmap': None
                 }
        fargs.update( kwargs )

        self.debug = fargs['debug']

        # load the index map graph and initialize the graph
        if fargs['indexmap']:
            self.adj_matrix = convert.load_index_map( fargs['indexmap'] )
            self.index_map = digraph.DiGraph( data=self.adj_matrix )
        else:
            self.adj_matrix = None
            self.index_map = digraph.DiGraph()
        if fargs['genmap']:
            # convert from matlab if necessary; otherwise, pass
            # dictionary back unchanged.
            self.generators = convert.convert_generators( fargs['genmap'] )
        else:
            self.generators = None
        # Now that we have the index map and generators, initialize
        # the processor
        IP.IndexMapProcessor.__init__( self, self.index_map,
                                       self.generators, **fargs )

        # initially flag all edges as not prohibited
        self.flag_edges_from( label='prohibited', flag=False )
 
    def shift_equivalent_map( self ):
        """
        Graphical representation of the shift equivalence proved in
        Theorem 3.1 and Lemmas 3.2-3.4.

        The required sets of nodes V1, V2, and V3 are computed as
        follows:
            Given G = (V,E), with V = {V1,V2,V3},

            1. Find strongly connected components (scc) of G. Labeled
            by nodes V2.

            2. Perform a forward depth-first search; then perform
            backward depth-first search by reversing the orientation
            on the edges. This locates generators that are not
            connected to cycles. Labeled by nodes V1 \cup V3.

            3. Trim self.index_map to include only those nodes in
                     V2 = V\(V1 \cup V3)

            4. The nodes in V1 \cup V3 are stored in the attrribute
            'non_mis_nodes'.
        """
        # determine the maximal invariant set of generators in
        # self.index_map (1 & 2 above)
        mis_nodes = algorithms.graph_mis( self.index_map )
        nbunch = set( self.index_map.nodes() )
        # compute V2
        self.non_mis_nodes = nbunch.difference( mis_nodes )
        # remove nodes to form subgraph of map on generators
        # containing only nodes that are part of the maximally
        # invariant set.
        self.index_map.remove_nodes_from( self.non_mis_nodes )


        

    def compute_entropy( self, max_path_length=20 ):
        """
        """
        # from IndexMapProcessor        
        verified_cycles = self.verify_cycles( max_path_length )
        

