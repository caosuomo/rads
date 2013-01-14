from walk_library import UnverifiedLibrary, BadLibrary
from collections import deque

class IndexMapProcessor( object ):
    """ Processes a given index map of generators """
    def __init__( self, indexmap, debug=False ):
        """
        indexmap -- IndexMap object
        """
        self.debug = debug
        self.map = indexmap
        # Stores all edge sets that need to have one edge cut
        self.bad_edges = BadLibrary()

        # Unravel the index map (stored as a graph) into list of 1-step walks. 
        self.edge_walks = {}
        for start,end,attr in self.map.graph.edges_iter( data=True ):
            self.edge_walks[ (start,end) ] = attr["edge"]
            #        self.edge_walks = [ attr["edge"] ]
        
        # Stores edges that need to be expanded, to search for more edges
        # init with the 1-step walks above
        self.todo = deque( self.edge_walks.values() )
        
        # Stores all edges that are unverified
        # again, init with the 1-step walks
        self.unverified = UnverifiedLibrary( self.edge_walks.values() )

    def find_bad_edge_sets( self, maxLength ):
        """
        Version of Algorithms 6 in Day, Frongillo, Trevino.

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
        
        while self.todo[0].length <= maxLength:
            # Grab the first walk to extend
            old = self.todo.popleft()
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
            for succ in self.map.graph.successors_iter( old.end ):
                next_step = self.edge_walks[ (old.end, succ) ]
                if self.debug:
                    print "Successor edge:", next_step
                    print " matrix"
                    print next_step.matrix
                #for next_step in ( append["edge"] for append in self.map[old.end] ):
                # Create a new walk by appending the new edge
                new_walk = old + next_step

                if self.debug:
                    print "--> New walk:"
                    print new_walk
                    print new_walk.matrix
                
                # Check if the walk has an edgeset that will already be cut...
                # If so, ignore it
                if new_walk in self.bad_edges:
                    continue
                
                # Check if the walk is a zero matrix, or is a cycle with zero
                # trace. If so, add it to the bad edge list.
                if new_walk.zero() or (new_walk.cycle() and new_walk.zero_trace()):
                    if self.debug:
                        print "=========================="
                        print "BADS", self.bad_edges.bads
                        print " ** Adding BAD edgeset **"
                        print new_walk.edges
                        print new_walk.matrix
                        print "=========================="

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
                
                if self.debug:
                    print "\n\n"

    # def cutBadEdgeSets(self, **kwargs):
    #     args = {bad_edge_sets = self.bad_edges.bad,
    #             cut_edges = [],
    #             excluded_edges = [],
    #             best_entropy = -1,
    #             cutoff = 10000}
                
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
    
    IM = IndexMap( generators, regions, adjmatrix, debug=True )
