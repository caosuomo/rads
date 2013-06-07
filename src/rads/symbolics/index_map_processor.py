from rads.symbolics.walk_library import UnverifiedLibrary, BadLibrary, Walk
from rads.graphs.digraph import DiGraph
from collections import deque
import numpy as np
from math import log
from itertools import product

"""
index_map_processor.py

Opened: ~ June 13, 2012

Authors: Jakub Gedeon and Jesse Berwald

Processes objects contained in walk_library.py for calculation of the
entropy of system semi-conjugate to the symbolic system represented by
matrix/graph in transitions (see IndexMap).
"""

class IndexMap( DiGraph ):
    
    def __init__(self,
                 generators=None,
                 regions=None,
                 transitions=None,
                 debug=False,
                 verbose=False):
        """
        Inputs:
        
            generators -- a matix representing mapping of generators in
                rows onto generators in columns.
                
            regions -- a dictionary of lists representing generators
            per region.

            transitions -- An adjacency matrix representing the
            transitions between regions in phase space.

            matlab_shift -- Account for 1-based indexing in Matlab. 
            
            debug (optional) = a boolean representing if debug
                statements should be printed
        """
        
        # Graph that will store the backend reprsentation 
        DiGraph.__init__(self)
        
        self.generators = generators
        self.regions = regions
        self.debug = debug
        self.verbose = verbose
        
        # adjacency matrix will have '__abs__' attribute
        if hasattr( transitions, '__abs__' ):
            self.unverified_symbolic_system = DiGraph()
            self.unverified_symbolic_system.from_numpy_matrix( transitions )
        # otherwise, assume it's already a Digraph (no check done!)
        else:
            self.unverified_symbolic_system = transitions

            #self.library = UnverifiedLibrary()

        # Utility functions for packing and unpacking edge information into an integer
        self.regioncount = len( self.regions )
        self.edge2hash = lambda start, end: start * self.regioncount + end
        self.hash2edge = lambda hashed: hashed / self.regioncount, #hashed%regioncount            

class IndexMapProcessor( IndexMap ):
    """
    Processes a given index map of generators.
    """
    def __init__( self,
                  index_map=None,
                  reg2gen=None,
                  symbolic_system=None,
                  debug=False,
                  verbose=False,
                  check_trace=False ):
        """
        Objects and methods for verifying paths in a symbolic system
        based on the given index map. Works whether symbolic system
        contains one or many disjoint strongly connected components.

        index_map -- numpy matrix of map on generators on disjoint
        regions of an attactor in phase space. Output from CHomP or
        other homology software.

        reg2gen -- a dictionary of lists representing generators
        per region.
        
        symbol_system -- An adjacency matrix representing the
        transitions between regions in phase space.

        debug -- turn on debugging messages (default = False)

        check_trace -- toggle whether to check the trace of matrix
        products resulting from cycles. (default=False)
        """
        IndexMap.__init__( self, index_map, reg2gen,
                           symbolic_system, debug, verbose )

        self.check_trace = check_trace
        
        # Stores all edge sets that need to have one edge cut
        self.bad_edges = BadLibrary( debug=debug )

        # source and sink along one-step paths phase space (maps
        # between regions). self.unverified_symbolic_system inherited
        # from IndexMap.
        if self.verbose:
            print "CONSTRUCTING INITIAL WALK LIBRARY FROM INDEX MAP"\
                "AND UNVERIFIED SYMBOLIC SYSTEM GRAPH:"
            print ""
        for s, t in self.unverified_symbolic_system.edges_iter():
            # generators supported on each region
            r2g = ( self.regions[ s ], self.regions[ t ] )
            matidx = ( r2g[0][0],r2g[0][-1]+1,
                       r2g[1][0],r2g[1][-1]+1
                       )
            edge = Walk( start=s,
                         end=t,
                         edges=frozenset( [(s,t)] ),
                         matrix=self.generators[ matidx[2]:matidx[3],
                                                 matidx[0]:matidx[1] ],
                         length=1
                        )
            if self.verbose:
                print "---------------------------------------------"
                print "Walk from ", s, " --> ", t
                print "  region to generator map:", r2g[0], r2g[1]
                print "  corresponding edge matrix:\n", edge.matrix
                print ""
            if not edge.zero():
                self.graph.add_edge( s, t, edge=edge )
                # self.library.add( edge )

        # Unravel the underlying index map (stored as a graph) into
        # list of 1-step walks.
        self.edge_walks = {}
        for start,end,attr in self.graph.edges_iter( data=True ):
            self.edge_walks[ (start,end) ] = attr["edge"]
        
        # Stores edges that need to be expanded, to search for more edges.
        # init with the 1-step walks above
        self.todo = deque( self.edge_walks.values() )
        
        # Stores all edges that are unverified
        # again, init with the 1-step walks
        self.unverified = UnverifiedLibrary( self.edge_walks.values() )

        # set initial entropy
        self.entropy = -1
        
        # best symbolic system found 
        self.verified_symbolic_system = None

    def __repr__( self ):
        s = "IndexMapProcessor on " +str( len( self.graph ) ) + " regions"
        return s

    def check_walk( self, walk ):
        """
        Check whether the matrix product is zero. If self.check_trace
        == True, also check zero trace condition if we pass the zero
        matrix condition.

        Returns True if either the matrix product is zero or the trace
        of a cycle is zero.
        """
        # If matrix product is zero matrix, always add to bad edge
        # set, so check first
        if walk.zero():
            return True
        # If we pass the zero matrix condition *and*
        # check_trace==True, then check if we're a cycle and if the
        # trace is zero.
        if self.check_trace:
            if walk.cycle() and walk.zero_trace():
                return True
        return False        

    def find_bad_edge_sets( self, maxLength ):
        """
        Version of Algorithm 6 in Day, Frongillo, Trevino.

        maxLength -- maximum length of the paths allowed in edge verifications.

        Overview of algorithm:
        ---------------------

        self.todo -- Holds most recent Walks to be extended and verified.

        self.unverified -- Holds _all_ unverified Walks. Extended
        paths from todo are checked against those in unverified to see
        if a reduction exists.
        """
        if self.debug:
            maxlen = 1
        try:
            path = self.todo[0]
        except IndexError:
            raise
        
        while path.length <= maxLength:
            # Grab the first walk to extend
            try:
                old = self.todo.popleft()
            # exhausted the todo list!! 
            except IndexError:
                return
            if self.debug:
                print "Extending:", old
                print "-------------------"
                print " matrix", old.matrix
                if old.length > maxlen:
                    maxlen = old.length
                    print ""
                    print "**** LENGTH ****", maxlen
                    print ""
                    
            # Now take every edge that comes out from the end of the old walk
            for succ in self.unverified_symbolic_system.graph.successors_iter( old.end ):
                next_step = self.edge_walks[ (old.end, succ) ]
                if self.debug:
                    print "Successor edge:", next_step
                    print " matrix"
                    print next_step.matrix
                # Create a new walk by appending the new edge
                new_walk = old + next_step

                if self.debug:
                    print "--> New walk:"""
                    print new_walk
                    print new_walk.matrix
                
                # Check if the walk has an edgeset that will already be cut...
                # If so, ignore it
                if new_walk.edges in self.bad_edges:
                    continue
                
                # Check if the walk is a zero matrix, or is a cycle with zero
                # trace. If so, add it to the bad edge list.
                if self.check_walk( new_walk ):
                    if self.debug:
                        print "------------------------------"
                        print "BADS", self.bad_edges.bads
                        print " ** Adding BAD edgeset **"
                        print new_walk.edges
                        print new_walk.matrix
                        print "------------------------------"
                    self.bad_edges.add( new_walk.edges )
                    continue

                # Let p = new_walk. If there exists
                # another walk, q, such that (i) p == s-u and q == s-u
                # (share same edges), (ii) |q| <= |p| (p is longer
                # than q), (iii) and q.matrix = a * p.matrix, then we
                # can ignore new_walk "by induction"
                if self.unverified.reduction_exists( new_walk, debug=self.debug ):
                    continue

                # We have not decided if this path is bad or good, so it has to
                # added back into the queue of edges to check
                self.todo.append( new_walk )
                self.unverified.add( new_walk )

                # might be a slicker way to grab next path for
                # while-loop condition, but this is pretty efficient
                # and will work
                try:
                    path = self.todo[0]
                # we've exhausted our todo list! woohoo!
                except IndexError:
                    return
                
                if self.debug:
                    print "\n\n"

    def cut_bad_edge_sets(self, edges=None, **kwargs):
        """
        Remove edges in bad_edges. Maximize entropy by checking all
        combinations of removed edges. We assume that there are less
        than ~10k bad edge sets so that an exhaustive search is still
        fast.
        """
        # for each path in collection self.bad_edges.bads, choose one
        # edge from each path and cut it.
        # all combinations of one edge from each path
        if edges:
            bads = edges
        else:
            bads = self.bad_edges.bads
       
        bad_combos = list( product( *bads ) )
        # product returns ((.),) format if only one bad edge set, so
        # take first entry of each if this is the case.
        if len( bads ) == 1:
            bad_combos = [ x[0] for x in bad_combos ]

        # cycle through combinations looking for cut combo that
        # maximizes entropy.
        if self.verbose:
            print "Searching for best combination of edges to cut..."
            print "  Collections (one from each bad path):", bad_combos
        for edge_combo in bad_combos:
            if self.verbose:
                print "  edges cut: ", edge_combo
            # symbolic system to check entropy (copy of full system
            # with 'combo' edges cut.            
            S = self._cut_edges( edge_combo )
            entropy = log_max_eigenvalue( S )
            if self.verbose:
                print "   --> resulting entropy: ", entropy
            if entropy > self.entropy:
                self.entropy = entropy
                # save the best semi-conjugate subshift found
                self.verified_symbolic_system = S

        if self.verbose:
            if self.entropy == -1 or self.entropy == 0:
                print "No positive entropy found in this region!"
                print ""
            

    def _cut_edges( self, edges ):
        """
        Remove allowable transitions (edges) from full transition
        graph on regions (self.unverified_symbolic_system)
        """
        # remove_edges_from operates in-place, so we must make a copy
        # of the full transition matrix. 
        T = self.unverified_symbolic_system.copy()
        try:
            T.remove_edges_from( edges )
        # occurs if bad_combos in cut_bad_edge_sets only contain one path
        except (TypeError,IndexError):
            T.remove_edge( edges[0], edges[1] )            
        return T

    ##################
    # End classes
    ##################    
        
def log_max_eigenvalue( S, base=None ):
    """
    Returns max( log( 0, || v || ) ), where v is the leading
    eigenvalue of M.

    M -- Transition graph or numpy matrix

    Note: log is base e by default 
    """
    # S is not an array, so it must be a DiGraph object (no type
    # checking past this).
    if not hasattr( S, '__abs__' ):
        M = S.to_numpy_matrix()
    # S is already a numpy matrix
    else:
        M = S
    # compute the eigenvalues, take |v| of each, and store the largest
    # in eigmax
    eigmax = np.absolute( np.linalg.eigvals( M ) ).max()
    if eigmax == 0:
        return 0
    else:
        if not base:
            return log( eigmax )
        else:
            return log( eigmax, base )
        

                
if __name__ == "__main__":

    import numpy
    from index_map_new import IndexMap
    
    generators = numpy.matrix( [[0,0,0,1,0,0,0,0],
                                [0,0,0,0,1,0,0,0],
                                [0,0,0,0,1,0,0,0],
                                [0,0,0,0,0,1,0,0],
                                [-1,-1,0,0,0,0,0,0],
                                [0,0,0,0,0,0,-1,1],
                                [0,0,1,0,0,0,0,0],
                                [-1,0,0,0,0,0,0,0]]
                               )
    
    regions = { 0 : [0],
                1 : [1,2],
                2 : [3],
                3 : [4],
                4 : [5],
                5 : [6,7]
                }

    adjmatrix = numpy.matrix( [[0,0,1,0,0,0],
                               [0,0,0,1,0,0],
                               [0,0,0,0,1,0],
                               [1,1,0,0,0,0],
                               [0,0,0,0,0,1],
                               [1,1,0,0,0,0]]
                              )
    
    IM = IndexMap( generators, regions, adjmatrix, debug=False )
