"""
walk_library.py

Opened: Jun. 13, 2012

Author: Jakub Gedeon

Contains classes and functions for dealing with storing and handling a set of
walks through a graph where matricies are associated with each edge.

Summary:
-------

We work with a finite alphabet, A = {1,...,m}, which are the nodes of
the constructed graph. Allowable transitions i -> j, determined from
the underlying dynamics, define the edges, E. We can think of each
edge as colored. The coloring function associates to each edge e \in E
a color in B = {1,...,k},

c : E --> B.

The graph G is the triple < G, E, c >. In this way we create a
subshift of a colored graph, defined by

X_G = { (x_i)_{i \in N} : \forall n \in N, \exists path a \subset E
such that the coloring c(a) = [x] is allowable },

where c(a) = [x] refers to the coloring of the path (i.e. the matrix
product). We are constructing X_G based on a search for all [x].
"""
# External dependenices
import numpy as np
# Default python libraries
from collections import defaultdict, deque
from itertools import chain, ifilter

class BadLibrary( object ):
    """
    A library of edgesets that are bad (all parameters are of type frozenset())
    """
    def __init__(self, edgesets=[], debug=False):
        self.bads = []
        self.bads.extend( edgesets )

        self.debug = debug
    
    def __contains__(self, edgeset):
        """
        Returns true if the given edgeset is a superset of any set to be cut,
        which implies that the edgeset will be cut as well.
        """
        for bad in self.bads:
            if bad.issubset( edgeset ):
                return True
        return False

    def add(self, edgeset):
        """
        Adds the edgeset to the set of edges to be cut. Filter
        according to whether the new edgeset is a superset of an
        edgeset already in bads.
        """
        if self.debug:
            print "*****"
            print "bads", self.bads
            print "edgeset", edgeset
            print "*****"
            print ""
        self.bads = filter( lambda bad: bad.issuperset( edgeset ), self.bads )
        # self.bads = filter( lambda bad: not bad >= edgeset, self.bads )
        self.bads.append( edgeset )

class UnverifiedLibrary( object ):
    """
    A library of walks for fast lookup and indexing, for walks
    that have not yet been exhaustively searched.
    """
    def __init__(self, walks=[]):
        # Allows fast search for items given a start and end, and edgeset
        self.count = 0
        self.start_end_dict = defaultdict( lambda: defaultdict( lambda: defaultdict( list ) ) )
        for walk in walks:
            self.add(walk)
        
    
    def add(self, walk):
        """ Add a Walk to the library """ 
        self.start_end_dict[walk.start][walk.end][walk.edges] = walk
        self.count += 1

    def reduction_exists( self, walk, debug=False ):
        """
        If an s-u path P results in a matrix product M, and smaller s-u
        path Q results in cM for some nonzero scalar c, and Q uses only
        edges from P, then we can ignore P 'by induction'.

        s -- walk.start

        u -- walk.end

        M -- walk.matrix
        """
        for other in self.start_end_dict[walk.start][walk.end].itervalues():
            if debug:
                print "IN REDUCTION", other
            if other.edges.issubset( walk.edges ) and walk.is_multiple( other ):
                if debug:
                    print ""
                    print "REDUCTION!"
                    print ""
                    print "other", other.edges
                    print "walk", walk.edges
                    print ""
                return True
            else:
                return False
            
        
class Walk( object ):
    """
    All the information needed for symbolics calculation about a walk
    """
    def __init__(self, start, end, edges, matrix, length):
        """
        Parameters:
            start = start node
            end = end node
            edges = set object containing the edges
            matrix = a numpy matrix of some sort, representing generator translation
            length = integer that is the edge length
        """
        self.start = start
        self.end = end
        self.edges = frozenset( edges )
        self.matrix = matrix
        self.length = length

    def __add__(self, other):
        """
        When two walks are added togeather, the new walk is formed by connecting
        the end of the first to the start of the second, and then multiplying the
        submatrices
        """
        assert self.end == other.start, \
                "Cannot add walks a+b if a.end!=b.start"
        return Walk( self.start, other.end,
                     self.edges.union( other.edges ),
                     other.matrix * self.matrix,
                     self.length + other.length )

    def __iter__( self ):
        return self.edges

    def __repr__( self ):
        s = "Walk(start="+str( self.start )+\
            ", end="+str( self.end )+\
            ", len(edges)="+str( self.edges )+\
            ", length="+str(self.length)+")"
        return s

    def __eq__( self, other ):
        """
        Compare two walks.
        """
        return self.__dict__ == other.__dict__

    def is_multiple( self, other ):
        """
        Returns A = c*B, where A = self.matrix, B = other.matrix.
        """
        A = self.matrix
        B = other.matrix
        w0, w1 = np.where( A != 0 )
        try:
            c =  float( A[w0[0,0],w1[0,0]] ) / float( B[w0[0,0],w1[0,0]] )
        except ZeroDivisionError:
            return False
        if np.array_equal( A, c*B):
            return True
        else:
            return False           
        
    def zero_trace(self):
        """ Test if walk has a zero trace """
        return np.trace(self.matrix)==0

    def cycle(self):
        """ Test if a walk is a cycle """
        return self.start==self.end

    def zero(self):
        """ Test if the matrix of the walk is a zero matrix
        These walks are generally considered useless """
        return np.all( self.matrix == 0 )

if __name__ == "__main__":

    import sys
    sys.path.append( '/Users/jberwald/github/local/caja-matematica/rads/src/rads' )
    from graphs import DiGraph
    import numpy
    
    # sample data for unit tests
    # See Figure one in "Algorithms for rigorous entropy bounds"
    # edges = [ ('A','C'), ('C','E'), ('E','F'), ('F','B'),
    #          ('B','D'), ('D','B'), ('D','A'), ('F','A') ]
   
    generators = numpy.matrix( [[0,0,0,1,0,0,0,0],
                                [0,0,0,0,1,0,0,0],
                                [0,0,0,0,1,0,0,0],
                                [0,0,0,0,0,1,0,0],
                                [-1,-1,0,0,0,0,0,0],
                                [0,0,0,0,0,0,-1,1],
                                [0,0,1,0,0,0,0,0],
                                [-1,0,0,0,0,0,0,0]]
                               ).T
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
    
    P = DiGraph()
    P.from_numpy_matrix( adjmatrix )

    G = DiGraph( )
    G.from_numpy_matrix( generators )

    edgeset = defaultdict( list )

    # length-1 walks
    k = 1
    for s,t in P.edges():
        r2g = ( regions[ s ], regions[ t ] )
        edge_matrix = generators[ r2g[0],r2g[1] ]
        edgeset[k].append( Walk( s, t, set( [(s,t)] ), edge_matrix, k ) )
        
     
