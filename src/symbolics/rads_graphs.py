"""
Base class for holding index map information and subroutines. Mostly a convenience wrapper with (probably) some conversion methods.

Parameters
---------

idx_map : 
"""
import networkx as nx
from collections import deque


class DiGraph( nx.DiGraph ):
    """
    Base class for graph algorithms. Inherit all of NetworkX DiGraph's
    methods.
    """
    def __init__( self, **args ):
        nx.DiGraph.__init__( self )
        
        fargs = { 'matfile' : None,
                 'pklfile' : None,
                 'npyfile' : None,
                 'name' : '',
                 'ebunch' : None
                 }
        fargs.update( args )
        
        # load edge set/index map from possible file
        if fargs['npyfile']:
            # ebunch must consist of a list of tuples of the form
            # (u,v,wt)
            # TODO: test me!!
            ebunch = np.load( fargs['npyfile'] )
            self.add_weighted_edges_from( ebunch )
        elif fargs['pklfile']:
            print "not implemented"
        elif fargs['matfile']:
            self.mat = self._loadmat( fargs['matfile'] )
            self.mat2graph()

    def __repr__( self ):
        return str("RADS DiGraph")
        
    def __lt__( self, idx_map ):
        """
        Convenience overload for adding an edge to self.

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

        TODO: test me!
        """
        # nonzero indices
        nz = self.mat.nonzero()
        nnz = zip( nz[0], nz[1] )
        ebunch = deque()
        for u,v in nnz:
            ebunch.append( (u,v,self.mat[u,v]) ) 
        self.add_weighted_edges_from( ebunch )


####################
# INDEX_MAP
####################
class Index_Map( DiGraph ):
    """
    A representation of the map induced on homology. That is,

    f_{P*} : H_{*}(P_1,P_0) ---> H_{*}(P_1,P_0),

    where (P_1,P_0) form an index pair. 

    Inherits from
    ----------

    DiGraph

    Parameters
    ---------

    See DiGraph class
    """
    def __init__( self, **args ):        
        fargs = {'graph' : None}
        fargs.update( args )

        # user-supplied NetworkX Digraph
        if fargs['graph']:
            DiGraph.__init__( )
            self.add_weighted_edges_from( fargs['graph'] )
        else:
            DiGraph.__init__( self,  **args )

    def __repr__( self ):
        return str( "Index_Map graph" )

    def 


# class Error( Exception ):
#     """
#     Base class for exceptions.
#     """
#     pass

# class DiGraphError( Error ):
#     """
#     Exception raised for errors in DiGraph.

    
#     """
#     def __init__( self, expression, message ):
#         self.expression = expression
#         self.message = message 
                



if __name__ == "__main__":

    import numpy as np

    d = {(38,1) :       -1,
         (68,2) :      -1,
         (69,2) :      -1
         }

    matfile = '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/hom_map.mat'
    genfile =  '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/gens.mat'

    IM = Index_Map( matfile=matfile )
    #Gens = Generators( graph=IM )

