cdef class Point:
	def __cinit__(self, int dim):
		self.point = new_Point(dim)

	def __dealloc__(self):
		del_Point(self.point)

	def size(self):
		return self.point.size()

cdef Point a2point(np.ndarray[np.double_t,ndim=1] a):
	cdef Point p = Point(a.shape[1])
	for i in range(a.shape[1]):
		p.point.set(i,a[i])
	return p

