from cppdefs cimport *
from cyutils cimport *

cdef class Mapper:

	def __cinit__(self):
		print "in Mapper cinit"

	def set_params(self,object dict):
		cdef vector[std_string] names = self.mapper.get_param_names()
		cdef cIPoint p = self.mapper.get_params()	# so we can only overwrite a few and leave the rest
		for k in dict.keys():
			for i in range(names.size()):
				if k == (<object>names[i].c_str()): # name matches param name
					p.set(i,array2interval(dict[k]))
					break
		self.mapper.set_params(p)

	def get_params(self):
		cdef cIPoint p = self.mapper.get_params()	# so we can only overwrite a few and leave the rest
		cdef vector[std_string] names = self.mapper.get_param_names()
		d = dict()
		for i in range(names.size()):
			d[<object>names[i].c_str()] = interval2array(p[i])
		return d

	cdef cMapper *get_mapper(self):
		return self.mapper

	cpdef int get_dim(self):
		return self.mapper.dim()
	
#	cpdef char * get_name(self):
#		return self.mapper.name().c_str()
