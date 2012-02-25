import networkx as nx
import graph.Graph

class DiGraph(graph.Graph):
    """
    Base class for directed graphs.
    """
    def __init__( self, **kwargs ):
        """
        Initialize a DiGraph from one of the following formats:

        numpy adjacency array

        args:
        ----

        Note: The orientation of the map should be stored in the array
        values. Eg., (i,j) = -1 => directed edge from i -> j. The
        value -1 is stored as an edge weight. 
        """
		self.graph = nx.DiGraph(**kwargs)

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
