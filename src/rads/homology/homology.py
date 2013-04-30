import networkx as nx
from rads.graphs import algorithms
from rads.misc import utils
import numpy as np
import random, time
import subprocess as sp

debug = True

# from SO http://stackoverflow.com/questions/6800193
factors = lambda n: set(reduce(list.__add__, 
                  ([i, n//i] for i in range(1, int(n**0.5) + 1)
                   if n % i == 0)))


def get_cycles( P, max_period=2 ):
    """
    P : Directed graph

    P will be converted to sparse transition matrix for multiplication
    and cycle detection.
    """
    C = utils.to_sparse( P )
    nodes = P.nodes()
    nodes.sort()

    # Create a node map, index --> node number.  The graph
    # representation typically respects the original box ordering (at
    # least at that depth of the tree), whereas the sparse matrix
    # indexing is 0,...,len(nbunch).  The hash allows a quick link
    # between a sparse matrix representation of a combinatorial map
    # and a graph representation.
    # Note: searching a dictionary (eg. x in d) is O(1), while search
    # for containment in a list is O(n). Test using %timeit x in d ==>
    # 23.5 us (list), 70.6 ns (dict).
    nodemap = utils.make_node_hash( nodes )

    # convert to compressed sparse row format for efficiency
    #C = sp.tocsr()
    # period 1,2,..., max_period. This dict format will make plotting
    # different cycles and connections easier in the future.
    if debug:
        print "looking for cycles..."
    cycles = {}
    for k in range( 1, max_period+1 ):
        Ck = C**k 
        pk = set( np.where( Ck.diagonal() )[0] )

        # don't repeat multiples of cycles
        if k > 1:
            # find all factors of k
            facs = list( factors( k ) )[:-1]
            # remove all nodes from cycles of length j < k if j is a
            # factor of k (excluding k itself)
            for j in range( 1, k ):
                # if a shorter cycle of length j is a factor of the
                # cycles length k then we want to exclude it. see
                # above.
                if j in facs:
                    pk.difference_update( cycles[ j ] )
        if len( pk ) == 0:
            continue
        cycles[ k ] = pk

    Pcycles = {}
    for k,cyc in cycles.iteritems():
        pk = set()
        for c in cyc:
            # apply the node map
            pk.add( nodemap[ c ] )
        Pcycles[ k ] = pk
        
    if debug:
        print "Cycle stats:\n"
        ckeys = cycles.keys()
        ckeys.sort()
        for c in ckeys:
            print "    "+str(len(cycles[c]))+" period "+str( c )+\
                " cycles."
        print ""
    return Pcycles

def connect_cycles( A, B, P ):
    """
    A : set of period k orbits

    B : a set of period k' orbits

    P : transition graph

    TODO : Split up and choose only one node/box from each cycle. This
    is redundant as it loops through the cycles.
    """
    C = set()
    for u in A:
        for v in B:
            if u != v:
                # conns holds connecting nodes in path, but not the
                # start/end nodes.
                conn = connections( P, u, v )
                C.update( conn )
    return C

def connections( G, u, v ):
    """
    Find connections from one set of nodes to another (preds and
    succs, resp.).
    """
    conns = nx.dijkstra_path( G.graph, u, v )
    return conns[1:-1] # trim endpoints
        
def isolated( A, oI, S ):
    """
    Check whether the maximal invariant set is in the interior of oI.
    
    A : adjacency graph
    
    oI : one box neighborhood

    S : maximal invariant set
    """
    inv_map = A.inv_nodemap #convert.invert_dictionary( A.nodemap )
    # does the inv_set have any one-box neighbors that are not in oI?
    # Find adjacent boxes in order to figure this out.
    oI_adj = set( [inv_map[u] for u in oI] )
    S_adj = set( [inv_map[u] for u in S] )
    # Are there any neighbors of u in S that are not in oI?  If
    # so, then S can't be in the interior.
    for u in S_adj:
        nbrs_u = set( A.graph.neighbors( u ) )
        if not nbrs_u.issubset( oI_adj ):
            return False
    else:
        return True

def grow_iso( S, A, P ):
    """
    S : index set of boxes in invariant set
    
    P : transition graph

    A : Adjacency graph

    Grow a combinatorial isolating neighborhood N containing S. Return
    None if condition is not met.

    Algorithm 1 in Day, et al, 2008.
    """
    while 1:							
        N = set( algorithms.graph_mis( P.subgraph( S ) ) )
        oN = get_onebox( N, A )

        # print "in grow_iso"
        # print oN - N
        # print ""
        
        if N.issubset( oN ):
            return oN
        N = oN
    
def get_onebox( S, A ):
    """
    Return an isolating, one-box neighborhood around the boxes in
    S. (This rather inefficiently includes S itself. There should be a
    nice way to keep S, and just add to it.)
    
    S : box indices corresponding to nodes in transition map

    A : adjacency graph

    Returns S \cup combinatorial neighborhood of S
    """
    # container for (x,y) box tuples
    thehood = set()
    oS = S.copy()
    # loop over nodes contained in S, keep only those that are
    # adjacent but not already in S.
    for idx in oS:
        nbrs = A.neighbors( idx )
        thehood.update( nbrs )
    oS.update( thehood )
    return oS

def max_inv_set( P, I ):
    """
    Restrict the map P to the maximal isolated invariant neighborhood.

    Return indices of nodes in the maximal invariant set.
    """
    R = nx.subgraph( P.graph, I )
    S = algorithms.graph_mis( R )
    return set( S )

def build_ip( P, A, I ):
    """
    Building modified combinatorial index pairs for the combinatorial
    isolating neighborhood I.

    P : combinatorial map object

    A : adjacency matrix (spacial)

    I : invariant set. List or set object

    Returns the index pair X,A
    """
    # create one box nhd about the invariant set
    I = set( I )
    oI = get_onebox( I, A )

    print "oI - I", oI - I
    
    # initial X as the invariant set
    X = I.copy()
    
    # map the invariant set forward, F(I)
    print len(X)
    for u in I:
        X.update( P.neighbors( u ) )
    # just keep the image that intersects the one box nhd
    X = X & oI
        
    # initial exit set A
    A = X - I
    onebox = oI - I

    print "A init", A
    
    # grow the exit set
    while 1:
        FA = set()
        # image of exit set, eg., F(A)
        for u in A:
            FA.update( P.neighbors( u ) )
        # only keep images that intersect the one box 'hood            
        FA = FA & onebox # oI
        if FA.intersection( I ):
            print "eek!", len( FA.intersection( I ) )
        # images we haven't seen in the one box 'hood
        new_boxes = FA - A
        # print "new boxes", len(new_boxes)
        A.update( new_boxes )
        if not new_boxes:
            break
    X = I.union(A)
    return X,A,oI

def computeYB( X, I, P ):
    """
    Returns F(X) and F(X)\I
    """
    X_ = X.copy()
    I = set(I).copy()
    FX = set()
    # Find F(X)
    for u in X:
        FX.update( P.neighbors( u ) )
    Y = X_.union(FX)
    B = Y - I
    return Y,B


def run_homcubes( prefix,
                  full_path='/Users/jberwald/Dropbox/Projects/entropy/lorenz/' ):
    """
    Sample command:

    homcubes -i henon.map henon-X.dat henon-A.dat henon-Y.dat henon-B.dat -R

    where prefix='henon' == full path to data.
    """
    space = ' '
    prefix = full_path+prefix
    command = space.join( ['homcubes', '-i', prefix+'.map',\
                           prefix+'-X.dat', prefix+'-A.dat',\
                           prefix+'-Y.dat', prefix+'-B.dat',\
                           '-R', '-g', prefix+'.gen',\
                           '>'+prefix+'.out'] )
    print "homcubes called with the following command:"
    print command
    p=sp.call( command, shell=True )

def make_index_pair( P, A, I ):
    """
    P : transition graph on phase space

    A : adjacency graph on boxes in phase space

    I : list (set) of boxes in isolated neighborhood.
    """
    N = grow_iso( I, A )
    X,A = build_ip( P, Adj, I )
    Y,B = computeYB( X, I, P )
    return X,A,Y,B
