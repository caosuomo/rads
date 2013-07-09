import networkx as nx
from rads.graphs import Graph, DiGraph
import itertools

def condensation( G, components ):
	"""Return the condensation of G with respect to the given components.

	Given components C_1 .. C_k which partition the nodes of G, the
	condensation graph cG has nodes <C_1> .. <C_k> and has edge
	(<C_i>,<C_j>) iff there was an edge in G from a node in C_i to a
	node in C_j.
	
	Note: the given components can be arbitrary, but if they are the
	strongly connected components (in the graph theory sense, not the
	dynamics sense) of G, then cG corresponds to the directed acyclic
	graph between strongly connected components.

	Parameters
	----------
	G : DiGraph

	components : list of lists of component nodes.

	Returns
	-------
	cG : DiGraph
	   The condensed graph.
	"""
	mapping = dict([(n,c) for c in range(len(components)) for n in components[c]])
	cG = DiGraph()
	for u in mapping:
		cG.add_node(mapping[u])
		for _,v in G.graph.edges_iter(u, data=False):
			if v not in components[mapping[u]]:
				cG.add_edge(mapping[u], mapping[v])
	return cG

def condensation_nx( G, components) :
	"""
	G : DiGraph object. the nx.DiGraph attribute is extracted from this.

	components : 	Given components C_1 .. C_k which partition the nodes of G, the
	condensation graph cG has nodes <C_1> .. <C_k> and has edge
	(<C_i>,<C_j>) iff there was an edge in G from a node in C_i to a
	node in C_j.
	"""
	cG = DiGraph()
	cG.graph = nx.condensation( G.graph, components )
	return cG

def graph_mis( G, return_rsccs=False ):
	"""Return the maximal invariant set of a graph G.

	Given a graph G, the maximal invariant set (MIS) is the maximal
	subset S of nodes such that for every node v in S, there exists a
	bi-infinite walk in G which passes through v.

	Parameters
	----------
	G : DiGraph

	Returns
	-------
	S : list
	   The maximal invariant set of G.
	"""
	if len(G)==0:
		return []
	sccs,rsccs = scc_raf( G )
	#C = condensation( G, sccs )
        # returns RADS digraph
	#C = condensation_nx( G, sccs )
        C = condensation( G, sccs )
	forward = set( descendants( C, rsccs ) )
	rC = C.reverse()
	backward = set( descendants( rC, rsccs ) )
	cnodes = forward & backward
	if return_rsccs:
		return list(itertools.chain(*[sccs[c] for c in cnodes])), sccs, rsccs
	else:
		return list(itertools.chain(*[sccs[c] for c in cnodes]))
	
def descendants(G,S):
	"""Decendents of S (subset of nodes) in G"""
	G_nx = G.graph # get right to nx
	n = len(G_nx)
	G_nx.add_node(n)
	for s in S:
		G_nx.add_edge(n,s)
	d = list( nx.algorithms.dfs_postorder_nodes(G_nx,n) )
	d.remove(n)		# TODO: make more efficient!
	# JJB - since dfs_postorder_nodes return a generator, we must
	# keep the 'star' node n in until the generator is complete
	# RMF - casting to a list for now, for simplicity
	# JJB - when casting to a list, make sure to close your parens ;)
	G_nx.remove_node(n)
	return d

def first_return_times( k, backwards=False ):
	"""
	RMF: UPDATE

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

def scc_raf(G):
	"""Return nodes in strongly connected components of graph.

	Parameters
	----------
	G : DiGraph
	   An directed graph.

	Returns
	-------
	comp : list of lists
	   A list of nodes for each component of G.
	   The list is ordered from largest connected component to smallest.

	See Also	   
	--------
	connected_components

	Notes
	-----
	Uses Tarjan's algorithm with Nuutila's modifications.
	Nonrecursive version of algorithm.
	"""
	G_nx = G.graph
	preorder={}
	lowlink={}	 
	scc_found={}
	scc_queue = []
	scc_list=[]
	real_scc_inds=[]	# RAF
	i=0		# Preorder counter
	for source in G_nx:
		if source not in scc_found:
			queue=[source]
			while queue:
				v=queue[-1]
				if v not in preorder:
					i=i+1
					preorder[v]=i
				done=1
				v_nbrs=G_nx[v]
				for w in v_nbrs:
					if w not in preorder:
						queue.append(w)
						done=0
						break
				if done==1:
					lowlink[v]=preorder[v]
					for w in v_nbrs:
						if w not in scc_found:
							if preorder[w]>preorder[v]:
								lowlink[v]=min([lowlink[v],lowlink[w]])
							else:
								lowlink[v]=min([lowlink[v],preorder[w]])
					queue.pop()
					if lowlink[v]==preorder[v]:
						scc_found[v]=True
						scc=[v]
						while scc_queue and preorder[scc_queue[-1]]>preorder[v]:
							k=scc_queue.pop()
							scc_found[k]=True
							scc.append(k)
						# RAF: changed here to exclude 'transient' SCCs
						# thus, single vertices are SCCs iff they are loops
						scc_list.append(scc)
						if len(scc)>1 or G_nx.has_edge(scc[0],scc[0]):
							real_scc_inds.append(len(scc_list)-1) # index of the scc
					else:
						scc_queue.append(v)
	# RAF: removed sorting... don't really need it
	# scc_list.sort(key=len,reverse=True)			
	return scc_list, real_scc_inds
	
def blockmodel(G,partitions,multigraph=False):
    """
    Returns a reduced graph constructed using the generalized block
    modeling technique.

    The blockmodel technique collapses nodes into blocks based on a
    given partitioning of the node set.  Each partition of nodes
    (block) is represented as a single node in the reduced graph.

    Edges between nodes in the block graph are added according to the
    edges in the original graph.  If the parameter multigraph is False
    (the default) a single edge is added with a weight equal to the
    sum of the edge weights between nodes in the original graph The
    default is a weight of 1 if weights are not specified.  If the
    parameter multigraph is True then multiple edges are added each
    with the edge data from the original graph.

    Parameters
    ----------
    G : graph
        A networkx Graph or DiGraph
    partitions : list of lists, or list of sets 
        The partition of the nodes.  Must be non-overlapping.
    multigraph : bool, optional
        If True return a MultiGraph with the edge data of the original
        graph applied to each corresponding edge in the new graph.  If
        False return a Graph with the sum of the edge weights, or a
        count of the edges if the original graph is unweighted.

    Returns
    -------
    blockmodel : (a Networkx graph object) -> JJB -- a digraph.Digraph object
    
    Examples
    --------
    >>> G=nx.path_graph(6)
    >>> partition=[[0,1],[2,3],[4,5]]
    >>> M=nx.blockmodel(G,partition)

    References
    ----------
    .. [1] Patrick Doreian, Vladimir Batagelj, and Anuska Ferligoj
    	"Generalized Blockmodeling",Cambridge University Press, 2004.
    """
    # Create sets of node partitions
    part=list(map(set,partitions))

    # account for digraph structure
    #    G = G.graph

    # Check for overlapping node partitions
    u=set()
    for p1,p2 in zip(part[:-1],part[1:]):
        u.update(p1)
        #if not u.isdisjoint(p2):  # Python 2.6 required
        if len (u.intersection(p2))>0:
            raise nx.NetworkXException("Overlapping node partitions.")

    # Initialize blockmodel graph
    if multigraph:
        if G.is_directed():
            M=nx.MultiDiGraph() 
        else:
            M=nx.MultiGraph() 
    else:
        if G.is_directed():
	    # JJB - Make the block graph a rads DiGraph or Graph
	    M = nx.DiGraph()
	# M=nx.DiGraph() 
        else:
            M=nx.Graph() 
        
    # Add nodes and properties to blockmodel            
    # The blockmodel nodes are node-induced subgraphs of G
    # Label them with integers starting at 0
    for i,p in zip(range(len(part)),part):
        M.add_node(i)
        # The node-induced subgraph is stored as the node 'graph' attribute 
        SG=G.subgraph(p)
        M.node[i]['graph']=SG        
        M.node[i]['nnodes']=SG.number_of_nodes()
        M.node[i]['nedges']=SG.number_of_edges()
	## JJB -- don't need this
        # M.node[i]['density']=nx.density(SG)
        
    # Create mapping between original node labels and new blockmodel node labels
    block_mapping={}
    for n in M:
        nodes_in_block=M.node[n]['graph'].nodes()
        block_mapping.update(dict.fromkeys(nodes_in_block,n))

    # Add edges to block graph 
    for u,v,d in G.edges(data=True):
        bmu=block_mapping[u]
        bmv=block_mapping[v]
        if bmu==bmv: # no self loops
            continue
        if multigraph: 
            # For multigraphs add an edge for each edge in original graph
            M.add_edge(bmu,bmv,attr_dict=d)
        else:
            # For graphs and digraphs add single weighted edge
            weight=d.get('weight',1.0) # default to 1 if no weight specified
            if M.has_edge(bmu,bmv):
                # JJB - added 'abs' to deal with weights +1, -1 giving weight=0. 
                M[bmu][bmv]['weight']+= abs(weight)
            else:
                M.add_edge(bmu,bmv,weight=abs(weight))
    return M
    

