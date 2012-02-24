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
    Returns a Numpy adjacency matrix with weighted edges.

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
            raise 
        return data
    # string, filename 
    elif hasattr( data, 'join' ):
        try:
            return np.load( data ):
        except IOError:
            pass
    # more to add below....
    else:
        print "oops, made it here!"
 
