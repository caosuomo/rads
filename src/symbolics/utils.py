 try:
     from scipy.io import loadmat
 except ImportError:
     print "scipy.io.loadmat not found"
     raise

def cell2dict( ca ):
    """
    """
    pass

def convert_matlab_gens( genfile ):
    """
    Convert a Matlab (R) cell array to a dictionary. 
    """
    try:
        from scipy.io import loadmat
    except ImportError:
        print "scipy.io.loadmat not found"
        raise
    cell_array = loadmat( genfile )
    cell2dict( cell_array )
            
