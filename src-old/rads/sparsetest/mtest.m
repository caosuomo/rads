
  depth = 10;
  map = get_map('henon',0);

  to_be_subdivided = 8; % flag for subdivision
  delbox = 1;

  for d = 1 : depth
    tic;
	for j = 1 : map.tree.dim
	  map.tree.set_flags('all', to_be_subdivided);
	  map.tree.subdivide;
	end
	times(d,1) = toc;
    
    fprintf('step %d: ', d);

    tic;
	map.P = rigorous_matrix(map);
	map.Adj = adj_matrix(map); % compute the adjacency matrix
	times(d,2) = toc;
    
    tic;
    n = size(map.P, 1);
    [I, I_f] = graph_mis(map.P, map.Adj);
    times(d,3) = toc;
    
    tic;
    inv_bits = repmat('0', [1 n]);
    inv_bits(I) = '1';					% 11/18/2006
    map.tree.set_flags(inv_bits, delbox); 	% flag boxes in I + X
    map.tree.remove(delbox);				% remove non-flagged boxes
    times(d,4) = toc;
    
    fprintf('%d boxes in I, %d boxes total\n', ...
				 length(I), map.tree.count(-1));
  end
