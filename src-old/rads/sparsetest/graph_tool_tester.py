import numpy as np
from numpy import random as rnd
import graph_tool
import graph_tool.topology
from tester import Tester
import rads_gt

class Tester_graph_tool(Tester):

	def __init__(self):
		self.name = 'graph_tool'
		
	def set_graph(self,E,N):
		self.N = N
		self.M = graph_tool.Graph(directed=True)
		self.M.add_vertex(N)
		for (u,v) in E:
			self.M.add_edge(self.M.vertex(u),
							self.M.vertex(v))

	def rand_vert(self):
		return self.M.vertex(rnd.randint(0,self.N-1))

	def rand_row(self,R):
		i = self.rand_vert()
		for j in range(R):
			# will add multiple edges...
			self.M.add_edge(i,self.rand_vert())

	def rand_col(self,R):
		i = self.rand_vert()
		for j in range(R):
			# will add multiple edges...
			self.M.add_edge(self.rand_vert(),i)

	def rand_entry(self):
		self.M.add_edge(
			self.rand_vert(),
			self.rand_vert()
			)

	def subgraph(self,K):
		vf = self.M.new_vertex_property('bool')
		vf.get_array()[self.rand_verts(K)] = True
		self.M.set_vertex_filter(vf)
		self.M.set_vertex_filter(None)

	def scc(self):
		comp = graph_tool.topology.label_components(self.M)

	def graph_mis(self):
		rads_gt.graph_mis(self.M)
		
