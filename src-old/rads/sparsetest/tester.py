import numpy as np
from numpy import random as rnd

class Tester:
	def rand_vert(self):
		return rnd.randint(0,self.N-1)

	def rand_verts(self,R):
		return rnd.randint(0,self.N-1,R)

