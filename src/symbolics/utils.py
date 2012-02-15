"""
utils.py 
"""

try:
    from scipy.io import loadmat
except ImportError:
    print "scipy.io.loadmat not found"
    raise

class Utils( object ):

    def convert_matlab_gens( genfile ):
        """
        Convert a Matlab (R) cell array to a dictionary. 
        """
        try:
            from scipy.io import loadmat
        except ImportError( "scipy.io.loadmat not found. Is scipy installed?" ):
            raise
        cell_array = loadmat( genfile )
        return cell2dict( cell_array )
    
    def cell2dict( ca ):
        """
        """
        keys = ca.keys()
        # there should only be one name for the cell array, the other keys
        # should be metadata (eg., '__globals__', etc.)
        name = [ k for k in keys if not k.startswith('__') ][0]
        gens = ca[name][0]
        gdict = {}
        for r,gen in enumerate( gens ):
            if gen.shape == (0,0):
                gdict[r] = None
            else:
                gdict[r] = gen[0]
        return gdict
