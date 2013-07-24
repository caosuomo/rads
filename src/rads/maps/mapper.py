#!/usr/bin/python
"""General Mapper class for creating outer approximation. Requires
that Cython version of any map is compiled and in the PYTHONPATH.

author : Jesse Berwald

opened : July 14, 2013
"""
import numpy as np
import networkx as nx
from rads.enclosure import CombEnc,Tree
from rads.maps.tent import TentMapper
from rads.graphs.algorithms import graph_mis
from rads.misc import gfx
from rads.homology import homology as hom

class Mapper( object ):
    
    def __init__( self, root_box, map_obj, depth, **params ):
        """root_box : numpy 2 x d array. Each column of row 0 should contain
        the coordinate of the lower left corner of the root box.

        map_obj: width of the root box in each coordinate dimension.

        depth : The tree will be perform 'depth' subdivisions. Each
        subdivision operation performs a binary partition of the phase
        space in only one dimension.

        params : dictionary object. parameter values for the map in
        map_obj, keyed by parameter names (string values
        only). map_obj must have get_param() and set_param() methods. Eg., 

        # set the parameter
        p = mapper.get_params()
        p['r'][:] = r
        mapper.set_params( p )

        """
        self.box = root_box
        self.mapper = map_obj
        self.depth = depth

        # our tree (with root box), mapper, enclosure
        self.tree = Tree( box, full=True )
        
        # set the parameter
        p = self.mapper.get_params()
        for param in params:
            p[ param ][:] = params[ params ]
        self.mapper.set_params( p )

    def subdivide( self ):
        """Subdivide to specified depth """
        pass
