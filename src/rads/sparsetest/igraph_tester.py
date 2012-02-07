import numpy as np
from numpy import random as rnd
import igraph
from tester import Tester

class Tester_igraph(Tester):

	def __init__(self):
		self.name = 'igraph'

	def set_graph(self,E,N):
		self.N = N
		self.M = igraph.Graph(N)
		self.M.add_edges(E)

	def rand_row(self,R):
		i = self.rand_vert()
		self.M.add_edges([(i,self.rand_vert()) for j in range(R)])

	def rand_col(self,R):
		i = self.rand_vert()
		self.M.add_edges([(self.rand_vert(),i) for j in range(R)])

	def rand_entry(self):
		self.M.add_edges([(self.rand_vert(),self.rand_vert())])

	def scc(self):
		pass
#		self.M.biconnected_components()
