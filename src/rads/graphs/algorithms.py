import networkx as nx
import itertools

def condense( G, components ):
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
		for _,v,d in G.graph.edges_iter(u, data=True):
			if v not in components[mapping[u]]:
				cG.add_edge(mapping[u], mapping[v], d)
	return cG

def graph_mis( G ):
	"""Return the maximal invariant set of a graph G.

	Given a graph G, the maximal invariant set (MIS) is the maximal
	subset S of nodes such that for every node v in S, there exists a
	bi-infinite walk in G which passes through v.

	NOTE: UNTESTED!!

	Parameters
	----------
	G : DiGraph

	Returns
	-------
	S : list
	   The maximal invariant set of G.
	"""
	sccs,rsccs = scc_raf( G )
	C = condensation( G,sccs )
	forward = set( descendants( C,rsccs ) )
	backward = set( descendants( C.reverse(copy=False),rsccs ) )
	cnodes = forward & backward
	return list(chain(*[sccs[c] for c in cnodes]))

def first_return_times( self, k, backwards=False ):
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
	G : DiGraph Graph
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
	preorder={}
	lowlink={}	 
	scc_found={}
	scc_queue = []
	scc_list=[]
	real_scc_inds=[]					# RAF
	i=0		# Preorder counter
	for source in G:
		if source not in scc_found:
			queue=[source]
			while queue:
				v=queue[-1]
				if v not in preorder:
					i=i+1
					preorder[v]=i
				done=1
				v_nbrs=G[v]
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
						if len(scc)>1 or G.has_edge(scc[0],scc[0]):
							real_scc_inds.append(len(scc_list)-1) # index of the scc
					else:
						scc_queue.append(v)
	# RAF: removed sorting... don't really need it
	# scc_list.sort(key=len,reverse=True)			
	return scc_list,real_scc_inds

def descendants(G,S):
	"""Decendents of S (subset of nodes) in G"""
	G = G.graph # get right to nx
	n = len(G)
	G.add_node(n)
	for s in S:
		G.add_edge(n,s)
	d = list(nx.algorithms.dfs_postorder_nodes(G,n)
	# JJB - since dfs_postorder_nodes return a generator, we must
	# keep the 'star' node n in until the generator is complete
	# RMF - casting to a list for now, for simplicity
	G.remove_node(n)
	return d
