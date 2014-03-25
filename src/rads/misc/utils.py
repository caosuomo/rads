"""
utils.py

Author: Jesse Berwald

Opened: Feb. 15, 2012
"""
import networkx as nx
import numpy as np
import cPickle as pkl
import scipy.io as spio
from scipy import sparse
from rads.graphs import algorithms, DiGraph

def load_numpy( fname ):
    """
    Returns numpy array stored as .npy file
    """
    return np.matrix( np.load( fname ) )

def loadtxt( fname, dtype=np.int ):
    """
    Returns numpy array stored as .npy file
    """
    return np.matrix( np.loadtxt( fname, dtype=dtype ) )

def load_generators( fname ):
    """
    Returns dictionary of region --> generator hash map
    """
    with open( fname ) as fh:
        d = pkl.load( fh )
    return d
    

def load_matlab_matrix( matfile, matname=None ):
    """
    Wraps scipy.io.loadmat.

    If matname provided, returns np.ndarray representing the index
    map. Otherwise, the full dict provided by loadmat is returns.
    """
    if not matname:
        out = spio.loadmat( matfile )
        mat = _extract_mat( out )
        # if mat is a sparse matrix, convert it to numpy matrix
        try:
            mat = np.matrix( mat.toarray() )
        except AttributeError:
            mat = np.matrix( mat )
        return mat
    else:
        matdict = spio.loadmat( matfile )
        mat = matdict[ matname ]
        # if mat is a sparse matrix, convert it to numpy matrix
        try:
            mat = np.matrix( mat.toarray() )
        except AttributeError:
            mat = np.matrix( mat )
        return mat #np.matrix( mat[ matname ] )

def _extract_mat( mat ):
    keys = mat.keys()
    mat_key = filter( lambda x : not x.startswith( "__" ), keys )[0]
    return mat[ mat_key ]

def convert_matlab_gens( genfile, genname='generators' ):
    """
    Convert a Matlab (R) cell array to a dictionary.


	To guarantee a successful conversion, the name of the cell array
    in Matlab (R), should be provided (defaults to 'generators'). This
    allows provides an easy key to pick out in the Python dictionary
    returned by loadmat. Otherwise, there's a bit of guess work in
    cell2dict to find where the generators are stored.
    """
    cell_array = spio.loadmat( genfile )#, squeeze_me=True )
    return cell2dict( cell_array, genname )

def cell2dict( ca, genname ):
    """
    Parameters:
    -----------

    ca : cell array from Matlab, loaded using scipy.io.loadmat(). See
    convert_matlab_gens().

    ca is a np.ndarray of type object. Every entry is an array. Eg., if

        A = array([1, [2 3],[],...]), then
        
        A[0] = array(1,dtype=int8) <-- dimensionless array,
        A[1] = array([2,3],dtype=int8),
        etc.

    Empty lists (see above example) are quietly ignored.

    Returns a Python dictionary
    """
    keys = ca.keys()
    # there should only be one name for the cell array, the other keys
    # should be metadata (eg., '__globals__', etc.)
    try:
        gens = ca[ genname ]
    # this should work, as long as one does not use '__' in the name
    # of the cell array.
    except KeyError:
        name = [ k for k in keys if not k.startswith('__') ][0]
        gens = ca[name][0]

    # gens is a list of arrays, of type uint8, of shape (1,n)
    # region (r) |--> gen map
    genmap = {}
    gens = gens.flatten().tolist()
    
    # Remember to shift all region labels and generator labels by (-1)
    # to align with Python 0-based indexing.
    for r,g in enumerate( gens ):
        try:
            genmap[ r ] = map(lambda x: x-1, g.flatten().tolist())
        except AttributeError:
            genmap[ r ] = map(lambda x: x-1, g[0].flatten().tolist())
    return genmap

def index_map_to_region_map( hom_mat, reg2gen ):
    """
    hom_mat : numpy matrix representing a map on generators (index map).

    reg2gen : dictionary mapping region -> generator(s). (I.e, which
    regions support which generators)

    Returns a DiGraph object of the map on regions in phase space.
    """
    H = hom_mat
    R = reg2gen
    Rinv = invert_dictionary( R )
    G = DiGraph()
    
    # find where region k maps to based the index map
    for k in R.keys():
        # find generator connections
        if hasattr( H, 'nnz' ):
            if len( R[k] ) == 0:
                continue
            gen_conns, _J, _V = sparse.find( H[:,R[k]] )
        else:
            # dense matrix case
            gen_conns = np.where( H[:,R[k]] != 0 )[0]
            gen_conns = gen_conns.tolist()[0] # fix matrix formatting
        for edge in gen_conns:
            for glist in Rinv.keys():
                if edge in glist:
                    G.add_edge( k, Rinv[glist][0] )

    # return the graph so that we have access to the nodes labels that
    # correspond directly to regions with generators.
    return G

def make_node_hash( nbunch ):
    """
    Create a mapping from the boxes --> indices listed in the nodes of
    the directed graph.
    """
    idx = range( len( nbunch ) )
    d = dict()
    #d.fromkeys( idx )
    #d.fromkeys( nbunch )
    for i in idx:
        d[i] = nbunch[i]
    return d
    
def invert_dictionary( d ):
    inv_map = {}
    for k, v in d.iteritems():
        try:
            inv_map[v] = inv_map.get(v, [])
            inv_map[v].append(k)
        except TypeError:
            v = tuple( v )
            inv_map[v] = inv_map.get(v, [])
            inv_map[v].append(k)
    return inv_map

def array2chomp( arr, savename ):
    """
    Convert an array to chomp format, ( , , ). Write the resulting
    column of numbers to disk. Formatted for use with chomp-rutgers
    (see https://code.google.com/p/chomp-rutgers/)
    """
    rows = map( lambda x: str(x)+'\n', map( tuple, iter( arr ) ) ) 
    with open( savename, 'w' ) as fh:
        fh.writelines( rows )

def box2cub( boxes, fname ):
    """Write collection of boxes to disk in homcubes format. 

    X : indices (which boxes to write to disk

    boxes : n x d array, where d is the dimension of the
    complex. These are typically box corners from a tree

    """
    # (n,dim) shape
    dim = boxes.shape[1] 
    with open( fname, 'w' ) as fh:
        fh.write( 'dimension '+str( dim )+'\n' )
        # converts row \in A |--> (x,y,...) str coords in implicit
        # loop
        rows = map( lambda x: str(x)+'\n', map( tuple, iter( boxes ) ) ) 
        fh.writelines( rows )



def to_sparse( G ):
    """
    DiGraph to scipy sparse matrix.
    """
    try:
        return nx.to_scipy_sparse_matrix( G.graph, dtype=int, format='csr' )
    # in case one sends in G.graph instead.
    except AttributeError:
        return nx.to_scipy_sparse_matrix( G, dtype=int, format='csr' )        

