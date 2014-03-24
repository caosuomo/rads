from rads.symbolics.walk_library import UnverifiedLibrary, BadLibrary, Walk
from rads.graphs.digraph import DiGraph
from collections import deque
import numpy as np
from math import log
from itertools import product
from random import choice

class MatrixCollection( object ):
	"""
	Stores matrix products for words
	"""

	# look-up by (start,end), then by word
	# e.g. mc[(1,2)] would return {(1,3,2):matrix1, (1,2,3,2):matrix2, (1,2):matrix3, ...} 

	def __init__( self, letters ):
		# old way to write this: dict(((s,t),dict()) for s in letters for t in letters)
		self.mydict = {(s,t):{} for s in letters for t in letters}

	def matching_words( self, word ):
		"""
		Words in the collection matching the endpoints of word.
		"""
		return self.mydict[(word[0],word[-1])].keys()

	def __setitem__( self, word, matrix ):
		self.mydict[(word[0],word[-1])][word] = matrix

	def __contains__( self, word ):
		return word in self.mydict[(word[0],word[-1])]

	def __getitem__( self, word ):
		return self.mydict[(word[0],word[-1])][word]

def is_multiple( A, B ):
	"""
	Returns whether exists c != 0 s.t. A == c*B

	SPARSE ABILITIES NOT IMPLEMENTED YET!
	"""
	if A.shape != B.shape:
		return False
	if sparse.issparse( A ):
		w0, w1, V = sparse.find( A )
	else:
		w0, w1 = np.where( A != 0 )
	try:
		c =	 float( A[w0[0,0],w1[0,0]] ) / float( B[w0[0,0],w1[0,0]] )
	except ZeroDivisionError:
		return False
	if np.array_equal( A, c*B):
		return True
	else:
		return False	

class WordProcessor( object ):
	"""
	Processes a given index map of generators using the forbidden
	words approach.

	NOTE: WP right now assumes edge (i,j) means i points to j and will
	multiply matrices in edge order; this is opposite of the
	convention in the matlab code.
	
	Pseudocode for the algorithm, in different notation:

	P = {(v) for v in V(G)}
	L(p) = 1 for p in P
	F = {}
	R = {}
	for k=1:K or until P={}:
	  P' = {}
	  for p in P:
	    s = p(begin), t = p(end)
	    for (t,u) in E(G):n
	      if M(pu) = 0, F = F union {pu}
	      elif reducible(pu), R = R union {pu}
	      else:
	        P' = P' union {pu}
	        L(pu) = M(pu)
	  P = P'
	F = F union P
	F = F setminus {p in F : exists p' in F s.t. p subset p'}

	reducible(p):
	  exists p' in R s.t. p' suffix p
	  OR
	  exists p' in L s.t. p':s->t, |p'|<|p|, M(p) propto L(p')
	"""
	def __init__( self,
				  index_map=None,
				  reg2gen=None,
				  symbol_graph=None,
				  debug=False,
				  verbose=False,
				  check_trace=False,
				  max_length = 30 ):
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

		check_trace -- toggle whether to check the trace of matrix
		products resulting from cycles. (default=False)

		max_length -- after what length should all remaining words be
		prohibited. (default = 30)
		"""
		# IndexMap.__init__( self, index_map, reg2gen,
		#					 symbolic_system, debug, verbose )

		self.check_trace = check_trace
		self.max_length = max_length
		self.symbol_graph = symbol_graph
		self.debug = debug
		
		# words to explore next; init with all length-1 words
		self.explore_words = self.symbol_graph.edges()

		# forbidden words, which lead to 0 matrix products
		self.forbidden_words = set()

		# words which have the same matrix product as shorter words
		self.reduced_words = set()

		self.matrices = MatrixCollection(reg2gen.keys())
		for edge in self.symbol_graph.edges_iter():
			self.matrices[edge] = index_map[np.ix_(reg2gen[edge[0]],reg2gen[edge[1]])]

	def reducible( self, word, matrix ):
		# reducible if any suffix is
		for i in range(len(word)):
			if word[i+1:] in self.reduced_words:
				if self.debug:
					print "Reducible!  suffix:", word[i+1:]
				return True

		# reducible if a shorter word has the same matrix product up
		# to a scalar multiple
		for w in self.matrices.matching_words(word):
			if len(w) < len(word) and \
			  is_multiple(self.matrices[w],matrix):
				if self.debug:
					print "Reducible!  reduces to:", w
				return True
		return False

	def process( self ):
		for k in range(self.max_length):
			next_explore = set()
			for w in self.explore_words:
				if self.debug:
					print "Exploring word:", w
				# for all u which can extend w
				for (_,u) in self.symbol_graph.edges_iter(w[-1]):
					word = w+(u,) # append u
					if self.debug:
						print "Processing word:", word
					# matrix product of word TODO: CHECK MULT ORDER!
					matrix = self.matrices[w]*self.matrices[(w[-1],u)]
					if self.debug:
						print "Matrix:"
						print matrix
					if not np.any(matrix):
						if self.debug:
							print "Forbidden!  (You... SHALL NOT... PASS!!)"
						self.forbidden_words.add(word)
					elif self.reducible(word,matrix):
						self.reduced_words.add(word)
					else:
						if self.debug:
							print "Need to explore further..."
						next_explore.add(word)
						self.matrices[word] = matrix
			self.explore_words = next_explore
			
		# add any remaining words to the forbidden list
		self.forbidden_words.update(self.explore_words)

	def __repr__( self ):
		s = "WordProcessor on " + str( len( self.symbol_graph ) ) + " symbols\n"
		s += "  forbidden: " + self.forbidden_words.__repr__() + "\n"
		s += "  reduced:   " + self.reduced_words.__repr__()
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
	
	generators = numpy.matrix( [[1,0,-1],
								[0,-1,1],
								[1,1,-1]]
							   )
	
	regions = { 0 : [0,1],
				1 : [2]
				}

	adjmatrix = numpy.matrix( [[1,1],
							   [1,1]]
							  )


	wp = WordProcessor( generators, regions, DiGraph(data=adjmatrix), debug=True, max_length=10 )
	
