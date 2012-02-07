import numpy as np
from numpy import random as rnd
import pygraph.classes.digraph
import pygraph.algorithms.accessibility
from tester import Tester

class Tester_pygraph(Tester):

	def __init__(self):
		self.name = 'pygraph'

	def set_graph(self,E,N):
		self.N = N
		self.M = pygraph.classes.digraph.digraph()
		self.M.add_nodes(range(N))
		for e in E:
			self.M.add_edge(e)

	def rand_row(self,R):
		i = self.rand_vert()
		for j in range(R):
			e = (i,self.rand_vert())
			if not self.M.has_edge(e):
				self.M.add_edge(e)

	def rand_col(self,R):
		i = self.rand_vert()
		for j in range(R):
			e = (self.rand_vert(),i)
			if not self.M.has_edge(e):
				self.M.add_edge(e)

	def rand_entry(self):
		e = (self.rand_vert(),self.rand_vert())
		if not self.M.has_edge(e):
			self.M.add_edge(e)

	def subgraph(self,K):
		pass

	def scc(self):
		pygraph.algorithms.accessibility.mutual_accessibility(self.M)
