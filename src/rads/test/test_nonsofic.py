#!/usr/bin/python

import sys
import numpy as np
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor
import matplotlib.pyplot as plt

gens = np.matrix([[2,0,1,0],[0,1,0,1],[1,1,1,3],[-1,-1,0,1]])
regions = {0:[0,1],1:[2,3]}
sof = SoficProcessor(gens,regions)
ents = []
for i in range(300):
    sof.process(1)
    ents.append(sof.entropy())
    if i % 10 == 0:
        print sof, ents[-1]

print sof
print sof.entropy()

plt.figure()
plt.plot(np.log(ents[-1] - np.array(ents[:-1])))

gens = np.matrix([[0,1,1],[1,1,1],[1,1,1]])
regions = {0:[0,1],1:[2]}
sof = SoficProcessor(gens,regions)
ents2 = []
for i in range(40):
    sof.process(1)
    ents2.append(sof.entropy())

plt.figure()
plt.plot(np.log(np.log(2) - np.array(ents2)))

print sof
print sof.entropy()

plt.show()
