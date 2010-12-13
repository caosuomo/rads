giaopath = '/usr/local/gaio-2.2.8';
% add GAIO
addpath(giaopath)
addpath([giaopath '/matlab'])
addpath([giaopath '/gaio'])
addpath([giaopath '/models/i486-libc6/'])
addpath(genpath('/home/raf/projects/ds/matlab'))
addpath('/home/raf/projects/rads/src/build/lib.linux-i686-2.6/')

dims = [1:8];
depths = {1:10,
          1:8,
          1:4,
          1:3,
          1:3,
          1:3,
          1:2,
          1:2};
xdepths = {1:20,
          1:12,
          1:8,
          1:6,
          1:5,
          1:4,
          1:3,
          1:3};
funcs = {'insert', 'insert_box', 'search', 'search_box'};
R = {};

printrun = @(f,t) fprintf('%12s: %6.8f\n',f,1000*t);

for dim_index = 1 : length(dims)

  results = zeros(length(depths{dim_index}),length(funcs));

  for depth_index = 1 : length(depths{dim_index})
    dim = dims(dim_index);
    depth = depths{dim_index}(depth_index);
    tree = Tree(zeros(1,dim),ones(1,dim));
    for d = 1 : depth
      for j = 1 : dim
        tree.set_flags('all', 8);
        tree.subdivide;
      end
    end

    fprintf('bench_*_dim_%i_depth_%i.txt\n', dim, depth);
    
    f = 1;
    fname = sprintf('bench_%s_dim_%i_depth_%i.txt', funcs{f}, dim, depth);
    %disp(fname)
    D = load(fname);
    tic;
    for i = 1:size(D,1)
      tree.insert(D(i,:)',depth*dim);
    end
    tictoc = toc / size(D,1);
    results(depth_index,f) = tictoc;
    printrun(funcs{f},tictoc);
    
    f = f+1;
    fname = sprintf('bench_%s_dim_%i_depth_%i.txt', funcs{f}, dim, depth);
    %disp(fname)
    D = load(fname);
    tic;
    for i = 1:(size(D,1)/2)
      tree.insert_box(D(2*i-1,:)',D(2*i,:)',depth*dim);
    end
    tictoc = toc * 2 / size(D,1);
    results(depth_index,f) = tictoc;
    printrun(funcs{f},tictoc);

    reps = 1000;
    f = f+1;
    fname = sprintf('bench_%s_dim_%i_depth_%i.txt', funcs{f}, dim, depth);
    %disp(fname)
    D = load(fname);
    tic;
    for i = 1:size(D,1)
      for j = 1:reps
        tree.search(D(i,:)',depth*dim);
      end
    end
    tictoc = toc / (reps * size(D,1));
    results(depth_index,f) = tictoc;
    printrun(funcs{f},tictoc);

    reps = 50;
    f = f+1;
    fname = sprintf('bench_%s_dim_%i_depth_%i.txt', funcs{f}, dim, depth);
    %disp(fname)
    D = load(fname);
    tic;
    for i = 1:(size(D,1)/2)
      for j = 1:reps
        tree.search_box(D(2*i-1,:)',D(2*i,:)',depth*dim);
      end
    end
    tictoc = toc * 2 / (reps * size(D,1));
    results(depth_index,f) = tictoc;
    printrun(funcs{f},tictoc);
    
    delete(tree); clear tree;
  end

  results = max(results,0.00000000001); % play fair now, no 0's
  
  R{dim_index} = results;
end

save bench-tree.mat R