"""
utils.py

Author: Jesse Berwald

Opened: Feb. 15, 2012

A wrapper for various utility functions. Meant to be subclassed.
"""
import networkx as nx
import numpy as np
import cPickle as pkl
import scipy.io as spio
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
        return np.matrix( mat )
    else:
        mat = spio.loadmat( matfile )
        return np.matrix( mat[ matname ] )

def _extract_mat( mat ):
    keys = mat.keys()
    mat_key = filter( lambda x : not x.startswith( "__" ), keys )[0]
    return mat[ mat_key ]

def convert_matlab_gens( genfile, genname='generators' ):
    """
    Convert a Matlab (R) cell array to a dictionary.

    To guarantee a successful conversion, the name of the cell array
    in Matlab (R), should be provided (defaults to 'generators'. This
    allows provides an easy key to pick out in the Python dictionary
    returned by loadmat. Otherwise, there's a bit of guess work in
    cell2dict to find where the generators are stored.
    """
    cell_array = spio.loadmat( genfile, squeeze_me=True )
    return cell2dict( cell_array, genname )

def cell2dict( ca, genname ):
    """
    Parameters:
    -----------

    ca : cell array from Matlab, loaded using scipy.io.loadmat(). See
    convert_matlab_gens().

    ca is a np.ndarray of type object. Every entry is an array. Eg., if

        A = array([1, [2 3],[],...]), then
        
        A[0] = array(1,dtype=uint8) <-- dimensionless array,
        A[1] = array([2,3],dtype=uint8),
        etc.

    Empty lists (see above example) are quietly ignored.

    Returns a Python dictionary
    """
    keys = ca.keys()
    # there should only be one name for the cell array, the other keys
    # should be metadata (eg., '__globals__', etc.)
    try:
        gens = ca[ genname ]
    except KeyError:
        name = [ k for k in keys if not k.startswith('__') ][0]
        gens = ca[name][0]
    gdict = {}

    # region (r) |--> gen map
    # +1 to each region to aid with creation of index map
    for r,gen in enumerate( gens ):
        # dimensionless array
        if not gen.shape: 
            gdict[r+1] = [gen.item()]
        # gen contains a vector of gnerators
        else:
            g = gen.tolist()
            # no generators in this region
            if len( g ) == 0:
                # no generator on this region
                continue
            else:
                gdict[r+1] = g
    # Now that we've created the hash, shift all region labels and
    # generator labels by (-1) to align with Python 0-based indexing.
    aligned_dict = {}
    for key, val in gdict.items():
        new_val = [ x-1 for x in val ]
        aligned_dict[ key-1 ] = new_val
    return aligned_dict # gdict

def index_map_to_region_map( hom_mat, reg2gen, shift=0 ):
    """
    hom_mat : numpy matrix representing a map on generators (index map).

    reg2gen : dictionary mapping region -> generator(s).

    shift : align indices for numpy matrices. If generators produced
    from Matlab matrix or cell array of generators, then must account
    of 1-indexing in Matlab by shift = -1.

    Uses NetworkX blockmodel to condense reg2gen 

    Returns a numpy matrix representing the transition map on the
    regions.
    """
    H = hom_mat
    R = reg2gen
    Rinv = invert_dictionary( R )
    G = DiGraph()
    for k in R.keys():
        # # k is a tuple or set
        # if shift:
        #     idx = np.array( R[k] ) + shift
        #     gen_conns = np.where( H[:,idx] != 0 )[0]
        # else:
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


def to_sparse( G ):
    """
    DiGraph to scipy sparse matrix.
    """
    try:
        return nx.to_scipy_sparse_matrix( G.graph, dtype=int, format='csr' )
    # in case one sends in G.graph instead.
    except AttributeError:
        return nx.to_scipy_sparse_matrix( G, dtype=int, format='csr' )        

