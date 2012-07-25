"""
walk_library.py

Opened: Jun. 13, 2012

Author: Jakub Gedeon

Contains classes and functions for dealing with storing and handling a set of
walks through a graph where matrixies are associated with each edge.
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
    def __init__(self, edgesets=[]):
        self.bads = []
        self.bads.extend[edgesets]
    
    def __contains__(self, edgeset):
        """
        Returns true if the given edgeset is a superset of any set to be cut,
        which implies that the edgeset will be cut as well.
        """
        for bad in self.bads:
            if bad <= edgeset:
                return True
        return False

    def add(self, edgeset):
        """
        Adds the edgeset to the set of edges to be cut
        """
        # If a edgeset is a superset of the added set, it is going to be cut
        # anyways, so remove all such sets.
        self.bads = filter(lambda bad: not bad >= edgeset, self.bads)
        self.bads.append(edgeset)

class UnverifiedLibrary( object ):
    """
    A library of walks for fast lookup and indexing, for walks
    that have not yet been exhaustively searched
    """
    def __init__(self, walks=[]):
        # Allows fast search for items given a start and end, and edgeset
        self.start_end_dict = defaultdict( lambda: defaultdict( lambda: defaultdict(list) ) )
        for walk in walks:
            self.add(walk)
    
    def add(self, walk):
        """ Add a Walk to the library """ 
        self.start_end_list[walk.start][walk.end][walk.edges] = walk
        self.count += 1

    def has_scalar_multiple(self, walk):
        """ Finds every scalar multiple of the given walk """
        pass
        # for other in self.start_end_dict[walk.start][walk.end][walk.edges]:
        #     TODO!!!!! BROKEN

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
        self.edges = frozenset(edges)
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
                     self.edges.union(other.edges),
                     np.dot(self.matrix, other.matrix),
                     self.length + other.length )

    def __repr__( self ):
        s = "Walk(start="+str( self.start )+\
            ", end="+str( self.end )+\
            ", len(edges)="+str( len( self.edges ))+\
            ", length="+str(self.length)+")"
        return s

    def zero_trace(self):
        """ Test if walk has a zero trace """
        return np.trace(self.matrix)==0

    def cycle(self):
        """ Test if a walk is a cycle """
        return self.start==self.end

    def zero(self):
        """ Test if the matrix of the walk is a zero matrix
        These walks are generally considered useless """
        return np.any(self.matrix!=0)

if __name__ == "__main__":

    import sys
    sys.path.append( '/Users/jberwald/github/local/caja-matematica/rads/src/rads' )
    import graphs.digraph as DG
    
    # sample data for unit tests
    # barbell-shaped graph with two self-loops at the ends
    edges = [(0,0),(0,1),(1,0),(2,1),(2.5,2),(3,2.5),(4,3),(4,5),(5,4)]
    G = DG.DiGraph( )
    G.add_edges_from( edges )
