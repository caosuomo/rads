from symbolics/walk_library import UnverifiedLibrary, BadLibrary
from misc/

from collections import deque

class IndexMapProcessor(object):
    """ Processes a given index map of generators """
    def __init__(self, indexmap)
        self.map = indexmap
        # Stores all edge sets that need to have one edge cut
        self.bad_edges = BadLibrary()
        
        edge_walks = [attr["edge"] for start,end,attr in indexmap.edges_iter()]
        # Stores edges that need to be expanded, to search for more edges
        self.todo = deque(edge_walks)
        # Stores all edges that are unverified
        self.unverified = UnverifiedLibrary(edge_walks)

    def findBadEdgeSets(self, maxLength)
        while self.todo[0] <= maxLength:
            # First, grab the first walk to extend
            old = self.todo.popleft()
            # Now take every edge that comes out from the end of the old walk 
            for  in (append["edge"] for append in self.map[old.end])
                # Create a new walk by appending the new edge
                new = edge + next
                
                # Check if the walk has an edgeset that will already be cut...
                # If so, ignore it
                if new in self.bad_edges:
                    continue
                
                # Check if the walk is a zero matrix, or is a cycle with zero
                # trace. If so, add it to the bad edge list.
                if new.zero() or (new.cycle() and new.zero_trace()):
                    self.bad_edges.add(new.edges)
                    continue

                # If it has a scalar multiple, then the edge does not need to
                # be cut
                if self.unverified.has_scalar_multiple(new):
                    continue

                # We have not decided if this edge is bad or good, so it has to
                # added back into the queue of edges to check
                self.todo.push(new)
                self.unverified.add(new)

    def cutBadEdgeSets(self, **kwargs):
        args = {bad_edge_sets = self.bad_edges.bad,
                cut_edges = [],
                excluded_edges = [],
                best_entropy = -1,
                cutoff = 10000}
                
                
                
                
