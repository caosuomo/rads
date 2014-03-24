from rads.graphs.digraph import DiGraph
from rads.graphs import algorthms
from collections import deque
import numpy as np
from math import log
from itertools import product
from random import choice
from fractions import gcd 
import string

def log_max_eigenvalue( S ):
	"""
	Returns max( log( | v | ) ), where v is the leading
	eigenvalue of M.

	S -- Transition graph or numpy matrix
	"""
	# S is not an array, so it must be a DiGraph object (no type
	# checking past this).
	if isinstance(obj, DiGraph):
		M = S.to_numpy_matrix()
	# S is already a numpy matrix
	elif isinstance(obj, (numpy.matrix, numpy.ndarray)):
		M = S
	# compute the eigenvalues, take |v| of each, and store the largest
	# in eigmax
	eigmax = np.absolute( np.linalg.eigvals( M ) ).max()
	return np.log( eigmax )


class HashableMatrix( object ):
	"""
	A wrapper around a numpy matrix with a __hash__ function.
	"""
	def __init__(self, mat):
		self.mat = mat

	def __hash__(self):
		return hash(str(self.mat))

	def __repr__(self):
		s = str(self.mat.tolist())
		s = string.replace(s,",","")
		s = string.replace(s,"[[","[")
		s = string.replace(s,"] [","; ")
		s = string.replace(s,"]]","]")
		return "H" + s
		#		return "H" + string.replace(str(self.mat),"\n",";")

	def __eq__(self, other):
		if isinstance(other, HashableMatrix):
			return np.all(self.mat == other.mat)
		return NotImplemented

	def iszero(self):
		return not np.any(self.mat)
	
	def normalize(self):
		"""
		Returns a HashableMatrix given by dividing this matrix by the
		first nonzero entry.  If this is the zero matrix, this matrix
		is returned.
		"""

		# Already the zero matrix!
		if self.iszero():
			return HashableMatrix(self.mat.copy())

		# Divides mat by the greatest common divisor of the nonzero
		# entries --- note that this ensures that the integer division
		# doesn't zero out any nonzero entries!

		# list of nonzero entries
		nz = self.mat[np.nonzero(self.mat)].tolist()[0]
		# sign of 1st nz * gcd of nz's
		return HashableMatrix( self.mat * np.sign(nz[0])
							   / reduce(gcd, map(abs, nz)) )

		
class SoficProcessor( object ):
	"""
	Processes a given index map of generators into a sofic shift using
	VOODOO MAGIC.

	NOTE: WP right now assumes edge (i,j) means i points to j and will
	multiply matrices in edge order; this is opposite of the
	convention in the matlab code.
	
	Pseudocode for the algorithm, in different notation:

	MG = empty graph
	MG.add_node('zero')
	MG.add_node('unexplored')
	for e in E(G):
	  MG.add_node([e[1],M(e)])
	N = MG.nodes()

	for k=1:K or until N={}:
	  N' = {}
	  for n in N:
		t = n[0]
		for (t,u) in E(G)
		  M = matrix_product(pu)
		  if M == 0, MG.add_edge(n,'zero',label=u)
		  else:
			if [u,M] not in G, N' = N' union [u,M]
			G.add_edge(n,[u,M],label=u)
	  N = N'
	for n in N:
	  G.add_edge(n,'unexplored')
	"""
	def __init__( self,
				  index_map=None,
				  reg2gen=None,
				  symbol_graph=None,
				  debug=False,
				  max_iter = 30 ):
		"""
		Objects and methods for verifying words in a symbolic system
		based on the given index map. Works whether symbolic system
		contains one or many disjoint strongly connected components.

		index_map -- numpy matrix of map on generators on disjoint
		regions of an attactor in phase space. Output from CHomP or
		other homology software.

		reg2gen -- a dictionary of lists representing generators
		per region.

		symbol_graph -- An adjacency matrix representing the
		transitions between regions in phase space.

		debug -- turn on debugging messages (default = False)

		max_iter -- after how many iterations should we give
		up. (default = 30)
		"""

		self.max_iter = max_iter
		self.symbol_graph = symbol_graph
		if not self.symbol_graph:
			self.symbol_graph = \
			  algorithms.condensation(DiGraph(data=index_map), reg2gen)
			
		self.debug = debug

		# dictionary of matrices from the index map for each symbol
		# transition in symbol_graph
		self.edge_matrices = {}
		
		# graph with nodes (symbol,matrix)
		self.mgraph = DiGraph()
		for e in self.symbol_graph.edges_iter():
			# hashable, as we will use it in dictionaries
			mat = HashableMatrix( index_map[np.ix_(reg2gen[e[0]],
												   reg2gen[e[1]])] )
			self.edge_matrices[e] = mat
			self.mgraph.add_node( (e[1],mat.normalize()) )

		# nodes to explore next; init with all (symbol,matrix) nodes
		self.explore_nodes = self.mgraph.nodes()

		# add the special
		self.mgraph.add_node('zero')
		self.mgraph.add_node('unexplored')

		
	def matrix_product( self, node, symbol ):
		"""
		Comput the normalized matrix product of (s,M) and t
		"""
		# TODO: REVERSE MULT ORDER!
		return HashableMatrix( node[1].mat *
							  self.edge_matrices[(node[0],symbol)].mat
							  ).normalize()

								
		
	def process( self ):
		for k in range(self.max_iter):
			next_explore = set()
			for node in self.explore_nodes:
				if self.debug:
					print "Exploring node:", node
				s = node[0]				  # last vertex

				# for all t out of s
				for t in self.symbol_graph.successors(s):
					if self.debug:
						print "Processing transition:", (s,t)

					# hashable matrix which is the product up to t
					matrix = self.matrix_product(node,t)
					if self.debug:
						print "Matrix:", matrix

					if matrix.iszero():
						self.mgraph.add_edge(node,'zero',label=t)
						if self.debug:
							print "Forbidden!  (You... SHALL NOT... PASS!!)"
					else:
						new_node = (t,matrix)
						if new_node not in self.mgraph:
							if self.debug:
								print "New node!  Will explore further."
							next_explore.add(new_node)
						self.mgraph.add_edge(node,new_node,label=t)
						if self.debug:
							print "Adding edge:", node, "->", new_node

			self.explore_nodes = next_explore

			
		# flag any remaining nodes
		for node in self.explore_nodes:
			self.mgraph.add_edge(node,'unexplored')

	def __repr__( self ):
		s = "SoficProcessor on " + str( len( self.symbol_graph ) ) + " symbols"
		return s


		
if __name__ == "__main__":

	import numpy
	
	generators = numpy.matrix( [[0,0,0,1,0,0,0,0],
								[0,0,0,0,1,0,0,0],
								[0,0,0,0,1,0,0,0],
								[0,0,0,0,0,1,0,0],
								[-1,-1,0,0,0,0,0,0],
								[0,0,0,0,0,0,-1,1],
								[0,0,1,0,0,0,0,0],
								[-1,0,0,0,0,0,0,0]]
							   )
	
	regions = { 0 : [0],
				1 : [1,2],
				2 : [3],
				3 : [4],
				4 : [5],
				5 : [6,7]
				}

	adjmatrix = numpy.matrix( [[0,0,1,0,0,0],
							   [0,0,0,1,0,0],
							   [0,0,0,0,1,0],
							   [1,1,0,0,0,0],
							   [0,0,0,0,0,1],
							   [1,1,0,0,0,0]]
							  )
	
	generators = numpy.matrix( [[1, 0, 1],
								[0,-1,-1],
								[1, 1, 1]] )
	
	regions = { 0 : [0,1], 1 : [2] }

	adjmatrix = numpy.matrix( [[1,1],
							   [1,1]]
							  )


	sp = SoficProcessor( generators, regions, DiGraph(data=adjmatrix), debug=True, max_iter=2 )
	
