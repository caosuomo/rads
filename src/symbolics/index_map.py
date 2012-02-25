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
from graphs import digraph
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
        IP.IndexMapProcessor.__init__( self )
        fargs = {'debug': False,
                 'indexmap': None,
                 'genmat': None
                 }
        fargs.update( kwargs )

        self.debug = fargs['debug']

        # load the index map graph and initialize the graph
        if fargs['indexmap']:
            self.adj_matrix = convert.load_index_map( fargs['indexmap'] )
            self.graph = digraph.DiGraph( data=self.adj_matrix )

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

            3. Trim self.graph to include only those nodes in
                     V2 = V\(V1 \cup V3)

            4. The nodes in V1 \cup V3 are stored in the attrribute
            'non_mis_nodes'.

        args:
        -----

        copy : store the shift equivalent map in a new graph
        """
        

    def verify_cycles( self, max_length=10 ):
        """
        """
        pass


