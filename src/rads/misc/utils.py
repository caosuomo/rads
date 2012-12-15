"""
utils.py

Author: Jesse Berwald

Opened: Feb. 15, 2012

A wrapper for various utility functions. Meant to be subclassed.
"""
import networkx as nx
from scipy.io import loadmat

class Utils( object ):

    def convert_matlab_gens( self, genfile ):
        """
        Convert a Matlab (R) cell array to a dictionary. 
        """
        try:
            from scipy.io import loadmat
        except ImportError( "scipy.io.loadmat not found. Is scipy installed?" ):
            raise
        cell_array = loadmat( genfile )
        return self.cell2dict( cell_array )
    
    def cell2dict( self, ca ):
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
