import sys
sys.path.append( '/Users/jberwald/github/local/caja-matematica/rads/src/rads' )

from graphs.digraph import DiGraph
#from symbolics import Path
from walk_library import Walk, UnverifiedLibrary
from numpy import zeros

class IndexMap(DiGraph):
    def __init__(self, generators=None, regions=None, transitions=None, debug=False):
        """
        Inputs:
        
            generators -- a matix representing mapping of generators in
                rows onto generators in columns.
                
            regions -- a dictionary of lists representing generators
            per region.

            transitions -- An adjacency matrix representing the
            transitions between regions in phase space.
            
            debug (optional) = a boolean representing if debug
                statements should be printed
        """
        self.generators = generators
        self.regions = regions
        self.transitions = DiGraph()
        self.transitions.from_numpy_matrix( transitions )
        self.library = UnverifiedLibrary()
        self.debug = debug

        # Graph that will store the backend reprsentation 
        DiGraph.__init__(self)

        # Utility functions for packing and unpacking edge information into an integer
        self.regioncount = len( self.regions )
        self.edge2hash = lambda start, end: start * self.regioncount + end
        self.hash2edge = lambda hashed: hashed / self.regioncount, #hashed%regioncount

        # source and sink along one-step paths phase space (maps between regions)
        for s, t in self.transitions.edges():
            # generators supported on each region
            r2g = ( self.regions[ s ], self.regions[ t ] )
            matidx = ( r2g[0][0],r2g[0][-1]+1,
                       r2g[1][0],r2g[1][-1]+1 )
            edge = Walk(start=s,
                        end=t,
                        edges=frozenset( [(s,t)] ),#frozenset( [self.edge2hash( s, t )] ),
                        matrix=self.generators[ matidx[2]:matidx[3],
                                                matidx[0]:matidx[1] ],
                        length=1
                        )
            if debug:
                print "Walk from ", s, " --> ", t
                print "  region to generator map:", r2g[0], r2g[1]
                print "  corresponding edge matrix ('color'):\n", edge.matrix
                print "  mat.shape:", edge.matrix.shape
                print ""
            if not edge.zero():
                self.graph.add_edge( s, t, edge=edge )
                self.library.add( edge )
            


if __name__ == "__main__":

    import numpy
    from index_map_processor import IndexMapProcessor
    
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

    debug = True
    IM = IndexMap( generators, regions, map_on_regions, debug=debug )
    IP = IndexMapProcessor( IM, debug=debug )
    IP.find_bad_edge_sets( 2 )
                                        
                                         
