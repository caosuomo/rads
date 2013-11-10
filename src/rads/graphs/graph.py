import networkx as nx
import matplotlib.pyplot as plt

class Graph( object ):
	"""
	Base class for undirected graphs.
	"""
	def __init__( self, **kwargs ):
		"""
		Initialize a Graph from one of the following formats:

		numpy adjacency array

		Networkx Graph

		kwargs:
		----

		Note: The orientation of the map should be stored in the array
		values. Eg., (i,j) = -1 => directed edge from i -> j. The
		value -1 is stored as an edge weight. 
		"""
		self.graph = nx.Graph(**kwargs)
	
	@property
	def name(self):
		return self.graph.name
	
	@name.setter
	def name(self, s):
		self.graph.name=s

	def __repr__(self):
		return "rads Graph object on %i nodes and %i edges" % (
			self.number_of_nodes(), self.number_of_edges() )

	def __str__(self):
		"""Return the graph name.

		Returns
		-------
		name : string
			The name of the graph.

		Examples
		--------
		>>> G = Graph(name='foo')
		>>> str(G)
		'foo'
		"""
		return self.name

	def __iter__(self):
		"""Iterate over the nodes. Use the expression 'for n in G'.

		Returns
		-------
		niter : iterator
			An iterator over all nodes in the graph.
		"""
		return self.graph.__iter__()

	def __contains__(self,n):
		"""Return True if n is a node, False otherwise. Use the expression
		'n in G'.
		"""
		return (n in self.graph)

	def __len__(self):
		"""Return the number of nodes. Use the expression 'len(G)'.

		Returns
		-------
		nnodes : int
			The number of nodes in the graph.
		"""
		return len(self.graph)

	def add_node(self, n, attr_dict=None, **attr):
		"""Add a single node n and update node attributes.

		Parameters
		----------
		n : node
			A node can be any hashable Python object except None.
		attr_dict : dictionary, optional (default= no attributes)
			Dictionary of node attributes.	Key/value pairs will
			update existing data associated with the node.
		attr : keyword arguments, optional
			Set or change attributes using key=value.

		See Also
		--------
		add_nodes_from

		Examples
		--------
		>>> G = Graph()	  # or DiGraph
		>>> G.add_node(1)
		>>> G.add_node('Hello')
		>>> K3 = Graph([(0,1),(1,2),(2,0)])
		>>> G.add_node(K3)
		>>> G.number_of_nodes()
		3

		Use keywords set/change node attributes:

		>>> G.add_node(1,size=10)
		>>> G.add_node(3,weight=0.4,UTM=('13S',382871,3972649))

		Notes
		-----
		A hashable object is one that can be used as a key in a Python
		dictionary. This includes strings, numbers, tuples of strings
		and numbers, etc.
		"""
		self.graph.add_node(n, attr_dict, **attr)


	def add_nodes_from(self, nodes, **attr):
		"""Add multiple nodes.

		Parameters
		----------
		nodes : iterable container
			A container of nodes (list, dict, set, etc.).
			OR
			A container of (node, attribute dict) tuples.
			Node attributes are updated using the attribute dict.
		attr : keyword arguments, optional (default= no attributes)
			Update attributes for all nodes in nodes.
			Node attributes specified in nodes as a tuple
			take precedence over attributes specified generally.

		See Also
		--------
		add_node

		Examples
		--------
		>>> G = Graph()	  # or DiGraph
		>>> G.add_nodes_from('Hello')
		>>> K3 = Graph([(0,1),(1,2),(2,0)])
		>>> G.add_nodes_from(K3)
		>>> sorted(G.nodes(),key=str)
		[0, 1, 2, 'H', 'e', 'l', 'o']

		Use keywords to update specific node attributes for every node.

		>>> G.add_nodes_from([1,2], size=10)
		>>> G.add_nodes_from([3,4], weight=0.4)

		"""
		self.graph.add_nodes_from(nodes, **attr)

	def remove_node(self, n):
		"""Remove node n.

		Removes the node n and all adjacent edges.
		Attempting to remove a non-existent node will raise an exception.

		Parameters
		----------
		n : node
		   A node in the graph

		Raises
		-------
		NetworkXError
		   If n is not in the graph.

		See Also
		--------
		remove_nodes_from
		"""
		self.graph.remove_node(n)

	def remove_nodes_from(self, nbunch):
		"""Remove multiple nodes.

		Parameters
		----------
		nodes : iterable container
			A container of nodes (list, dict, set, etc.).  If a node
			in the container is not in the graph it is silently
			ignored.

		See Also
		--------
		remove_node
		"""
		self.graph.remove_nodes_from(nbunch)

	def add_edge(self, u, v, attr_dict=None, **attr):
		"""Add an edge between u and v.

		The nodes u and v will be automatically added if they are
		not already in the graph.

		Edge attributes can be specified with keywords or by providing
		a dictionary with key/value pairs.	See examples below.

		Parameters
		----------
		u,v : nodes
			Nodes can be, for example, strings or numbers.
			Nodes must be hashable (and not None) Python objects.
		attr_dict : dictionary, optional (default= no attributes)
			Dictionary of edge attributes.	Key/value pairs will
			update existing data associated with the edge.
		attr : keyword arguments, optional
			Edge data (or labels or objects) can be assigned using
			keyword arguments.

		See Also
		--------
		add_edges_from : add a collection of edges

		Notes
		-----
		Adding an edge that already exists updates the edge data.

		Examples
		--------
		The following all add the edge e=(1,2) to graph G:

		>>> G = Graph()	  # or DiGraph
		>>> e = (1,2)
		>>> G.add_edge(1, 2)		   # explicit two-node form
		>>> G.add_edge(*e)			   # single edge as tuple of two nodes
		>>> G.add_edges_from( [(1,2)] ) # add edges from iterable container

		Associate data to edges using keywords:

		>>> G.add_edge(1, 2, weight=3)
		>>> G.add_edge(1, 3, weight=7, capacity=15, length=342.7)
		"""
		self.graph.add_edge(u,v,attr_dict,**attr)

	def add_edges_from(self, ebunch, attr_dict=None, **attr):
		"""Add all the edges in ebunch.

		Parameters
		----------
		ebunch : container of edges
			Each edge given in the container will be added to the
			graph. The edges must be given as as 2-tuples (u,v) or
			3-tuples (u,v,d) where d is a dictionary containing edge
			data.
		attr_dict : dictionary, optional (default= no attributes)
			Dictionary of edge attributes.	Key/value pairs will
			update existing data associated with each edge.
		attr : keyword arguments, optional
			Edge data (or labels or objects) can be assigned using
			keyword arguments.


		See Also
		--------
		add_edge : add a single edge

		Notes
		-----
		Adding the same edge twice has no effect but any edge data
		will be updated when each duplicate edge is added.

		Examples
		--------
		>>> G = Graph()	  # or DiGraph
		>>> G.add_edges_from([(0,1),(1,2)]) # using a list of edge tuples
		>>> e = zip(range(0,3),range(1,4))
		>>> G.add_edges_from(e) # Add the path graph 0-1-2-3

		Associate data to edges

		>>> G.add_edges_from([(1,2),(2,3)], weight=3)
		>>> G.add_edges_from([(3,4),(1,4)], label='WN2898')
		"""
		self.graph.add_edges_from(ebunch, attr_dict, **attr)

	def remove_edge(self, u, v):
		"""Remove the edge between u and v.

		Parameters
		----------
		u,v: nodes
			Remove the edge between nodes u and v.

		Raises
		------
		NetworkXError
			If there is not an edge between u and v.

		See Also
		--------
		remove_edges_from : remove a collection of edges
		"""
		self.graph.remove_edge(u,v)

	def remove_edges_from(self, ebunch):
		"""Remove all edges specified in ebunch.

		Parameters
		----------
		ebunch: list or container of edge tuples
			Each edge given in the list or container will be removed
			from the graph. The edges can be:

				- 2-tuples (u,v) edge between u and v.
				- 3-tuples (u,v,k) where k is ignored.

		See Also
		--------
		remove_edge : remove a single edge

		Notes
		-----
		Will fail silently if an edge in ebunch is not in the graph.
		"""
		self.graph.remove_edges_from(ebunch)


	def neighbors(self, n):
		"""Return a list of the nodes connected to the node n.

		Parameters
		----------
		n : node
		   A node in the graph

		Returns
		-------
		nlist : list
			A list of nodes that are adjacent to n.

		Raises
		------
		NetworkXError
			If the node n is not in the graph.

		Notes
		-----
		It is usually more convenient (and faster) to access the
		adjacency dictionary as G[n]:

		>>> G = nx.Graph()	 # or DiGraph, MultiGraph, MultiDiGraph, etc
		>>> G.add_edge('a','b',weight=7)
		>>> G['a']
		{'b': {'weight': 7}}

		Examples
		--------
		>>> G = nx.Graph()	 # or DiGraph, MultiGraph, MultiDiGraph, etc
		>>> G.add_path([0,1,2,3])
		>>> G.neighbors(0)
		[1]

		"""
		return self.graph.neighbors(n)
	
	def has_edge(self, u, v):
		"""Return True if the edge (u,v) is in the graph.

		Parameters
		----------
		u,v : nodes
			Nodes can be, for example, strings or numbers.
			Nodes must be hashable (and not None) Python objects.

		Returns
		-------
		edge_ind : bool
			True if edge is in the graph, False otherwise.
		"""
		return self.graph.has_edge(u,v)

	def nodes( self, data=False ):
		"""
		Return a list of the nodes in the graph.

		Parameters
		----------
		data : boolean, optional (default=False)
		       If False return a list of nodes.  If True return a
		       two-tuple of node and node data dictionary

		Returns
		-------
		nlist : list
		    A list of nodes.  If data=True a list of two-tuples containing
		    (node, node data dictionary).

		Examples
		--------
		>>> G = nx.Graph()   # or DiGraph, MultiGraph, MultiDiGraph, etc
		>>> G.add_path([0,1,2])
		>>> G.nodes()
		[0, 1, 2]
		>>> G.add_node(1, time='5pm')
		>>> G.nodes(data=True)
		[(0, {}), (1, {'time': '5pm'}), (2, {})]
		"""
		return self.graph.nodes( data )
		
	def edges(self, nbunch=None, data=False):
		"""Return a list of edges.

		Edges are returned as tuples with optional data
		in the order (node, neighbor, data).

		Parameters
		----------
		nbunch : iterable container, optional (default= all nodes)
			A container of nodes.  The container will be iterated
			through once.
		data : bool, optional (default=False)
			Return two tuples (u,v) (False) or three-tuples (u,v,data) (True).

		Returns
		--------
		edge_list: list of edge tuples
			Edges that are adjacent to any node in nbunch, or a list
			of all edges if nbunch is not specified.

		Notes
		-----
		Nodes in nbunch that are not in the graph will be (quietly) ignored.
		"""
		return self.graph.edges(nbunch, data)

	def number_of_nodes(self):
		"""Return the number nodes in a graph."""
		return self.graph.number_of_nodes()

	def number_of_edges(self):
		"""Return number of edges in a graph."""
		return self.graph.number_of_edges()


	def edges_iter( self, nbunch=None, data=False):
		"""
		Returns an iterator over the edges. See edges for args.
		"""
		return self.graph.edges_iter( nbunch, data )

        def subgraph( self, nbunch ):
		"""
		Return the subgraph induced on nodes in nbunch in the
		form of a Graph() object.

		Parameters
		----------
		nbunch : list, iterable
		   A container of nodes that will be iterated through once (thus
		   it should be an iterator or be iterable).  Each element of the
		   container should be a valid node type: any hashable type except
		   None.  If nbunch is None, return all edges data in the graph.
		   Nodes in nbunch that are not in the graph will be (quietly)
		   ignored.
		"""
		subG = self.graph.subgraph( nbunch )
		S = Graph()
		S.graph = subG
		return S

	def clear(self):
		"""Remove all nodes and edges from the graph.

		This also removes the name, and all graph, node, and edge attributes.
		"""
		self.graph.clear()

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
		G = Graph()
		copy_self = self.graph.copy()
		G.add_edges_from( copy_self.edges( data=True ) ) 
		return G


	def deepcopy(self):
		"""Return a deep copy of the graph.

		Returns
		-------
		G : Graph
			A deepcopy of the graph.

		Notes
		-----
		This returns a "deepcopy" of the edge, node, and
		graph attributes which attempts to completely copy
		all of the data and references.

		See the Python copy module for more information on shallow
		and deep copies, http://docs.python.org/library/copy.html.
		"""
		return deepcopy(self)

	def to_numpy_matrix( self, **kwargs ):
		"""
		Returns the adjacency matrix of self.graph as a numpy
		matrix.
		"""
		return nx.to_numpy_matrix( self.graph, **kwargs )

	def draw( self, **kwargs ):
		"""
		Draw graph using NetworkX. See nx.draw_networkx and friends
		for kwargs.

		Defaults used for drawing:

		node_size : 60
		alpha : 0.8
		node_color : 'g' (green)
		with_labels : False

		Use keyword args to change these and any others
		accepted by networkx.draw_networkx().
		"""
		args = { 'node_size' : 60,
			 'alpha' : 0.8,
			 'node_color' : 'g',
			 'with_labels' : False
			 }
		args.update( kwargs )

		fig = plt.figure()
		ax = fig.gca()
		limits = ax.axis( 'off' )
		pos = nx.graphviz_layout( self.graph )
		nx.draw_networkx( self.graph, pos=pos, ax=ax, **args )
		fig.show()
		return fig

	def save( self, **kwargs ):
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
        
