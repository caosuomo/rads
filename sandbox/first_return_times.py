import networkx as nx

def first_return_times(G,k):
	d = dict()
	length = nx.all_pairs_shortest_path_length(G,k)
	for i in G.nodes_iter():
		distances = [length[j][i]+1 for j in filter(lambda x: length[x].has_key(i),G.successors(i))]
		print distances
		if distances:
			d[i] = min(distances)
			
		# for j in G.successors_iter(i):
		# 	if length[j].has_key(i) and length[j][i] + 1 < d[i]:
		# 		d[i] = length[j][i] + 1
	return d


if __name__ == "__main__":
	G = nx.binomial_graph(20,0.1,directed=True)
	d = first_return_times(G,4) # return times <= 4
	print d
