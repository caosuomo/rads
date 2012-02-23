from libcpp.vector cimport vector
from libcpp.string cimport string

#------------------------------
# C++ declarations
#------------------------------

cdef extern from "point.h":
	cdef cppclass cPoint "Point":
		cPoint()
		cPoint(int)
		void set(int,double&)
		double& operator[] (int)
		int size()		
	cPoint *new_Point "new Point" (int)
	void del_Point "delete" (cPoint *)

	cdef cppclass cIPoint "IPoint":
		cIPoint()
		cIPoint(int)
		void set(int,double&)
		double& operator[] (int)
		int size()		
	cIPoint *new_IPoint "new IPoint" (int)
	void del_IPoint "delete" (cIPoint *)

cdef extern from "box.h":
	cdef cppclass cBox "Box":
		cPoint v
		cPoint w
		cBox()
		cBox(int)
		int size()
	cBox *new_Box "new Box" (int)
	void del_Box "delete" (cBox *)


cdef extern from "boxset.h":
	cdef cppclass cBoxSet "BoxSet":
		cBoxSet(int size)
		void set_box(int i, cBox &b)
		cBox get_box(int i)
		int size()
	cBoxSet *new_BoxSet "new BoxSet" (int)
	void del_BoxSet "delete" (cBoxSet *)

	cdef cppclass cUniformBoxSet "UniformBoxSet":
		cUniformBoxSet(int size)
		cUniformBoxSet(cPoint &w)
		cUniformBoxSet(int size, cPoint &w)
		cUniformBoxSet(cPoint &w, vector[cPoint] &c)
		
		void set_width(cPoint &w)
		cPoint get_width()
		void set_corner(int i, cPoint &v)
		cPoint get_corner(int i)
		cBox get_box(int i)
		int size()
	cUniformBoxSet *new_UniformBoxSet "new UniformBoxSet" (int)
	void del_UniformBoxSet "delete" (cUniformBoxSet *)

cdef extern from "tree.h":
	cdef cppclass cTree "Tree":
		cTree(cBox &)
		cTree(cTree &)

		void subdivide()
		void print_tree "print" ()

		int search(cPoint &)
		vector[int] search(cBox &)

		int insert(cPoint &)
		vector[int] insert(cBox &)

		int remove(int)
		int remove(vector[int])

		int dimension()
		int depth()
		int count()
		cUniformBoxSet boxes()
		cBox bounding_box()

	cTree *new_Tree "new Tree" (cBox&)
	cTree *new_Tree "new Tree" (cTree&)
	void del_Tree "delete" (cTree *tree)


cdef extern from "mapper.h":
	cdef cppclass cMapper "Mapper":
		cMapper(int,string)
		virtual cIPoint map_point (const cIPoint &v)
		cBoxSet map_points (const cUniformBoxSet &bs)
		cBoxSet map_points (const cBoxSet &bs)
		cIPoint get_params()
		vector[string] get_param_names()
		void set_params(cIPoint p)
		int dim()
		string name()
