import networkx as nx
from rads.graphs import Graph

class DiGraph( Graph ):
    """
    Base class for directed graphs.
    """
    def __init__( self, **kwargs ):
	"""
	Initialize a DiGraph from one of the following formats:
	
	numpy adjacency array
	
	args:
	----
	
	see nx.DiGraph

        Note: The orientation of the map should be stored in the array
        values. Eg., (i,j) = -1 => directed edge from i -> j. The
        value -1 is stored as an edge weight. 
        """
        self.graph = nx.DiGraph(**kwargs)

    def __repr__(self):
	return "rads DiGraph object on %i nodes and %i edges" % (
		self.number_of_nodes(),self.number_of_edges() )

    def __getitem__( self, u ):
        """Return the successor nodes of u. See nx.Graph.__getitem__() for
        more info.
        """
        return self.graph[u]

    def is_directed(self):
        """Returns True. This is needed by blockmodel."""
        return self.graph.is_directed()
        
    def has_successor(self, u, v):
        """Return True if node u has successor v.

        This is true if graph has the edge u->v.
        """
        return self.graph.has_successor(u,v)

    def has_predecessor(self, u, v):
        """Return True if node u has predecessor v.

        This is true if graph has the edge u<-v.
        """
        return self.graph.has_predecessor(u,v)

    def successors(self, n):
        """Return a list of successor nodes of n."""
        return self.graph.successors(n)

    def predecessors(self, n):
        """Return a list of predecessor nodes of n."""
        return self.graph.predecessors(n)

    def subgraph( self, nbunch ):
        """
        Return DiGraph on nbunch nodes. See Graph object of NetworkX.
        """ 
        subG = self.graph.subgraph( nbunch )
        S = DiGraph()
        S.graph = subG
        return S

    def from_numpy_matrix( self, mat ):
        """
        Interface to NX's function. In-place conversion.
        """
        self.graph = nx.from_numpy_matrix( mat, create_using=self.graph )

    def reverse( self, copy=True ):
        """
        Return the reverse of the graph.

        The reverse is a graph with the same nodes and edges
        but with the directions of the edges reversed.

        Parameters
        ----------
        copy : bool optional (default=True)
            If True, return a new DiGraph holding the reversed edges.
            If False, reverse the reverse graph is created using the
            original graph (this changes the original graph). Returns
            a new RADS DiGraph if True.
        """
        if copy:
            Rnx = self.graph.reverse( copy=copy )
            RG = DiGraph()
            RG.graph = Rnx
            return RG
        else:
            Rnx = self.graph.reverse()
            self.graph = Rnx

    def copy( self ):
	"""
        Return a copy of the graph.

        Returns
        -------
        G : Graph
            A copy of the graph.

        See Also
        --------
        to_directed: return a directed copy of the graph.

        Notes
        -----
        This makes a complete copy of the graph including all of the
        node or edge attributes.

        Examples
        --------
        >>> G = nx.Graph()   # or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> G.add_path([0,1,2,3])
        >>> H = G.copy()
        """
        G = DiGraph()
        copy_self = self.graph.copy()
        G.add_edges_from( copy_self.edges( data=True ) ) 
        return G

    def flag_edges(self, label, flag, ebunch=None):
        """
        Attached an attribute to each edge in order to easily
        include/exclude it during certain computations. Eg., flag
        prohibited edges during cycle verification.

	NOTE: This may not be the best way. It might be better to
	simply remove the edge if we find that it is prohibited.

        If ebunch is not provided, all edges in graph are flagged with
        given label and flag.

        Parameters
        ----------

        label : str, name of the flag

        flag : bool

        ebunch : iterable object containing edges to flag
        (default=None).

        Example
        -------

        Flag edges is equivalent to:

        In [1]: G = nx.path_graph(6)
        Out [2]: for e in G.edges_iter(data=True):
                    e[2]['flag'] = True
        In [3]:  G.edges(data=True)
        Out [3]: [(0, 1, {'flag': True}),
                (1, 2, {'flag': True}),
                (2, 3, {'flag': True}),
                (3, 4, {'flag': True}),
                (4, 5, {'flag': True})]
        """
        if ebunch is None:
            ebunch = G.edges_iter( data=True )
        else:
            ebunch = iter( ebunch )
        for e in ebunch:
            e[2][label] = flag

    def in_edges(self, nbunch=None, data=False):
        """Return a list of the incoming edges.

        See Also
        --------
        edges : return a list of edges
        """
        return self.graph.in_edges(nbunch,data)

    def in_degree(self, nbunch=None, weighted=False):
        """Return the in-degree of a node or nodes.

        The node in-degree is the number of edges pointing in to the node.

        Parameters
        ----------
        nbunch : iterable container, optional (default=all nodes)
            A container of nodes.  The container will be iterated
            through once.
        weighted : bool, optional (default=False)
           If True return the sum of edge weights adjacent to the node.

        Returns
        -------
        nd : dictionary, or number
            A dictionary with nodes as keys and in-degree as values or
            a number if a single node is specified.

        See Also
        --------
        degree, out_degree
        """
	return self.graph.in_degree(nbunch,weighted)

    def out_degree(self, nbunch=None, weighted=False):
        """Return the out-degree of a node or nodes.

        The node out-degree is the number of edges pointing out of the node.

        Parameters
        ----------
        nbunch : iterable container, optional (default=all nodes)
            A container of nodes.  The container will be iterated
            through once.
        weighted : bool, optional (default=False)
           If True return the sum of edge weights adjacent to the node.

        Returns
        -------
        nd : dictionary, or number
            A dictionary with nodes as keys and out-degree as values or
            a number if a single node is specified.
        """
        return self.graph.out_degree(nbunch,weighted)
    
    def remove_nodes_from(self, nbunch):
        """
        Remove multiple nodes.

        Parameters :	
        ------------

        nodes : iterable container

        A container of nodes (list, dict, set, etc.). If a node in the
        container is not in the graph it is silently ignored.
        """
        self.graph.remove_nodes_from( nbunch )

    def first_return_times( self, k ):
        """Computes:
        
        length = shortest path lengths <= k 

        length[i][j] = length of shortest path i->j, if <= k, using
	NX.all_pairs_shortest_path_length

        length[i] is a dict keyed by neighbors of node i, with values
	length of path to j

        Returns dictionary of return times <= k, length dictionary
        described above.
        """
	return_times = dict()

	# length = shortest path lengths <= k
	# length[i][j] = length of shortest path i->j, if <= k
        # length[i] a dict keyed by neighbors of node i, with values
        # length of path to j
	length = nx.all_pairs_shortest_path_length( self.graph, k )
	for i in G.nodes_iter():
		# nodes = list of successors j which return to i
		nodes = filter(lambda j: length[j].has_key(i),G.successors(i))
		# distances for each successor j
		distances = [length[j][i]+1 for j in nodes]
		if distances:
			return_times[i] = min(distances)

	return return_times, length
