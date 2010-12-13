import numpy as np
from numpy import random as rnd
import graph_tool as gt
import graph_tool.topology as top
import graph_tool.community as comm


def graph_mis(G):
	comp_prop = top.label_components(G)
	print comp_prop.a
	(C,ec,vc) = comm.condensation_graph(G,comp_prop)
	print C.num_vertices()
	print ec.a
	print vc.a


E = [(0, 8), (0, 5), (1, 19), (1, 10), (1, 3), (1, 5), (2, 11), (2,
 4), (2, 7), (4, 19), (6, 19), (6, 3), (6, 5), (7, 19), (7, 3), (8,
 0), (9, 16), (9, 14), (9, 6), (10, 3), (10, 12), (11, 19), (11, 6),
 (12, 3), (12, 12), (13, 8), (13, 19), (13, 11), (13, 5), (14, 1),
 (14, 9), (16, 3), (17, 1), (18, 16), (18, 13), (19, 9)]
G = gt.Graph(directed=True)
G.add_vertex(20)
for (u,v) in E:
	G.add_edge(G.vertex(u),G.vertex(v))
correct_answer = [0, 8, 12, 10, 1, 19, 9, 6, 14]

graph_mis(G)
