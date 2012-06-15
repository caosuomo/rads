import networkx as nx
import numpy as np
from collections import defaultdict



def simple_cycles_jjb( G ):
    """Find simple cycles (elementary circuits) of a directed graph.
    
    An simple cycle, or elementary circuit, is a closed path where no 
    node appears twice, except that the first and last node are the same. 
    Two elementary circuits are distinct if they are not cyclic permutations 
    of each other.

    Parameters
    ----------
    G : NetworkX DiGraph
       A directed graph

    Returns
    -------
    A list of circuits, where each circuit is a list of nodes, with the first 
    and last node being the same.
    
    Example:
    >>> G = nx.DiGraph([(0, 0), (0, 1), (0, 2), (1, 2), (2, 0), (2, 1), (2, 2)])
    >>> nx.simple_cycles(G)
    [[0, 0], [0, 1, 2, 0], [0, 2, 0], [1, 2, 1], [2, 2]]
    
    See Also
    --------
    cycle_basis (for undirected graphs)
    
    Notes
    -----
    The implementation follows pp. 79-80 in [1]_.

    The time complexity is O((n+e)(c+1)) for n nodes, e edges and c
    elementary circuits.

    References
    ----------
    .. [1] Finding all the elementary circuits of a directed graph.
       D. B. Johnson, SIAM Journal on Computing 4, no. 1, 77-84, 1975. 
       http://dx.doi.org/10.1137/0204007

    See Also
    --------
    cycle_basis
    """
    # Jon Olav Vik, 2010-08-09
    def _unblock(thisnode):
        """Recursively unblock and remove nodes from B[thisnode]."""
        if blocked[thisnode]:
            blocked[thisnode] = False
            while B[thisnode]:
                _unblock(B[thisnode].pop())

    # JJB -- 2012-05-15
    def _zero_trace( mat ):
        try:
            if np.trace( mat ) == 0:
                return True
            else:
                return False
        except ValueError:
            if mat == 0:
                return True
            else:
                return False       

    # JJB -- 2012-05-15
    def _zero_product( mat ):
        nz = mat.nonzero()
        # nonzero() return nz elements by axes, so if anything is
        # nonzero, the first axis will show up with length > 0
        if nz[0].size > 0:
            return True
        else:
            return False
    
    def circuit(thisnode, startnode, component, genmap=1):
        closed = False # set to True if elementary path is closed
        path.append(thisnode)
        blocked[thisnode] = True

        # JJB -- 2012-05-15, added map_on_gen
        print "thisnode", thisnode
        for nextnode, map_on_gen in component[thisnode].items(): # direct successors of thisnode
            print "  nextnode", nextnode
            try:
                gen_products.append( map_on_gen['gen'] * gen_products[-1] )
            except ValueError:
                gen_products.append( map_on_gen['gen'] * gen_products[-2] )
            # This path contains a prohibited edge, move on
            if not _zero_product( gen_products[-1] ):
                print "  zero, path", path
                print "    product", gen_products
                # print "*********"
                # print "ZERO"
                # print "*********"
                # get rid of the last zero product so it's not used in further calculations
                gen_products.pop()
                continue
            if nextnode == startnode:
                # This cycle is not verifiable
                if _zero_trace( gen_products[-1] ):
                    continue
                else:
                    result.append(path + [startnode])
                    print "RESULT", result[-1]
                    print "matrix product", gen_products[-1]
                    print ""
                closed = True
            elif not blocked[nextnode]:
                if circuit(nextnode, startnode, component, genmap=gen_products[-1]):
                    closed = True
        if closed:
            _unblock(thisnode)
        else:
            for nextnode in component[thisnode]:
                if thisnode not in B[nextnode]: # TODO: use set for speedup?
                    B[nextnode].append(thisnode)
        path.pop() # remove thisnode from path
        return closed
    
    if not G.is_directed():
        raise nx.NetworkXError(\
            "simple_cycles() not implemented for undirected graphs.")
    path = [] # stack of nodes in current path
    blocked = defaultdict(bool) # vertex: blocked from search?
    B = defaultdict(list) # graph portions that yield no elementary circuit
    result = [] # list to accumulate the circuits found
    # Johnson's algorithm requires some ordering of the nodes.
    # They might not be sortable so we assign an arbitrary ordering.
    ordering=dict(zip(G,range(len(G))))
    for s in ordering:
        # Build the subgraph induced by s and following nodes in the ordering
        subgraph = G.subgraph(node for node in G 
                              if ordering[node] >= ordering[s])
        # Find the strongly connected component in the subgraph 
        # that contains the least node according to the ordering
        strongcomp = nx.strongly_connected_components(subgraph)
        mincomp=min(strongcomp, 
                    key=lambda nodes: min(ordering[n] for n in nodes))
        component = G.subgraph(mincomp)
        if component:
            # new generator product
            gen_products = [1]
            # smallest node in the component according to the ordering
            startnode = min(component,key=ordering.__getitem__) 
            for node in component:
                blocked[node] = False
                B[node][:] = []
            dummy=circuit(startnode, startnode, component)
    
    return result

def create_subshift_matrix( cycles, num_regions ):
    """
    From verified cycles, construct appropiate adjacency matrix.
    """
    subshift = np.zeros( (num_regions, num_regions), dtype=np.int )
    for cycle in cycles:
        c = zip( cycle[:-1], cycle[1:] )
        for e in c:
            subshift[e] = 1
    return subshift.T


if __name__ == "__main__":

    #import index_map as IM
    import index_processor as IP

    if 1:
        map_on_generators = np.matrix( [[0,0,0,0,-1,0,0,-1],
                                        [0,0,0,0,-1,0,0,0],
                                        [0, 0, 0, 0, 0, 0, 1, 0],
                                        [1, 0, 0, 0, 0, 0, 0, 0],
                                        [ 0 ,1, 1, 0, 0, 0, 0, 0],
                                        [0, 0, 0, 1, 0, 0, 0, 0],
                                        [0, 0, 0, 0, 0, -1, 0,0],
                                        [0, 0, 0, 0, 0, 1, 0,0]]
                                        ).T

        # have to adjust for mutiple generators in each region
        # nodes refer to regions, and index ranges give generators within
        # said region
        A = map_on_generators
        index_edges = { (0,2): A[0,3].T,
                        (1,3): A[1:3,4].T,
                        (2,4): A[3,5].T,
                        (3,0): A[4,0].T,
                        (3,1): A[4,1:3].T,
                        (4,5): A[5,6:8].T,
                        (5,0): A[6:8,0].T,
                        (5,1): A[6:8,1:3].T
                        }

        map_on_regions = np.matrix( [[0,0,0,1,0,1],
                                     [0, 0, 0, 1, 0, 1],
                                     [1,0, 0, 0, 0, 0],
                                     [0, 1, 0, 0, 0, 0],
                                     [0,0,1,0,0,0],
                                     [0,0,0,0,1,0]]
                                     ).T

        G = nx.DiGraph()
        nx.from_numpy_matrix( map_on_regions, create_using=G )
        for e, gmap in index_edges.items():
            G[ e[0] ][ e[1] ][ 'gen' ] = gmap

        cycles = simple_cycles_jjb( G )
        print ""
        print "cycles", cycles

        T = create_subshift_matrix( cycles, num_regions=len(G) )

        spectral_radius = np.abs( np.linalg.eigvals( T ).max() )
        entropy = np.log( spectral_radius )
        print "entropy", entropy

    if 0:
        map_on_generators = np.matrix([[0,1],[1,1]])

        A = map_on_generators

        index_edges = { (0,0): A[0,0].T,
                        (0,1): A[0,1],
                        (1,0): A[1,0],
                        (1,1): A[1,1]
                        }

        map_on_regions = np.matrix( [[1,1],[1,1]] )
        G = nx.DiGraph()
        nx.from_numpy_matrix( map_on_regions, create_using=G )
        for e, gmap in index_edges.items():
            G[ e[0] ][ e[1] ][ 'gen' ] = gmap

        cycles = simple_cycles_jjb( G )
        print ""
        print "cycles", cycles

        T = create_subshift_matrix( cycles, num_regions=len(G) )

        spectral_radius = np.abs( np.linalg.eigvals( T ).max() )
        entropy = np.log( spectral_radius )
        print "entropy", entropy
