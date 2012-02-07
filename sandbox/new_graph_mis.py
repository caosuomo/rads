import numpy as np
from numpy import random as rnd
import graph_tool.all as gt
from graph_tool.all import *
#import graph_tool.topology as top
#import graph_tool.community as comm
import itertools

class RADSVisitor(gt.DFSVisitor):
	def __init__(self, visited):
		self.visited = visited
		self.started = 0
	
	def tree_edge(self, e):
		if self.started > 1:
			return
#		print "tree edge (%s, %s) has been examined..." % \
#			  (e.source(),e.target())
		self.visited[e.target()] = 1

	def start_vertex(self, u):
		"""
		This is invoked on the source vertex once before the start of the
		search.
		"""
		#could set to visited, but start will be the phantom node anyway...
#		print 'starting vertex', u
		self.started += 1
		return


class SCCVisitor(gt.DFSVisitor):
	def __init__(self, G, comps):
		self.S = []
		self.comp_num = 0
		self.index = 0
		self.pred = G.new_vertex_property("int")
		self.instack = G.new_vertex_property("int")
		self.indices = G.new_vertex_property("int")
		self.lowlinks = G.new_vertex_property("int")
		self.comps = comps

	def discover_vertex(self, v):
		self.indices[v] = self.index
		self.lowlinks[v] = self.index #+1	# gotta earn it's SCCness
		self.index += 1
		self.S.append(v)
		self.instack[v] = 1

	def finish_vertex(self, v):
		u = self.pred[v]-1				# parent
		self.lowlinks.a[u] = min(self.lowlinks.a[u],self.lowlinks[v])
		if self.lowlinks[v] == self.indices[v]:
			print 'scc',int(v)
			while True:
				s = self.S.pop()
				self.instack[s] = 0
				self.comps[s] = self.comp_num
				if s == v:
					break
			self.comp_num += 1
			
	def tree_edge(self, e):
		self.pred[e.target()] = int(e.source())+1

	def back_edge(self, e):
		if self.instack[e.target()]:
			self.lowlinks[e.source()] = min(self.lowlinks[e.source()],
											self.indices[e.target()])

	def forward_or_cross_edge(self, e):
		if self.instack[e.target()]:
			self.lowlinks[e.source()] = min(self.lowlinks[e.source()],
											self.indices[e.target()])


class MISVisitor(gt.DFSVisitor):
	def __init__(self, G, mis):
		self.S = []
		self.index = 0
		self.pred = G.new_vertex_property("int")
		self.instack = G.new_vertex_property("int")
		self.indices = G.new_vertex_property("int")
		self.lowlinks = G.new_vertex_property("int")
		self.mis_ancestor = G.new_vertex_property("int")
		self.mis = mis
		for v in G.vertices():
			self.lowlinks[v] = G.num_vertices() # infinity

	def discover_vertex(self, v):
		self.indices[v] = self.index
#		self.lowlinks[v] = self.index
		self.index += 1
		self.S.append(v)
		self.instack[v] = 1

	def finish_vertex(self, v):
		u = self.pred[v]-1				# parent
		self.lowlinks.a[u] = min(self.lowlinks.a[u],self.lowlinks[v])
		print 'vertex',int(v),'lowlink',self.lowlinks[v],self.indices[v]
		if self.lowlinks[v] == self.indices[v]:
			print 'scc', int(v)
			while True:
				s = self.S.pop()
				self.instack[s] = 0
				self.mis[s] = 1
				if s == v:
					break
		elif self.lowlinks[v] > self.indices[v]:
			if self.mis_ancestor[v] == 0: # leads to nothingness :]
				assert(self.S[-1] == v)
				self.S.pop()			  # last one is v
		self.mis_ancestor.a[u] |= (self.mis_ancestor[v] | self.mis[v])

	def tree_edge(self, e):
		print 'tree',(int(e.source()),int(e.target()))
		self.pred[e.target()] = int(e.source())+1

	def back_edge(self, e):
		print 'back',(int(e.source()),int(e.target()))
		print 'lowlink',self.lowlinks[e.source()],self.indices[e.target()]
		if self.instack[e.target()]:
			self.lowlinks[e.source()] = min(self.lowlinks[e.source()],
											self.indices[e.target()])

	def forward_or_cross_edge(self, e):
		print 'forward',(int(e.source()),int(e.target()))
		print 'lowlink',self.lowlinks[e.source()],self.indices[e.target()]
		if self.instack[e.target()]:
			self.lowlinks[e.source()] = min(self.lowlinks[e.source()],
											self.indices[e.target()])


def graph_mis_dfs(G):
	mis = G.new_vertex_property("int")
	vis = MISVisitor(G,mis)
	gt.dfs_search(G, G.vertex(0), vis)
	print 'vis.mis_ancestor.a', vis.mis_ancestor.a
	return np.nonzero(np.array(mis.a))[0]

def raf_components(G):
	comps = G.new_vertex_property("int")	
	gt.dfs_search(G, G.vertex(0), SCCVisitor(G,comps))
	return comps

def descendents(G,S):
	new_vert = G.add_vertex()
	for v in S:
		G.add_edge(new_vert,v)
	visited = G.new_vertex_property("int")
	gt.dfs_search(G, new_vert, RADSVisitor(visited))
	G.remove_vertex(new_vert)
	return set(np.nonzero(np.array(visited.a))[0])


import time

def graph_mis(G):
	t = time.time()
	comp_prop = gt.label_components(G)
#	print comp_prop.a
	(C,vertex_count,edge_count) = gt.condensation_graph(G,comp_prop)
	print 1, time.time() - t

# 	t = time.time()
# 	comp_prop = raf_components(G)
# #	print comp_prop.a
# 	(C,vertex_count,edge_count) = gt.condensation_graph(G,comp_prop)
# 	print 2, time.time() - t

	t = time.time()
	nodes = []
	for i in range(C.num_vertices()):
		nodes.append([])

	for v in G.vertices():
		nodes[comp_prop[v]].append(int(v))
	print nodes
	print 3, time.time() - t

	t = time.time()
	comps = []
	for v in C.vertices():
		if vertex_count.a[int(v)] > 1:
			comps.append(v)
		else:
			w = nodes[int(v)][0]		# single vertex
			if G.edge(w,w):
				comps.append(v)
	print 4, time.time() - t

	t = time.time()
	upstream = descendents(C,comps)
#	print [nodes[c] for c in upstream]
	C.set_reversed(True)
	downstream = descendents(C,comps)
#	print [nodes[c] for c in downstream]
	
	cnodes = upstream & downstream
#	print [nodes[c] for c in cnodes]
	mis = list(itertools.chain(*[nodes[c] for c in cnodes]))
	print 5, time.time() - t
	
 	return mis 
	
if __name__ == "__main__":
	E = [(0, 8), (0, 5), (1, 19), (1, 10), (1, 3), (1, 5), (2, 11), (2, 4), (2, 7), (4, 19), (6, 19), (6, 3), (6, 5), (7, 19), (7, 3), (8, 0), (9, 16), (9, 14), (9, 6), (10, 3), (10, 12), (11, 19), (11, 6), (12, 3), (12, 12), (13, 8), (13, 19), (13, 11), (13, 5), (14, 1), (14, 9), (16, 3), (17, 1), (18, 16), (18, 13), (19, 9)]
	G = gt.Graph(directed=True)
	G.add_vertex(20)
	for (u,v) in E:
		G.add_edge(G.vertex(u),G.vertex(v))
	correct_answer = [0, 1, 6, 8, 9, 10, 12, 14, 19]
	raf_components(G)
	my_answer = sorted(graph_mis(G))
	dfs_answer = sorted(graph_mis_dfs(G))
	print 'graph_mis(G) =    ', my_answer
	print 'graph_mis_dfs(G) =', dfs_answer
	print 'correct answer =  ', correct_answer



E = [(0,1), (1,2), (2,0), (2,3), (3,4), (1,5), (5,6), (6,6), (7,0), (8,7)]
G = gt.Graph(directed=True)
G.add_vertex(9)
for (u,v) in E:
	G.add_edge(G.vertex(u),G.vertex(v))

# import networkx as nx
# nxg = nx.DiGraph()
# nxg.add_nodes_from(range(9))
# for (u,v) in E:
# 	nxg.add_edge(u,v)
# nx.draw(nxg)

my_answer = sorted(graph_mis(G))
dfs_answer = sorted(graph_mis_dfs(G))
print 'graph_mis(G) =    ', my_answer
print 'graph_mis_dfs(G) =', dfs_answer
print 'correct answer =  ', [0,1,2,5,6]
