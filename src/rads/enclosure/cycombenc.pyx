cimport numpy as np
import numpy as np
from cppdefs cimport *
from cyutils cimport *
from cyboxset cimport UBoxSet
from cymapper cimport Mapper
from cytree cimport Tree

#from sage.all cimport *
#from sage.graphs.base.sparse_graph cimport SparseGraph
###from sage.all import Graph,DiGraph
from rads.graphs import Graph,DiGraph

cdef class CombEnc:
	def __init__(self,tree=None,mapper=None):
		if mapper is not None:
			self.mapper = mapper
		if tree is not None:
			self.tree = tree
		self.adj = None
		self.mvm = None

	def __cinit__(self):
		pass
	def __dealloc__(self):
		pass

	cpdef update(self):
		# just start all over for now; optimize later
		cdef cUniformBoxSet ubs = self.ctree.boxes()
		self.compute_adj(ubs)
		self.compute_mvm(ubs)

	cdef void compute_adj(self,cUniformBoxSet &ubs):
		cdef Py_ssize_t i
		cdef int N = ubs.size()
		cdef np.ndarray a
		#self.adj = Graph(N,implementation="c_graph")
		self.adj = Graph()
		self.adj.add_nodes_from(range(N))
		for i in range(N):
			# find the neighbors of box i
			# search the tree for ith box
			#   (search is inclusive, so we get the right boxes)
			# convert the result to an array, and index into adj
			#self.adj[i,vector2array_int(self.ctree.search(ubs.get_box(i)))] = 1
			a = vector2array_int(self.ctree.search(ubs.get_box(i)))
			# add (i,v) for v in a:
			self.adj.add_edges_from(zip(np.tile(i,len(a)),a))

	cdef void compute_mvm(self,cUniformBoxSet &ubs):
		cdef Py_ssize_t i
		cdef int N = ubs.size()
		cdef np.ndarray a
		#self.mvm = DiGraph(N,implementation="c_graph")
		self.mvm = DiGraph()
		self.mvm.add_nodes_from(range(N))

		# map all boxes under mapper's mapper
		cdef cBoxSet ims = self.cmapper.map_points(ubs)
		# Cython won't let us stack allocate this!!! argh... will have
		# to change cMapper.map_points to create a new BoxSet and
		# return it, and we'll have to keep track of it and delete
		# it... GRRR

		# update the matrix
		for i in range(ubs.size()):
			#print 'image of', box2array(ubs.get_box(i)),
			#'is', box2array(ims.get_box(i))
			a = vector2array_int(self.ctree.search(ims.get_box(i)))
			#print 'image box ids:', a
			#print 'new edges:', zip(np.tile(i,len(a)),a)
			# add (i,v) for v in a:
			self.mvm.add_edges_from(zip(np.tile(i,len(a)),a))
			#self.mvm[i,vector2array_int(self.ctree.search(ims.get_box(i)))] = 1
			#self.mvm = self.mvm.transpose()	# since A[j,i] iff i->j
		
	property adj:
		def __get__(self):
			return self.adj
		
	property mvm:
		def __get__(self):
			return self.mvm
		
	property mapper:
		def __set__(self,Mapper mapper):
			# TODO: type check that it's a Mapper
			self.pymapper = mapper
			self.cmapper = mapper.mapper
		def __get__(self):
			return self.pymapper

	property tree:
		def __set__(self,Tree tree):
			# TODO: type check that it's a Tree
			self.pytree = tree
			self.ctree = tree.tree
		def __get__(self):
			return self.pytree

	def __repr__(self):
		return "CombEnc object"

