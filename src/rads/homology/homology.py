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

def grow_isolating_neighborhood( invariant, adjacency, transition ):
    """
    invariant : list of boxes in invariant set (any Python iterable)
  
    adjacency : Adjacency graph  
    
    transition : transition graph

    Grow a combinatorial isolating neighborhood N containing S. Return
    None if condition is not met.

    Algorithm 1 in Day, et al, 2008.
    """
    S = invariant
    while 1:							
        N = set( algorithms.graph_mis( transition.subgraph( S ) ) )
        oN = get_onebox( N, adjacency )

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
    
    S : indices of boxes corresponding to nodes in transition map

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

def build_index_pair( iso, adjacency, transition ):
    """
    Building modified combinatorial index pairs for the combinatorial
    isolating neighborhood I.

    iso : isolating 'hood. List or set object

    adjacency : adjacency matrix (spacial)

    transittion : combinatorial map object

    Returns the index pair X,A
    """
    I = iso
    A = adjacency
    P = transition

    # create one box nhd about the invariant set
    I = set( I )
    oI = get_onebox( I, A )
    
    # initial X as the invariant set
    X = I.copy()
    
    # map the invariant set forward, F(I)
    for u in I:
        X.update( P.neighbors( u ) )
    # just keep the image that intersects the one box nhd
    X = X & oI
        
    # initial exit set A
    A = X - I
    onebox = oI - I
    
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
    return X,A

def computeYB( X, iso, P ):
    """
    Finding the image, (Y,B) of (X,A) under F.

    X : from index pair (X,A)

    A : Exit set. So we can find I=X\A below

    Returns Y=F(X) and B=F(X)\I
    """
    # if we don't copy, union below will be in-place
    X_ = X.copy()
    I = set( iso ).copy()
    FX = set()
    # Find F(X)
    for u in X:
        FX.update( P.neighbors( u ) )

    # F(X)
    Y = X_.union(FX)

    # B = Y - I, where I = X\A
    B = Y - I
    return Y,B


def run_homcubes( prefix, save_dir ):
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

def make_index_pair( iso, adjacency, transition ):
    """
    iso : list (set) of boxes in isolating neighborhood.

    transition : transition graph on phase space (combinatorial enclosure)

    adjacency : adjacency graph on boxes in phase space
    
    returns X,A,Y,B
    """
    X,A = build_index_pair( iso, adjacency, transition )
    Y,B = computeYB( X, iso, transition )
    return X,A,Y,B

def box2cub( tree, idx, fname ):
    """Write corners of boxes[idx], stored in tree, to disk.
    
    The scaling, done in order to put the corners on a unit grid,
    assumes that the grid is *uniform* in all dimension. 

    The format used is for homcubes (registered trademark of
    P. Pilarcyk).

    tree : box tree associated with the combinatorial enclosure.

    idx : list of indices of the set of boxes to write to disk

    fname : full path where we should write the cub file.

    """
    idx = list( idx )
    dim = tree.dim
    boxes = tree.boxes()
    corners = boxes.corners

    # diff finds the first difference along axis=-1 by default.
    r = corners[1:] - corners[:-1]
    
    scaled = np.empty_like( corners )
    for i in range( dim ):
        # assume uniform box size in each dimension
        scaled[:,i] = corners[:,i] / r[0,i]

    scaled = np.asarray( scaled, dtype=np.int )

    with open( fname, 'w' ) as fh:
        fh.write( 'dimension '+str(dim)+'\n' )
        # converts each row in A |--> (x,y,...) str coords 
        rows = map( lambda x: str(x)+'\n', map( tuple, iter( scaled[idx] ) ) ) 
        fh.writelines( rows )
