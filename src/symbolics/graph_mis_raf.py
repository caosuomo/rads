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
	n = len(G)
	G.add_node(n)
	for s in S:
		G.add_edge(n,s)
	d = nx.algorithms.dfs_postorder_nodes(G,n)
	# JJB - since dfs_postorder_nodes return a generator, we must
	# keep the 'star' node n in until the generator is complete
	#G.remove_node(n)
	return d

def dfs_postorder_nodes(G,source=None):
    """Produce nodes in a depth-first-search post-ordering starting 
    from source.
    """
    post=(v for u,v,d in dfs_labeled_edges(G,source=source)
          if d['dir']=='reverse')
    # chain source to end of pre-ordering
#    return chain(post,[source])
    return post

def dfs_labeled_edges(G,source=None):
    """Produce edges in a depth-first-search starting at source and
    labeled by direction type (forward, reverse, nontree).
    """
    # Based on http://www.ics.uci.edu/~eppstein/PADS/DFS.py
    # by D. Eppstein, July 2004.
    if source is None:
        # produce edges for all components
        nodes=G
    else:
        # produce edges for components with source
        nodes=[source]
    visited=set()
    for start in nodes:
        if start in visited:
            continue
        yield start,start,{'dir':'forward'}
        visited.add(start)
        stack = [(start,iter(G[start]))]
        while stack:
            parent,children = stack[-1]
            try:
                child = next(children)
                if child in visited:
                    yield parent,child,{'dir':'nontree'}
                else:
                    yield parent,child,{'dir':'forward'}
                    visited.add(child)
                    stack.append((child,iter(G[child])))
            except StopIteration:
                stack.pop()
                if stack:
                    yield stack[-1][0],parent,{'dir':'reverse'}
        yield start,start,{'dir':'reverse'}
