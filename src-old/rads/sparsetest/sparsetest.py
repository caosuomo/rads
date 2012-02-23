#from pysparse_tester    import *
#from scipy_tester       import *
from networkx_tester    import *
from graph_tool_tester  import *
#from pygraph_tester     import *
import timeit

testers = [Tester_networkx(), Tester_graph_tool()]

debug = True


tests = {

	1000:[
		{'matlab':0.000000, 'F':'rand_row', 'T':100, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'rand_col', 'T':100, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'rand_entry', 'T':100, 'args':'', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'subgraph', 'T':100, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'scc', 'T':10, 'args':'', 'who':range(len(testers))},
		{'matlab':0.004000, 'F':'graph_mis', 'T':10, 'args':'', 'who':range(len(testers))},
		],
	10000:[
		{'matlab':0.008300, 'F':'rand_row', 'T':100, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.008300, 'F':'rand_col', 'T':100, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000160, 'F':'rand_entry', 'T':100, 'args':'', 'who':range(len(testers))},
		{'matlab':0.021552, 'F':'subgraph', 'T':100, 'args':'1000', 'who':range(len(testers))},
		{'matlab':0.045000, 'F':'scc', 'T':10, 'args':'', 'who':range(len(testers))},
		{'matlab':0.055500, 'F':'graph_mis', 'T':10, 'args':'', 'who':range(len(testers))},
		],
}

bigtests = {
	100000:[
		{'matlab':0.000000, 'F':'rand_row', 'T':10, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'rand_col', 'T':10, 'args':'100', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'rand_entry', 'T':10, 'args':'', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'subgraph', 'T':10, 'args':'10000', 'who':range(len(testers))},
		{'matlab':0.000000, 'F':'scc', 'T':10, 'args':'', 'who':range(len(testers))},
		{'matlab':0.891000, 'F':'graph_mis', 'T':10, 'args':'', 'who':range(len(testers))},
		],
}


def time_my_shit(test,debug=False):
	import_str = 'from __main__ import testers'
	results = {}
	for tester in test['who']:
		rnd.seed(10)							# for consistency
		test_str = 'testers[%i].%s(%s)' % (tester,test['F'],test['args'])

		timer = timeit.Timer(test_str, import_str)
		if debug:
			num_trials = 1
		else:
			num_trials = test['T']
		results[tester] = max(timer.repeat(repeat=3,number=num_trials))/num_trials

	test['results'] = results

def print_test(test):
	print '%s(%s) -- %i trials:' % (test['F'], test['args'], test['T'])
	for i in test['who']:
		print '%12s %10.2f usec' % (testers[i].name,
									test['results'][i]*1000000)

for N in tests:
	G = networkx.read_adjlist('testmat%i.adjlist' % (N))
	N = len(G.nodes())
	print 'matrix loaded:', N, 'nodes',
	E = G.edges()
	E = map(lambda x: (int(x[0]),int(x[1])), E)
	print 'and', len(E), 'edges'

	print 'initializing testers... ',
	for tester in testers:
		tester.set_graph(E,N)
		print tester.name,
	print

	for test in tests[N]:
		time_my_shit(test,debug)
		print_test(test)

if not debug:
	import pickle
	from time import strftime
	pickle.dump(tests,file('tests-'+strftime("%Y-%m-%d-%H-%M-%S")+'.pickle','w'))

"""
matrix loaded... 1000 nodes
...all 2041 edges translated
performing 1000 trials each
rand_row(10) networkx 84.15 usec/pass
rand_row(10) pysparse 20.71 usec/pass
rand_row(10) scipy 232.18 usec/pass
rand_row(10) pygraph 102.39 usec/pass
rand_row(10) graph_tool 177.62 usec/pass
rand_row(10) igraph 1566.61 usec/pass
MATLAB: 200 usec/pass

rand_row(100) networkx 695.91 usec/pass
rand_row(100) pysparse 56.17 usec/pass
rand_row(100) scipy 2406.19 usec/pass
rand_row(100) pygraph 2046.91 usec/pass
rand_row(100) graph_tool 2634.01 usec/pass
rand_row(100) igraph 11445.63 usec/pass
MATLAB: 3800 usec/pass

rand_col(10) networkx 42.67 usec/pass
rand_col(10) pysparse 6.58 usec/pass
rand_col(10) scipy 131.98 usec/pass
rand_col(10) pygraph 107.21 usec/pass
rand_col(10) graph_tool 178.79 usec/pass
rand_col(10) igraph 1275.87 usec/pass
MATLAB: 200 usec/pass

rand_col(100) networkx 477.75 usec/pass
rand_col(100) pysparse 36.24 usec/pass
rand_col(100) scipy 944.18 usec/pass
rand_col(100) pygraph 1103.12 usec/pass
rand_col(100) graph_tool 1752.70 usec/pass
rand_col(100) igraph 8042.28 usec/pass
MATLAB: 3800 usec/pass

rand_entry() networkx 7.45 usec/pass
rand_entry() pysparse 2.34 usec/pass
rand_entry() scipy 31.53 usec/pass
rand_entry() pygraph 17.37 usec/pass
rand_entry() graph_tool 26.30 usec/pass
rand_entry() igraph 486.14 usec/pass
MATLAB: 130 usec/pass

matrix loaded... 10000 nodes
...all 40247 edges translated
performing 1000 trials each
rand_row(100) networkx 392.36 usec/pass
rand_row(100) pysparse 23.71 usec/pass
rand_row(100) scipy 1511.65 usec/pass
rand_row(100) pygraph 1358.34 usec/pass
rand_row(100) graph_tool 1788.53 usec/pass
MATLAB: 8300 usec/pass

rand_row(1000) networkx 3986.29 usec/pass
rand_row(1000) pysparse 704.27 usec/pass
rand_row(1000) scipy 13258.92 usec/pass
rand_row(1000) pygraph 23520.08 usec/pass
rand_row(1000) graph_tool 17100.06 usec/pass
MATLAB: 70000 usec/pass

rand_col(100) networkx 386.77 usec/pass
rand_col(100) pysparse 21.55 usec/pass
rand_col(100) scipy 951.98 usec/pass
rand_col(100) pygraph 1039.45 usec/pass
rand_col(100) graph_tool 1673.78 usec/pass
MATLAB: 8300 usec/pass

rand_col(1000) networkx 3927.62 usec/pass
rand_col(1000) pysparse 627.34 usec/pass
rand_col(1000) scipy 9474.15 usec/pass
rand_col(1000) pygraph 12062.26 usec/pass
rand_col(1000) graph_tool 16548.65 usec/pass
MATLAB: 70000 usec/pass

rand_entry() networkx 8.31 usec/pass
rand_entry() pysparse 2.60 usec/pass
rand_entry() scipy 31.31 usec/pass
rand_entry() pygraph 11.28 usec/pass
rand_entry() graph_tool 25.80 usec/pass
MATLAB: 160 usec/pass

matrix loaded... 1000 nodes
...all 2041 edges translated
performing 1000 trials each
subgraph(50) networkx 30.46 usec/pass
subgraph(50) pysparse 92.36 usec/pass
subgraph(50) scipy 981.32 usec/pass
subgraph(50) graph_tool 216.20 usec/pass
MATLAB: 3100 usec/pass

matrix loaded... 10000 nodes
...all 40247 edges translated
performing 1000 trials each
subgraph(500) networkx 141.32 usec/pass
subgraph(500) pysparse 8100.42 usec/pass
subgraph(500) scipy 6623.46 usec/pass
subgraph(500) graph_tool 243.91 usec/pass
MATLAB: 5900 usec/pass

matrix loaded... 10000 nodes
...all 40247 edges translated
performing 1000 trials each
subgraph(5000) networkx 1513.81 usec/pass
subgraph(5000) pysparse 1104167.23 usec/pass
subgraph(5000) scipy 61959.55 usec/pass
subgraph(5000) graph_tool 539.73 usec/pass
MATLAB: 611800 usec/pass

matrix loaded... 1000 nodes
...all 2041 edges translated
performing 100 trials each
scc() networkx 9260.05 usec/pass
scc() pygraph 19291.47 usec/pass
scc() graph_tool 577.69 usec/pass
pscc() igraph 980399.76 usec/pass
MATLAB: 3100 usec/pass

matrix loaded... 10000 nodes
...all 40247 edges translated
performing 10 trials each
scc() networkx 154569.51 usec/pass
scc() pygraph 1718555.28 usec/pass
scc() graph_tool 5629.21 usec/pass
MATLAB: 45000 usec/pass
"""
