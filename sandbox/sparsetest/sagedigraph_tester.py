import numpy as np
from numpy import random as rnd
from sage.all import DiGraph
from tester import Tester
#import rads_nx

class Tester_sagedigraph(Tester):

	def __init__(self):
		self.name = 'networkx'

	def set_graph(self,E,N):
		self.N = N
		self.M = DiGraph(N)
		self.M.add_edges(E)
	
	def rand_row(self,R):
		i = self.rand_vert()
		self.M.add_edges([(i,self.rand_vert()) for j in range(R)])

	def rand_col(self,R):
		i = self.rand_vert()
		self.M.add_edges([(self.rand_vert(),i) for j in range(R)])

	def rand_entry(self):
		i = self.rand_vert()
		j = self.rand_vert()
		if not self.M.has_edge(i,j):
			self.M.add_edge(i,j)

	def subgraph(self,K):
		self.M.subgraph(self.rand_verts(K))

	def scc(self):
		self.M.strongly_connected_components()

	def graph_mis(self):
		pass
#		rads_nx.graph_mis(self.M)
