import matplotlib.pyplot as plt
import numpy as np
import scipy.io

d = np.load('/home/raf/projects/rads/runs/tipping_point_rerun.npz')
es = d['ents_sof'].flatten()
ed = d['ents_dft'].flatten()
em = d['ents_max'].flatten()
data = [ed, es, em]

plt.figure()
bins = np.linspace(0.4, 0.47, 20)
plt.hist(ed,alpha=0.5, bins=bins)
plt.hist(es,alpha=0.5, bins=bins)

plt.figure()
y = map(np.mean,data)
print y
errlow = [np.mean(a)-np.percentile(a,15) for a in data]
errhigh = [np.percentile(a,85)-np.mean(a) for a in data]
print errlow
print errhigh
plt.errorbar(range(len(y)), y, yerr=[errlow,errhigh], fmt='o')
plt.title('DFT, Sofic, Max')
# plt.hist(em,**kwargs)
#plt.hist((ed,es,em),**kwargs)
plt.show()
