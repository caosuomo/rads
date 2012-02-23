names = ['NetworkX', 'PySparse', 'SciPy', 'PyGraph', 'Graph-Tool'] #, 'Matlab']

original_tests = {

	1000:[
		{'matlab':0.000000, 'F':'rand_row', 'T':100, 'args':'100', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'rand_col', 'T':100, 'args':'100', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'rand_entry', 'T':100, 'args':'', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'subgraph', 'T':100, 'args':'100', 'who':[0,1,2,4]},
		{'matlab':0.000000, 'F':'scc', 'T':10, 'args':'', 'who':[0,3,4]},
		{'matlab':0.004000, 'F':'graph_mis', 'T':10, 'args':'', 'who':[0]},
		],
	
	10000:[
		{'matlab':0.008300, 'F':'rand_row', 'T':100, 'args':'100', 'who':range(len(names))},
		{'matlab':0.008300, 'F':'rand_col', 'T':100, 'args':'100', 'who':range(len(names))},
		{'matlab':0.000160, 'F':'rand_entry', 'T':100, 'args':'', 'who':range(len(names))},
		{'matlab':0.021552, 'F':'subgraph', 'T':100, 'args':'1000', 'who':[0,1,2,4]},
		{'matlab':0.045000, 'F':'scc', 'T':10, 'args':'', 'who':[0,3,4]},
		{'matlab':0.055500, 'F':'graph_mis', 'T':10, 'args':'', 'who':[0]},
		],

	100000:[
		{'matlab':0.000000, 'F':'rand_row', 'T':10, 'args':'100', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'rand_col', 'T':10, 'args':'100', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'rand_entry', 'T':10, 'args':'', 'who':range(len(names))},
		{'matlab':0.000000, 'F':'subgraph', 'T':10, 'args':'10000', 'who':[0,1,2,4]},
		{'matlab':0.000000, 'F':'scc', 'T':10, 'args':'', 'who':[0,4]},
		{'matlab':0.891000, 'F':'graph_mis', 'T':10, 'args':'', 'who':[0]},
		],
}

matsize = 10000
matlab = {}
for t in original_tests[matsize]:
	matlab[t['F']] = t['matlab']

import pickle
tests = pickle.load(file('tests-2010-11-29-17-03-01.pickle','r'))

import matplotlib.pyplot as plt
import numpy as np

funcs = ['rand_row','subgraph','scc']
legend_labels = ['Write', 'Read', 'Algs']
fgcolor = np.array([20,50,100])/255.
bgcolor = 0.9 + 0.1*fgcolor
colors = [fgcolor,
		  np.array([98,158,31])/255.,
		  np.array([120,20,10])/255.
		  ]
#colors = ['blue','red','green']
labels = names

width = 0.2
pos = np.arange(len(labels))+0.5
offsets = np.arange(-0.2,0.21,0.2)
bars = []

plt.rcdefaults()
plt.rcParams['ytick.major.pad']='10'
plt.rcParams['ytick.color']=fgcolor
plt.rcParams['xtick.color']=fgcolor
#plt.rcParams['figure.subplot.right']='0.97'
#plt.rcParams['figure.subplot.left']='0.18'
#plt.rcParams['figure.subplot.top']='0.97'
plt.rc('text', color=fgcolor)
plt.rc('axes', linewidth=2, facecolor=bgcolor, edgecolor=fgcolor, labelcolor=fgcolor)

plt.figure(frameon=False)
plt.show()

for t in tests[matsize]:
	if t['F'] not in funcs:
		continue

	i = funcs.index(t['F'])

	print t

	dat_list = [t['results'][w] for w in t['who']]
	dat_who = t['who']
# 	dat_list.append(matlab[funcs[i]])
# 	dat_who.append(5)					# matlab
# 	dat = np.zeros(len(labels))			# default = unimplemented
# 	dat[dat_who] = dat_list
# 	dat = np.array(dat)/max(dat_list)
	dat = np.log((matlab[funcs[i]])/np.array(dat_list))

	b = plt.barh(pos[dat_who] - offsets[i], dat, color=colors[i], height=width, align='center')
	bars.append(b[0])

	for j in range(len(labels)):
		if j not in dat_who:
			plt.text(0, pos[j] - offsets[i] - 0.03, 'not fully implemented', color=colors[i], weight='bold',
					 horizontalalignment='left',
					 verticalalignment='center')

plt.yticks(pos, labels, horizontalalignment='right', size='large')
plt.xlabel('relative performance = log(matlab/python)',size='large')
plt.xlim(-1,6)
lg = plt.legend(bars, legend_labels, loc='lower right')
lg.get_frame().set_fill(False)

#plt.gca().labelpad = 20
#plt.setp(plt.getp(plt.gca(), 'yticklabels'), fontsize=30) 
plt.grid(False)
plt.savefig('/home/raf/Dropbox/cs262a/report/graphs.pdf',bbox_inches='tight')

screw-up

# plt.figure()
# for t in tests[matsize]:
# 	if t['F'] not in ['rand_row','subgraph','scc']:
# 		continue
	
# 	dat = [t['results'][w] for w in t['who']]
# 	dat.append(matlab[t['F']])
# 	dat = np.array(dat)*1000			# milliseconds
# 	labels = [names[w] for w in t['who']]
# 	labels.append('Matlab')
# 	pos = np.arange(len(t['who'])+1) + 0.5

# 	print t['F'],t['who'],dat,pos

# 	plt.barh(pos,dat,align='center')
# 	plt.yticks(pos, labels)
# 	plt.xlabel('milliseconds')
# 	plt.ylabel(t['F'])
# 	plt.title(t['F'])
# 	plt.grid(True)
