import networkx as nx
import itertools

def scc_raf(G):
	"""Return nodes in strongly connected components of graph.

	Parameters
	----------
	G : NetworkX Graph
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
	cdef dict preorder={}
	cdef dict lowlink={}	 
	cdef dict scc_found={}
	cdef list scc_queue = []
	cdef list scc_list=[]
	cdef list real_scc_inds=[]					# RAF
	cdef int i=0		# Preorder counter
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

def condensation(G,sccs):
	"""Returns the condensation of G.

	The condensation of G is the graph with each of the strongly connected
	components contracted into a single node.

	Parameters
	----------
	G : NetworkX Graph
	   A directed graph.

	Returns
	-------
	cG : NetworkX DiGraph
	   The condensation of G.

	Notes
	-----
	After contracting all strongly connected components to a single node,
	the resulting graph is a directed acyclic graph.

	"""
	cdef dict mapping = dict([(n,c) for c in range(len(sccs)) for n in sccs[c]])
	cG = nx.DiGraph()
	for u in mapping:
		cG.add_node(mapping[u])
		for _,v,d in G.edges_iter(u, data=True):
			if v not in sccs[mapping[u]]:
				cG.add_edge(mapping[u], mapping[v])
	return cG

def graph_mis(G):
	sccs,rsccs = scc_raf(G)
	C = condensation(G,sccs)
	cnodes = set(descendents(C,rsccs)) & set(descendents(C.reverse(copy=False),rsccs))
	return list(itertools.chain(*[sccs[c] for c in cnodes]))

cdef list descendents(G,S):
	cdef int n = len(G)
	G.add_node(n)
	for s in S:
		G.add_edge(n,s)
	cdef list d = list(nx.algorithms.dfs_postorder_nodes(G,n))
	G.remove_node(n)
	return d[:-1]

