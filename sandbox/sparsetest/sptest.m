function sptest

addpath(genpath('/home/raf/projects/ds/matlab'))
R = 1000;
subgraph = 1000;
load testmat10000.mat
N = size(A,1);
trials = 1000;
format long;

fprintf('subgraph(%i)\n',subgraph)
M = A;
tic;
for t = 1:trials
  S = randint(1,subgraph,N)+1;
  M(S,S);
end
disp(toc/trials)

return

for i=1:10
fprintf('graph_mis\n')
M = A;
tahm=cputime;
tic
for t = 1:trials
  graph_mis(M);
end
toc
disp((cputime - tahm)/trials)
end

fprintf('scc()\n')
M = A;
tahm=cputime;
tic
for t = 1:trials
  components_raf(M);
end
toc
disp((cputime - tahm)/trials)

fprintf('rand_entry()\n')
M = A;
tahm=cputime;
tic
for t = 1:trials
  M(1+randint(1,1,N),1+randint(1,1,N)) = 1;
end
toc
disp((cputime - tahm)/trials)

fprintf('rand_row(%i)\n',R)
M = A;
tahm=cputime;
tic
for t = 1:trials
  M(1+randint(1,R,N),1+randint(1,1,N)) = 1;
end
toc
disp((cputime - tahm)/trials)

fprintf('rand_col(%i)\n',R)
M = A;
tahm=cputime;
tic
for t = 1:trials
  M(1+randint(1,1,N),1+randint(1,R,N)) = 1;
end
toc
disp((cputime - tahm)/trials)

%A = sparse(1000,1000);
%for i=1:length(s)      
  %A(s(i,1)+1,s(i,2)+1)=1;
%end