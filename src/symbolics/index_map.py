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

    def shift_equivalence( self ):
        """
        The index map, f, 
        """
        pass

    def 



