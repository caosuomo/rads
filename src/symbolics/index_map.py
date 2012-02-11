"""
Base class for holding index map information and subroutines. Mostly a convenience wrapper with (probably) some conversion methods.

Parameters
---------

idx_map : 
"""
import networkx as nx


class Symbol_Graph( nx.DiGraph ):
    """
    Base class for 
    """

class Index_Map( nx.DiGraph ):
    """
    A representation of the map induced on homology, that is

    f_{P*} : H_{*}(P_1,P_0) ---> H_{*}(P_1,P_0),

    where (P_1,P_0) form an index pair. 

    Parameters
    ---------

    fname : Path to file containing index map definition.
    """
    def __init__( self, **args ):

        fargs = { 'matfile' : None,
                 'pklfile' : None,
                 'npyfile' : None,
                 'graph' : None
                 }
        fargs.update( args )

        if fargs['graph']:
            self.graph = fargs['graph']     
        else:
            self.graph = nx.DiGraph()
            # if numpy array
            if fargs['npyfile']:
                ebunch = np.load( fargs['npyfile'] )
            elif fargs['pklfile']:
                print "not implemented"
            elif fargs['matfile']:
                self.mat = self._loadmat( fargs['matfile'] )
                self.mat2graph()

    def __lt__( self, idx_map ):
        """
        Convenience overload for adding an edge to self.graph

        Parameter
        --------

        idx_map : (region1, region2, map)
        """
        try:
            self.add_edge( idx_map[0], idx_map[1], idx_map[2] )
        except IndexError:
            print "< failed"


    def _loadmat( self, fname ):
        try:
            from scipy.io import loadmat
        except ImportError:
            print "scipy.io.loadmat not found"
            raise
        idx_map = loadmat( fname, mat_dtype=True )
        keys = idx_map.keys()
        # peel of matrix name. If we sort the keys, the matrix
        # name will float to the end
        keys.sort()
        return idx_map[ keys[-1] ]

    def mat2graph( self ):
        """
        Convert a sparse matrix loaded from MatLab (R) format to a
        directed graph.

        Note: Indices automatically shifted to 0-based by loadmat().
        """
        # nonzero indices
        nz = self.mat.nonzero()
        nnz = zip( nz[0], nz[1] )
        for u,v in nnz:
            self.add_edge( u, v, orientation=int( self.mat[u,v] ) )

    def edges( self, data=True ):
        return self.graph.edges( data=data )


    def add_edge( self, u, v, orientation=None ):
        """
        A node is a region in space. The index map specifies how
        generators are mapped between such nodes. For instance, the map 

        u -> v

        implies that the generator at region u is mapped onto the
        generator v at region 1. The optional orientation argument
        records the 'weight' on the edge by specifying the orientation
        of the map (usually +/- 1).
        """
        self.graph.add_edge( u, v, weight=orientation )




if __name__ == "__main__":

    import numpy as np

    d = {(38,1) :       -1,
         (68,2) :      -1,
         (69,2) :      -1
         }

    matfile = '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/hom_map.mat'
    genfile =  '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/gens.mat'

    IM = Index_Map( matfile=matfile )

