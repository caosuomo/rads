import networkx as nx
from rads.graphs import DiGraph
from rads.graphs.algorithms import graph_mis

if __name__ == "__main__":
	E = [(0, 8), (0, 5), (1, 19), (1, 10), (1, 3), (1, 5), (2, 11), (2, 4), (2, 7), (4, 19), (6, 19), (6, 3), (6, 5), (7, 19), (7, 3), (8, 0), (9, 16), (9, 14), (9, 6), (10, 3), (10, 12), (11, 19), (11, 6), (12, 3), (12, 12), (13, 8), (13, 19), (13, 11), (13, 5), (14, 1), (14, 9), (16, 3), (17, 1), (18, 16), (18, 13), (19, 9)]
	G = DiGraph()
	G.add_edges_from(E)
	correct_answer = [0, 1, 6, 8, 9, 10, 12, 14, 19]
	my_answer = sorted(graph_mis(G))
	print 'graph_mis(G) =  ', my_answer
	print 'correct answer =', correct_answer

	E = [(0,1), (1,2), (2,0), (2,3), (3,4), (1,5), (5,6), (6,6), (7,0), (8,7)]
	G = DiGraph()
	G.add_edges_from(E)
	my_answer = sorted(graph_mis(G))
	print 'graph_mis(G) =  ', my_answer
	print 'correct answer =', [0,1,2,5,6]

