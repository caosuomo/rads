import networkx as nx

def first_return_times(G,k):
	return_times = dict()

	# length = shortest path lengths <= k
	# length[i][j] = length of shortest path i->j, if <= k
        # length[i] a dict keyed by neighbors of node i, with values
        # length of path to j
	length = nx.all_pairs_shortest_path_length(G,k)
	for i in G.nodes_iter():
		# nodes = list of successors j which return to i
		nodes = filter(lambda j: length[j].has_key(i),G.successors(i))
		# distances for each successor j
		distances = [length[j][i]+1 for j in nodes]
		if distances:
			return_times[i] = min(distances)

	return return_times, length


if __name__ == "__main__":
	G = nx.binomial_graph(10,0.1,directed=True)
	d, ld = first_return_times(G,4) # return times <= 4
	print "return times", d
