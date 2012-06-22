from graphs.digraph import DiGraph
from symbolics import Path

from numpy import zeros

class IndexMap(DiGraph):
    def __init__(self, generators=None, regions=None, debug=None):
        """
        Inputs:
            generators = a matix representing mapping of generators in rows onto
                generators in columns.
            regions = a list of lists representing generators per region
            debug (optional) = a boolean representing if debug statements should
                be printed
        """
        self.generators = generators
        self.regions = regions

        # Graph that will store the backend reprsentation
        DiGraph.__init__(self)

        # Utility functions for packing and unpacking edge information into a integer
        regioncount = len(regions)
        self.edge2hash = lambda start, end: start*regioncount+end
        self.hash2edge = lambda hashed: hashed/regioncount, hashed%regioncount

        # Instantiate the edges between with a length 1 walk on each
        for source_region, source_gens in enumerate(regions):
            for sink_region, sink_gens in enumerate(regions):
                edge = Walk(start=source_region,
                            end=sink_region,
                            walk=frozenset((self.edge2hash(source_region,
                                sink_region)),
                            matrix=generators[source_gens,:][:,sink_gens],
                            length=1)
                if not edge.zero():
                    self.graph.add_edge(source_region, sink_region, edge=Walk)
                    self.library.add(edge)