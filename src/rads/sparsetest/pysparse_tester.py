import numpy as np
from numpy import random as rnd
import pysparse
from tester import Tester

class Tester_pysparse(Tester):

	def __init__(self):
		self.name = 'pysparse'

	def set_graph(self,E,N):
		self.N = N
		self.M = pysparse.spmatrix.ll_mat(N,N)
		for e in E:
			self.M[e]=1

	def rand_row(self,R):
		self.M[self.rand_vert(),self.rand_verts(R)] = 1

	def rand_col(self,R):
		self.M[self.rand_verts(R),self.rand_vert()] = 1

	def rand_entry(self):
		self.M[self.rand_vert(),self.rand_vert()] = 1

	def subgraph(self,K):
		S = self.rand_verts(K)
		self.M[S,S]

	def scc(self):
		pass
