"""
im_graphs.py

Opened: Feb. 10, 2012

Author: Jesse Berwald

Module containing wrappers and algorithms for index map information
and subroutines. Index_Map is a wrapper around the DiGraph class,
which inherits from NetworkX's DiGraph class, with (probably) some
conversion methods.
"""
from graphs import digraph

  
####################
## INDEX_MAP
####################
class Index_Map( digraph.DiGraph ):
    """
    A representation of the map induced on homology. That is,

    f_{P*} : H_{*}(P_1,P_0) ---> H_{*}(P_1,P_0),

    where (P_1,P_0) form an index pair. 

    Inherits from
    ----------

    DiGraph

    Parameters
    ---------

    See DiGraph class
    """
    def __init__( self, **kwargs ):        
        fargs = {'graph' : None,
                 'generators' : None,
                 'genfile' : None,
                 'name' : 'Index Map on homology',
                 'debug' : False
                 }
        fargs.update( kwargs )

        # initialize DiGraph
        digraph.DiGraph.__init__( self, **fargs )
        self.debug = fargs['debug']

        # set the info attribute
        self._update_info()
                    
    def __repr__( self ):
        return self.info

    def _update_info( self ):
        self.info = "Index map on "+ str( len(self) )+" generators."
                
    def trim_generators( self, genfile=None ):
        """
        Trim generators to get rid of those that have been removed
        (i.e. the non-invariant generators).  Generators are stored in
        a dictionary keyed by region. Each region is a disjoint index
        pair that is a component of the isolating neighborhood
        computed by the analysis on the original map F on phase space.

        Eg.,

        {0: set([1]),
        1: set([2, 3]),
        2: set([4])}

        lists a potential generators 1,2,3,4, that are supported on
        regions N_0, N_1, and N_2.

        Parameters
        ---------

        genfile : path to a dictionary or matlab cell array containing
        the generator hash.
        """
        # fill this in later
        if not self.generators:
            pass
        # generators have already been initialized
        else:
            pop = []
            for k in self.generators:
                try:
                    gens = self.generators[k].difference( self.non_mis_nodes )
                    # record transient gen keys; can't pop them here
                    # or it changes dict length mid-loop
                    if len( gens )==0:
                        pop.append( k )
                # quietly pass over regions with no generators. 
                except AttributeError:
                    continue
            # Get rid of keys associated with transient generators
            for k in pop: self.generators.pop( k )

    def _generators2regions( self ):
        """
        Map the region number that each generator/node resides in to a
        node property. This maps the keys of self.generators to the
        nodes.
        """
        for region in self.generators:
            for gen in self.generators[region]:
                self.node[gen] = {'region': region}
                
    def remove_transient_generators( self, copy=False,  only_scc=False ):
        """
        Construct a graph that is asymtotically 'similar' to the
        original. If the index map F is represented by a matrix, we
        seek a change of basis that yields a matrix that is shift
        equivalent to the orginal index map F. Graphically, this is
        equivalent to 1) finding the strongly connected components; 2)
        trimming the nodes that are not both forward and backward
        invariant. (This uses the graph algorithms developed in Lemmas
        3.2-3.4 of Day, et al., to arrive at a graph representation of
        the matrix A.

        Parameters
        ---------

        k : maximum length of path forward and backward paths to
        search. Default=-1, corresponds to infintiy.

        copy : copy the old index map graph to self.full_index_map prior to 

        only_scc : Only find the strongly conected components and list
        of indices with single nodes removed.
        """
        # first find the strongly connected components
        if only_scc:
            self.scc_list, self.scc_idx = GM.scc_raf( self )
        elif copy:
            # store the original, full index map before performing
            # shift equivalence reduction
            self.full_index_map = self.copy()
        # use graph algorithms to find the invariant regions and
        # corresponding shift equivalence
        mis_nodes = self.graph_mis()
        # trim off noninvariant nodes
        non_mis_nodes = set( [u for u in self
                              if u not in mis_nodes] )
        self.remove_nodes_from( non_mis_nodes )
        # update the __repr__ information
        self._update_info()
        
     
    def contract_index_map( self ):
        """
        Create a block model of the Index Map by contracting
        nodes/generators based on regions in which they reside. I.e.,
        if generators i and j are both from region r, contract nodes i
        and j to a single node.

        Returns Index_Map graph of the block model of self
        """
        # map the regions to the nodes
        self._generators2regions()
        node_map = nx.get_node_attributes( self, 'region' )
        inv_map = {}
        for k, v in node_map.iteritems():
            inv_map[v] = inv_map.get(v, [])
            inv_map[v].append(k)
        self.blocks = list( chain(inv_map.itervalues()) )
        # use modified blockmodel() from utils
        return DiGraph( graph=utils.blockmodel( self, self.blocks ),
                        name='Contracted Map' )
    

if __name__ == "__main__":

    import numpy as np
    from pylab import *

    matlab = False
    npy = True
    graph=False
    make_plot = False
    
    if matlab:
        matfile = '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/hom_map.mat'
        genfile =  '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/gens.mat'
        IM = Index_Map( matfile=matfile, genfile=genfile )     
    elif npy:
        idxfile = '/Users/jberwald/Dropbox/Projects/entropy/rads/src/'\
            'symbolics/debug/index_map.npy'
        genfile =  '/Users/jberwald/Dropbox/Projects/entropy/ds.bak/gens.mat'
        print "creating Index_Map object from", idxfile
        print ""
        IM = Index_Map( npyfile=idxfile, genfile=genfile )
    elif graph:
        print "creating Index_Map object from", G
        print ""
        IM = Index_Map( graph=G )

    print "index map constructed with", len(IM), "generators"
    # draw the graph before finding shift equivalence
    if make_plot:
        fig = figure()
        ax = fig.gca()
        #IM.draw_graph( ax=ax )
        pos = nx.spring_layout( IM )
        G = IM.copy()
        nx.draw_networkx( G, pos=pos, ax=ax, alpha=0.7 )

    # Compute shift equivalence 
    IM.remove_transient_generators( copy=True )
    IM.trim_generators()
    cim = IM.contract_index_map()
    print "index map after shift equivalence now has", len(IM), "generators"
    # draw the new, condensed graph
    if make_plot:
        nx.draw_networkx_nodes( G, pos=pos, nodelist=IM.nodes(), node_color='g', alpha=0.8 )
        fig2 = figure()
        ax2 = fig2.gca()
        IM.draw_graph( ax=ax2, nodelist=IM.nodes(), node_color='g', alpha=0.7 )

        figure()
        cim.draw_graph( node_color='b', alpha=0.7 )
        
        show()
