#!/usr/bin/python

import sys
import numpy as np
import numpy.random
import matplotlib.pylab as plt
from rads import UBoxSet,Tree,gfx
import timeit

def time_stuff(test_str,import_str,num_trials):
	return timeit.Timer(test_str, import_str).timeit(number=num_trials)/num_trials


dims = range(1,9)
xbounds = {
	1:(1,10),
 	2:(1,8),
 	3:(1,4),
 	4:(1,3),
 	5:(1,3),
 	6:(1,3),
 	7:(1,2),
	8:(1,2),
		}
bounds = {
 	1:(1,20),
  	2:(1,12),
  	3:(1,8),
  	4:(1,6),
  	5:(1,5),
  	6:(1,4),
  	7:(1,3),
 	8:(1,3),
	}

results = {}
funcs = ['insert','insert_box','search','search_box']
trials_list = [1,1,1000,50]
trials = {}

def get_data(test):
	np.random.seed(10)					# for consistency
	d = {'insert':np.random.rand(10000,test['dim']),
		 'insert_box':np.random.rand(1000,2,test['dim']),
		 'search':np.random.rand(50,test['dim']),
		 'search_box':np.random.rand(50,2,test['dim']),
		 }
	d['insert_box'][:,1] /= 2
	d['search_box'][:,1] /= 2
	return d
			
def get_tree(test):
	bbox = np.tile([[0.0],[1]],test['dim'])		# unit cube
	tree = Tree(bbox,depth=test['depth'],full=False)
	return tree

def bench(test,data):
	print 'dimension %(dim)i at depth %(depth)i' % test;
	test['results'] = {}
	for f in funcs:
		r = []
		for i in range(len(data[f])):
			t = time_stuff('tree.%s(data[\'%s\'][%i])' % (f,f,i),
						   'from __main__ import tree, data',
						   trials[f]) / trials[f]
			r.append(t)
		print '%12s: %6.8f' % (f,1000*np.mean(r))
		test['results'][f] = r
		results[f][test['dim']][test['depth']] = np.mean(r)

def save_data(test,data):
	for d in data:
		if '_box' in d:
			s = data[d].shape
			data[d] = data[d].reshape(s[0]*s[1],s[2])
		# print 'bench_%s_dim_%i_depth_%i.txt' % (d,test['dim'],test['depth'])
		np.savetxt('bench_%s_dim_%i_depth_%i.txt' % (d,test['dim'],test['depth']),
				   data[d])

import pickle
import time
import scipy.io

data = None
tree = None
print __name__
if __name__ == "__main__":
	tests = []
	for dim in bounds:
		tests += [{'dim':dim, 'depth':d}
				  for d in
				  range(bounds[dim][0],bounds[dim][1]+1)]
	for t in range(len(trials_list)):
		trials[funcs[t]] = trials_list[t]
	for f in funcs:
		results[f] = {}
		for dim in bounds:
			results[f][dim] = np.zeros(bounds[dim][1]+1)
	print "running tests..."
	for test in tests:
		if tree:
			del data
			del tree
		data = get_data(test)
		tree = get_tree(test)
		bench(test,data)
		save_data(test,data)
	datestr = time.strftime("%Y-%m-%d-%H-%M-%S")
	pickle.dump(results,file('bench-results-%s.pickle' % (datestr),'w'))
	pickle.dump(results,file('bench-results.pickle','w'))
else:
	results = pickle.load(file('bench-results.pickle','r'))
	mat = scipy.io.loadmat('bench-tree.mat')['R'][0]
	# matlab indices are: [dim,depth,func]


	fgcolor = np.array([20,50,100])/255.
	bgcolor = 0.9 + 0.1*fgcolor
	plt.rcdefaults()
	plt.rcParams['figure.subplot.right']='0.9'
	plt.rcParams['figure.subplot.left']='0.2'
	plt.rcParams['figure.subplot.top']='0.8'
	plt.rcParams['ytick.major.pad']='10'
	plt.rcParams['ytick.color']=fgcolor
	plt.rcParams['xtick.color']=fgcolor
	plt.rcParams['ytick.major.size']=4
	plt.rcParams['xtick.major.size']=4
	plt.rc('lines', linewidth=2)
	plt.rc('text', color=fgcolor)
	plt.rc('axes', linewidth=2, facecolor=bgcolor, edgecolor=fgcolor, labelcolor=fgcolor)
	plt.show()

	plotfuncs = [1,3]
	labels = ['Insert','Search']
	colors = [np.array([120,20,10])/255.,np.array([98,158,31])/255.]

	plt.close('all')
	fig = plt.figure(frameon=False)
#	plt.title('depth 3')
	depth = 3
	for pf in range(len(plotfuncs)):
		f = plotfuncs[pf]
		x = dims
		y = [-np.log(results[funcs[f]][dims[i]][depth-1] / mat[i][depth-1,f]) for i in range(len(dims))]
		plt.plot(x,y,color=colors[pf])
		plt.xlabel('dimension',size='large')
		plt.ylabel('log(speedup) = log(matlab/python)',size='large')
	plt.ylim(-0.5,6.5)
	lg = plt.legend(labels,loc='upper left')
	lg.get_frame().set_fill(False)
	plt.savefig('/home/raf/Dropbox/cs262a/report/trees.pdf',bbox_inches='tight')

	screwup


# 		for f in range(len(funcs)):
# 			fig = plt.figure()
# 			plt.title(funcs[f])
# 			for d in range(len(dims)):
# 				x = np.arange(bounds[dims[d]][0],bounds[dims[d]][1]+1)
# 				y = results[funcs[f]][dims[d]][bounds[dims[d]][0]:] / mat[d][:,f]
# 				plt.plot(x,y)
# 			plt.legend(dims)
# 			plt.xlabel('depth')
# 			plt.ylabel('py / mat')

# 	fig = plt.figure()
# #	plt.title('dimension 3')
# 	d = 2    # dimension 3
# 	for f in range(len(funcs)):
# 		x = np.arange(bounds[dims[d]][0],bounds[dims[d]][1]+1)
# 		y = results[funcs[f]][dims[d]][bounds[dims[d]][0]:] / mat[d][:,f]
# 		plt.plot(x,y)
# 		plt.xlabel('depth')
# 		plt.ylabel('Python / Matlab')
# 	plt.legend(funcs)
