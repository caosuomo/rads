"""
convert.py

Author: Jesse Berwald

Opened: Feb. 24, 2012

Package containing function for converting various data formats to
formats required by DiGraph and IndexMap.
"""
import numpy as np
from scipy.io import loadmat

def load_index_map( data ):
    """
    Returns a Numpy adjacency matrix with weighted edges. data is
    converted to a numpy.matrix before being returned.

    Parameters:
    -----------

    data : The following formats are allowable,

           . Numpy array (returned unaltered)
           . Matlab (R) array
           . DiGraph object
           . edge bunch
           . filepath to any of the above
    """
    # numpy array or matrix, return immediately after shape check
    if hasattr( data, np.array ) or\
        hasattr( data, np.matrix ):
        nx,ny = data.shape
        assert nx == ny, "Adjacency matrix is not square."
        return np.mat( np.data )
    # string, filename 
    elif hasattr( data, 'join' ):
        try:
            return np.mat(np.load( data ))
        except IOError:
            pass
    # more to add below....
    else:
        print "oops, made it here!"
 
        
def convert_generators( gens ):
    """
    If gens is a path to a Matlab (R) cell array, convert to a
    dictionary. Otherwise, if gens is already a dict, just return it
    unchanged.
    """
    if hasattr( gens, 'keys'):
        return gens
    else:
        cell_array = loadmat( genfile )
        return cell2dict( cell_array )
    
def cell2dict( ca ):
    """
    Parameters:
    -----------
    
    ca : cell array from Matlab, loaded using scipy.io.loadmat()
    
    Returns a Python dictionary
    """
    keys = ca.keys()
    # there should only be one name for the cell array, the other keys
    # should be metadata (eg., '__globals__', etc.)
    name = [ k for k in keys if not k.startswith('__') ][0]
    gens = ca[name][0]
    gdict = {}
    for r,gen in enumerate( gens ):
        # Don't record regions with no generators
        if gen.shape == (0,0):
            #gdict[r] = None
            continue
        else:
        # shift to align with 0-index
            g = gen[0] -1
            if len( g )==0:
                continue
            gdict[r] = set( g )
    return gdict

def invert_dictionary( d )
    inv_map = {}
    for k, v in d.iteritems():
            inv_map[v] = inv_map.get(v, [])
            inv_map[v].append(k)
    return inv_map

