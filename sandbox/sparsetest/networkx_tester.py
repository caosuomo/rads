import numpy as np
from numpy import random as rnd
import networkx
from tester import Tester
import rads_nx

class Tester_networkx(Tester):

	def __init__(self):
		self.name = 'networkx'

	def set_graph(self,E,N):
		self.N = N
		self.M = networkx.DiGraph()
		self.M.add_nodes_from(range(N))
		self.M.add_edges_from(E)
	
	def rand_row(self,R):
		i = self.rand_vert()
		self.M.add_edges_from([(i,self.rand_vert()) for j in range(R)])

	def rand_col(self,R):
		i = self.rand_vert()
		self.M.add_edges_from([(self.rand_vert(),i) for j in range(R)])

	def rand_entry(self):
		i = self.rand_vert()
		j = self.rand_vert()
		if not self.M.has_edge(i,j):
			self.M.add_edge(i,j)

	def subgraph(self,K):
		networkx.subgraph(self.M,self.rand_verts(K))

	def scc(self):
		networkx.algorithms.components.strongly_connected_components(self.M)

	def graph_mis(self):
		rads_nx.graph_mis(self.M)
