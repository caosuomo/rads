"""
im_graphs.py

Opened: Feb. 10, 2012

Author: Jesse Berwald

Module containing graph wrappers and algorithms for index map
information and subroutines. DiGraph is mostly a convenience wrapper
around NetworkX's DiGraph class, with (probably) some conversion
methods.
"""
import networkx as nx
import numpy as np
import graph_mis_raf as GM
import utils
from itertools import chain

####################
## DiGraph
####################
class DiGraph( nx.DiGraph, utils.Utils ):
    """
    Base class for graph algorithms. Inherit all of NetworkX DiGraph's
    methods.
    """
    def __init__( self, **kwargs ):
        """
        Initialize a NetworkX DiGraph from one of the following formats:

        numpy adjacency array

        Matlab adjacency array, possibly sparse.

        edge bunch (NOT IMPLEMENTED)
        
        args:
        ----

        ****file : path to appropriate file

        Note: The orientation of the map should be stored in the array
        values. Eg., (i,j) = -1 => directed edge from i -> j. The
        value -1 is stored as an edge weight. 
        """
        fargs = {'matfile' : None,
                 'pklfile' : None,
                 'npyfile' : None,
                 'name' : '',
                 'ebunch' : None,
                 'graph' : None
                 }
        fargs.update( kwargs )

        # load edge set/index map from possible file
        if fargs['npyfile']:
            # adjacency matrix, weighted edges 
            self.adj_matrix = np.load( fargs['npyfile'] )
            fargs['data'] = self.adj_matrix
            nx.DiGraph.__init__( self, data=self.adj_matrix, name=fargs['name'] )
            #super( DiGraph, self ).__init__( **fargs ) #data=self.adj_matrix, name=fargs['name'] )
        elif fargs['pklfile']:
            print "not implemented"
        elif fargs['matfile']:
            self._loadmat( fargs['matfile'] )
            # init graph with index map matrix. Weights pulled from matrix entries.
            nx.DiGraph.__init__( self, data=self.adj_matrix, name=fargs['name'] )
        elif fargs['ebunch']:
            nx.DiGraph.__init__( self )
            self.add_weighted_edges_from( fargs['ebunch'] )
        elif fargs['graph']:
            nx.DiGraph.__init__( self, data=fargs['graph'] )
        else:
            nx.DiGraph.__init__( self )
        utils.Utils.__init__( self )
     
    def _loadmat( self, fname ):
        """
        Load a Matlab matrix.
        """
        try:
            from scipy.io import loadmat
        except ImportError:
            print "scipy.io.loadmat not found"
            raise
        mat = loadmat( fname, mat_dtype=True )
        keys = mat.keys()
        # sort forces the matrix name to the last entry
        keys.sort()
        mat_name = keys[-1]
        self.adj_matrix = mat[mat_name].toarray()

    def condensation( self, sccs ):
	"""Returns the condensation of G.

	The condensation of G is the graph with each of the strongly connected
	components contracted into a single node.

	Parameters
	----------
	G : NetworkX Graph
	   A directed graph.

        sccs : list of lists of connected component nodes.

	Returns
	-------
	cG : NetworkX DiGraph
	   The condensation of G.

	Notes
	-----
	After contracting all strongly connected components to a single node,
	the resulting graph is a directed acyclic graph.
	"""
	mapping = dict([(n,c) for c in range(len(sccs)) for n in sccs[c]])
	#cG = nx.DiGraph()
	cG = DiGraph() #Index_Map()
	for u in mapping:
		cG.add_node(mapping[u])
		for _,v,d in self.edges_iter(u, data=True):
			if v not in sccs[mapping[u]]:
				cG.add_edge(mapping[u], mapping[v], d)
	return cG

    def graph_mis( self ):
        """
        Maximal invariant set for the map on homology. Determined
        using the strongly connected components of Index_Map, and
        trimming off the nodes that cannot be verified to 'return'.

        Note: We may get more 'bang for the buck' using two calls to
        nx.attracting_components() instead of scc + 2* descendents()
        """
	sccs,rsccs = GM.scc_raf( self )
 	self.condensed = self.condensation( sccs )
	# JJB - Descendant generators converted to lists to slice off
	# last (source) index. Then convert back to sets for intersection.
        C = self.condensed
	n = len( C )
	forward = set( list( GM.descendants( C,rsccs ) ) )
        # JJB - reset C: descendants() add a 'star' node
	C.remove_node( n )
	backward = set( list( GM.descendants( C.reverse(copy=False),rsccs ) ) )
	C.remove_node( n )
	cnodes = forward & backward
        cnodes.remove( n )
	self.mis_nodes = list(chain(*[sccs[c] for c in cnodes]))

    def first_return_times( self, k, backwards=False ):
        """
        Look for k-recurrent vertices in the graph of the DiGraph. A
        k-recurrent vertex is a vertex v for which the path v -> v is
        of length <= k.

        Optional Parameters
        ---------

        k : maximum length of path (k+1)

        See nx.all_pairs_shortest_path_length(G,k)
        """
        if backwards:
            G = self.reverse()
            self.backward_return_times = dict()
            rt = self.backward_return_times
        else:
            G = self
            self.forward_return_times = dict()
            rt = self.forward_return_times
	# length = shortest path lengths <= k
	# length[i][j] = length of shortest path i->j, if <= k
        # length[i] a dict keyed by neighbors of node i, with values
        # length of path to j
	length = nx.all_pairs_shortest_path_length( G, k ) 
	for i in G.nodes_iter():
            # nodes = list of successors j which return to i
            nodes = filter( lambda j: length[j].has_key(i), G.successors(i) )
            # distances for each successor j
            distances = [length[j][i]+1 for j in nodes]
            if distances:
                rt[i] = min( distances )
            

    def draw_graph( self, **kwargs ):
        """
        Draw graph using NetworkX. See nx.draw_networkx and friends
        for kwargs.
        """
        nx.draw_networkx( self, **kwargs )

    def save_graph( self, **kwargs ):
        """
        Saves a copy of the graph as a pickled file.
        
        Option args:
        -----------

        fname :

             Name of graph
        """
        fargs = {'fname': None
                 }
        fargs.update( kwargs )

        if not fargs['fname']:
            raise ValueError( "Must provide a file name" )
        nx.write_gpickle( self, fargs['fname'] )
  
####################
## INDEX_MAP
####################
class Index_Map( DiGraph, utils.Utils ):
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

        self.debug = fargs['debug']

        # initialize DiGraph
        DiGraph.__init__( self, **fargs )
        # load generator mapping if available
        if fargs['genfile'] is None and fargs['generators'] is None:
            self.generators = None
        else:
            if fargs['genfile']:
                self.generators = self.convert_matlab_gens( fargs['genfile'] )
            else:
                if not type( fargs['generators'] ) == dict:
                    raise TypeError( "generators "\
                                     "should be a dictionary keyed by region!" )
                else:
                    self.generators = fargs['generators']

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
            self.full_index_map = IM.copy()
        # use graph algorithms to find the invariant regions and
        # corresponding shift equivalence
        self.graph_mis()
        # trim off noninvariant nodes
        self.non_mis_nodes = set( [u for u in self
                                   if u not in self.mis_nodes] )
        self.remove_nodes_from( self.non_mis_nodes )
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

# class Transition_Map( Index_Map ):
#     """
#     Directed graph associated to the symbol transition graph on the
#     disjoint regions in the isolating neighborhood N. Computed from 
#     """
#     def __init__( self, **kwargs ):
#         Index_Map


    

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
