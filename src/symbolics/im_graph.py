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
import itertools


class DiGraph( nx.DiGraph ):
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

        *file : path to appropriate file

        Note: The orientation of the map should be stored in the array
        values. Eg., (i,j) = -1 => directed edge from i -> j. The
        value -1 is stored as an edge weight. 
        """
        fargs = { 'matfile' : None,
                 'pklfile' : None,
                 'npyfile' : None,
                 'name' : '',
                 'ebunch' : None,
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
        else:
            nx.DiGraph.__init__( self )
            
    # def __repr__( self ):
    #     return str("RADS DiGraph")
     
    def _loadmat( self, fname ):
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

    def mat2graph( self ):
        """
        Convert a sparse matrix loaded from MatLab (R) format to a
        directed graph.

        Note: Indices automatically shifted to 0-based by loadmat().

        TODO: test me!
        """
        # nonzero indices
        arr = self.mat.toarray()
        
        # w = np.where( arr != 0 )
        # nz = map( zip( w[0], w[1] ), self.mat )
        # ebunch = deque()
        # for u,v in nnz:
        #     ebunch.append( (u,v,self.mat[u,v]) ) 
        # self.add_weighted_edges_from( ebunch )


####################
# INDEX_MAP
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
                 'name' : 'Index Map on homology'
                 }
        fargs.update( kwargs )

        # user-supplied NetworkX Digraph
        if fargs['graph']:
            DiGraph.__init__( self, **fargs )
            utils.Utils.__init__( self )
            self.add_weighted_edges_from( fargs['graph'] )
            #self.generators = fargs['generators']
        else:
            DiGraph.__init__( self, **fargs )
            utils.Utils.__init__( self )

            
        if fargs['genfile'] is None and fargs['generators'] is None:
            self.generators = None
        else:
            if fargs['genfile']:
                self.generators = self.convert_matlab_gens( fargs['genfile'] )
            else:
                if not type( fargs['generators'] ) == dict:
                    raise TypeError( "generators should be a dictionary keyed by region!" )
                else:
                    self.generators = fargs['generators']

    def __repr__( self ):
        return self.name
    
    def first_return_times( self, k=None, backwards=False ):
        """
        Look for k-recurrent vertices in the graph of the index map. A
        k-recurrent vertex is a vertex v for which the path v -> v is
        of length <= k.

        Optional Parameters
        ---------

        k : maximum length of path

        See NetworkX.all_pairs_shortest_path_length(G,k)
        """
        if backwards:
            G = self.reverse()
            self.backward_return_times = dict()
            rt = self.backward_return_times
        else:
            G = self
            self.forward_return_times = dict()
            rt = self.forward_return_times

        # k should be set in tim_generators, unless we are
        # debugging. Then it should be set 'by hand'.
        if not k:
            try:
                k = self.k_recurrence
            except AttributeError:
                raise
        
	# length = shortest path lengths <= k
	# length[i][j] = length of shortest path i->j, if <= k
        # length[i] a dict keyed by neighbors of node i, with values
        # length of path to j
	length = nx.all_pairs_shortest_path_length( G, k ) #self, k )
	for i in G.nodes_iter():
            # nodes = list of successors j which return to i
            nodes = filter( lambda j: length[j].has_key(i), G.successors(i) )
            # distances for each successor j
            distances = [length[j][i]+1 for j in nodes]
            if distances:
                rt[i] = min( distances )

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
	cG = Index_Map()
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
	self.mis_nodes = list(itertools.chain(*[sccs[c] for c in cnodes]))

    def shift_equivalence( self, k=-1, copy=False,  only_scc=False ):
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
                # quietly pass over regions with
                except AttributeError:
                    continue
            # Get rid of keys associated with transient generators
            for k in pop: self.generators.pop( k )


    def contract_index_map( self ):
        """
        
        """
        pass        
            
    def draw_graph( self, **kwargs ):
        """
        Draw graph using NetworkX. See nx.draw_networkx and friends
        for kwargs.
        """
        nx.draw_networkx( self, **kwargs )

# class Error( Exception ):
#     """
#     Base class for exceptions.
#     """
#     pass

# class DiGraphError( Error ):
#     """
#     Exception raised for errors in DiGraph.

    
#     """
#     def __init__( self, expression, message ):
#         self.expression = expression
#         self.message = message 
                



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
    if make_plot:
        fig = figure()
        ax = fig.gca()
        #IM.draw_graph( ax=ax )
        pos = nx.spectral_layout( IM )
        G = IM.copy()
        nx.draw_networkx( G, pos=pos, ax=ax, alpha=0.7 )

    # Compute shift equivalence 
    IM.shift_equivalence( copy=True )
    IM.trim_generators()
    print "index map after shift equivalence, now has", len(IM), "generators"
    
    if make_plot:
        # fig2 = figure()
        # ax2 = ax #fig2.gca()
        #IM.draw_graph( ax=ax2, nodelist=IM.nodes(), alpha=0.7 )
        nx.draw_networkx_nodes( G, pos=pos, nodelist=IM.nodes(), node_color='g', alpha=0.8 )
        show()
