import networkx as nx
import graph_mis_raf as GM
from itertools import chain
import utils

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
        Initialize a DiGraph from one of the following formats:

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
        # inherit from utils
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
	return list(chain(*[sccs[c] for c in cnodes]))

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
