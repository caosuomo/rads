"""
Process cubical files A \subset B, X \subset Y, for

F : (X,A) --> (Y,B)
"""
import numpy as np
import subprocess as sp

def index_map_call( fmap, xname, aname, yname, bname, gname=None ):
    """
    Calls homcubes_rads (specialized version of homcubes (R), Pawel
    Pilarczyk), with flag -g in order to write generator regions to
    gname file.

    fmap : Text file, must be of form

    ...
    (-143,-52)  -> {(-148,-43) (-147,-43) (-146,-43) (-145,-43) (-144,-43) }
    ...

    xname : Text file, must be of form

    dimension 2
    (-145,-52)
    (-147,-44)
    ...

    yname, aname, bname : see xname

    gname : path to files in which to write cubical generator homology
    generators of X

    Returns name of temp file to which map homology is saved (i.e. the index map)
    """
    # Assume that *name files have same prefix, with .cub or .map ending
    prefix = fmap[:-3]

    if not gname:
        gname = prefix + 'gen'
    cmd = [ 'homcubes_rads', '-i', '-a',
            fmap, xname, aname, yname, bname,
            '-g', gname ]

    out = sp.check_output( cmd, stderr=sp.STDOUT)

    #return out
    idxmap = _homout2pydict( out )    
    return idxmap

def _homout2pydict( out ):
    """
    Converts homcubes_rads output to a python dictionary.
    """
    # Homcubes still gives a little extra output, so strip it off
    s = out.split( '\n' )[2:-1]
    d = ''.join( s )
    return eval( d )
    
def index_map_to_matrix( idxmap, level ):
    """
    Convert dict format to a numpy array.

    idxmap : dictionary containing map on homology at each homology level.

    level : dimension/homology level at which to construct the map.

    Matrix is constructed as follows: For generator g_i, eg, idx[i] =
    [(a1,j1), (a2,j2)], then fill in column i so that the (j1,i) = a1
    and (j2,i) = a2.
    """
    idxmap = idxmap[ level ]
    size = len( idxmap ) 
    M = np.zeros( (size,size), dtype=np.int8 )

    print "M SHAPE", M.shape

    for gen in idxmap:
        # each key is a single generator (preimage)
        # homcubes numbers generators from 1..
        gnum = gen.keys()[0] - 1 
        imgs = gen.values()
        # put the images in the right slot
        for img in imgs:
            print img
            if len( img ) == 0:
                continue
            for y in img:
                print y
                M[ y[1]-1, gnum ] = y[0]
    return M

def save_index_map( idxmap, fname ):
    """
    idxmap : numpy array

    fname : full path to save idxmap to.
    """
    np.save( fname, idxmap )
        
    

if __name__ == "__main__":

    p = '/Users/jberwald/src/chomp/examples/tmp-AwHXVh-tutorialhenon-'
    fmap = p + 'map.dat'
    xname = p + 'X.dat'
    aname = p + 'A.dat'
    yname = p + 'Y.dat'
    bname = p + 'B.dat'

    idx = index_map_call( fmap, xname, aname, yname, bname ) 
    
        
    
