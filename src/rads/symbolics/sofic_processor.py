from rads.graphs.digraph import DiGraph
from rads.graphs import algorithms
from collections import deque
import numpy as np
import sympy as sp
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
	if isinstance(S, DiGraph):
		M = S.to_numpy_matrix()
	# S is already a numpy matrix
	elif isinstance(S, (np.matrix, np.ndarray)):
		M = S
	# compute the eigenvalues
	v = np.linalg.eigvals( M )
	# log of the maximum modulus
	return np.log( np.absolute( v ).max() )


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
		"""
		Returns True iff this is the zero matrix (or the empty matrix,
		which is the normalization of the zero matrix)
		"""
		return not np.any(self.mat)
	
	def normalize_gcd(self):
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

	def normalize(self):
		"""
		Returns a HashableMatrix describing the image of this matrix.
		"""

		A = sp.Matrix(self.mat).transpose() # cols -> rows
		#print A
		A = A.rref()[0]			# row reduce
		#print A
		# see https://groups.google.com/forum/#!topic/sympy/e8hcF4QAldc
		A = np.array(sp.lambdify((), A, 'numpy')()) # convert to numpy array
		#print A
		A = A[np.any(A,1),:]	# remove 0 rows
		A = A.transpose()		# back into original dimensions
		#print A
		
		return HashableMatrix( np.matrix(A) )
	
		
class SoficProcessor( object ):
	"""
	Processes a given index map of generators into a sofic shift using
	VOODOO MAGIC.

	NOTE ON EDGE DIRECTION: to match the interpretation of the index
	map as a linear transformation, we consider index_map[i,j] to be
	the coefficient of generator i in the image of generator j.  This
	means our matrix multiplication along a path of symbols (i,j,k)
	will index_map[g(k),g(j)] * index_map[g(j),g(i)], where g(i) is
	the indices of generators corresponding to symbol i.  We will
	however continue to write (i,j) and (j,k) in the *graph*, as is
	standard.
	
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
		#symbol_transitions=None,
				  debug=False ):
		"""
		Objects and methods for verifying words in a symbolic system
		based on the given index map. Works whether symbolic system
		contains one or many disjoint strongly connected components.

		index_map -- numpy matrix of map on generators on disjoint
		regions of an attactor in phase space. Output from CHomP or
		other homology software.

		reg2gen -- a dictionary of lists representing generators
		per region.

		debug -- turn on debugging messages (default = False)
		"""

		self.debug = debug
		
		# construct the starting graph on symbols
		# by the above, we must reverse the edge direction!
		generator_graph = DiGraph(data=index_map.transpose())
		self.symbol_graph = algorithms.condensation(generator_graph,
													reg2gen,
													loops=True)

		# dictionary of matrices from the index map for each symbol
		# transition in symbol_graph
		self.edge_matrices = {}
		
		# graph with nodes (symbol,matrix)
		self.mgraph = DiGraph()
		for e in self.symbol_graph.edges_iter():
			# hashable, as we will use it in dictionaries.
			#
			# note that an edge (i,j) means we are mapping generators
			# in region i to those in region j, so this is a n_j by
			# n_i matrix
			mat = HashableMatrix( index_map[np.ix_(reg2gen[e[1]],
												   reg2gen[e[0]])] )
			self.edge_matrices[e] = mat
			# we are mapping to region j, with matrix mat
			self.mgraph.add_node( (e[1],mat.normalize()) )

		# nodes to explore next; init with all (symbol,matrix) nodes
		self.explore_nodes = self.mgraph.nodes()

		# add the special nodes
		self.mgraph.add_node('zero')
		# self.mgraph.add_node('unexplored')

		
	def matrix_product( self, node, symbol ):
		"""
		Compute the normalized matrix product of (s,M) and t
		"""
		# NOTE the order of multiplication: M is a matrix mapping into
		# region s, and E=edge_matrices[(s,t)] is mapping from s to t,
		# so we want S*M
		return HashableMatrix( self.edge_matrices[(node[0],symbol)].mat
							   * node[1].mat
							  ).normalize()

								
		
	def process( self, steps=20, debug=None ):
		"""
		Process the index information for some number of steps.
		Stores all the nodes to explore in self.explore_nodes, so that
		this method may be run again to pick up where it left off.

		steps -- how many steps to run
		debug -- whether to print diagnostic info
		"""
		if debug == None:
			debug = self.debug
		
		for k in range(steps):
			next_explore = set()
			for node in self.explore_nodes:
				if self.debug:
					print "Exploring node:", node
				s = node[0]				  # last vertex

				# for all t out of s
				for t in self.symbol_graph.successors(s):
					if self.debug:
						print "Transition:", (s,t), self.edge_matrices[(s,t)]

					# hashable matrix which is the product up to t
					matrix = self.matrix_product(node,t)

					if matrix.iszero():
						self.mgraph.add_edge(node,'zero',label=t)
						if self.debug:
							print "FORBIDDEN!  ", node, "-X->", (t,matrix)
					else:
						new_node = (t,matrix)
						if new_node not in self.mgraph:
							if self.debug:
								print "Adding new node:", new_node
							next_explore.add(new_node)
						self.mgraph.add_edge(node,new_node,label=t)
						if self.debug:
							print "Adding edge:", node, "->", new_node

			self.explore_nodes = next_explore

			
		# flag any remaining nodes
		#		for node in self.explore_nodes:
		#	self.mgraph.add_edge(node,'unexplored')

	def entropy( self ):
		"""
		Return the topological entropy of the sofic shift up to the
		current level of processing.
		"""
		return log_max_eigenvalue(self.mgraph.to_numpy_matrix())

	def has_terminated( self ):
		"""
		Return whether or not the processor has more work to do.
		"""
		return len(self.explore_nodes)==0
	
	def __repr__( self ):
		s = ("SoficProcessor on %i symbols, with %i states and %i transitions"
			 % (str(len(self.symbol_graph)),
				self.mgraph.number_of_nodes(),
				self.mgraph.number_of_edges() ) )
		return s


		
if __name__ == "__main__":

	import numpy
	
	generators = numpy.matrix( [[1, 0, 1],
								[0,-1, 1],
								[1,-1, 1]] )
	
	regions = { 0 : [0,1], 1 : [2] }

	adjmatrix = numpy.matrix( [[1,1],
							   [1,1]]
							  )


	sof = SoficProcessor( generators, regions, debug=True )
	sof.process()
	print "Entropy of the even shift:", sof.entropy()
