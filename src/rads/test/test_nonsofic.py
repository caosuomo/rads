#!/usr/bin/python

import sys
import numpy as np
from rads.misc import utils
from rads.symbolics.sofic_processor import SoficProcessor
import matplotlib.pyplot as plt

def plot_convergence(sof,steps,true_value=None):
    ents = []
    for i in range(steps):
        sof.process(1)
        ents.append(sof.entropy())
        if i % 10 == 0:
            print sof, ents[-1]
    print sof, 'entropy:', ents[-1]
    plt.figure()
    if true_value:
        ents.append(true_value)
    plt.plot(np.log(ents[-1] - np.array(ents[:-1])))
    return ents

gens = np.matrix([[2,0,1,0],[0,1,0,1],[1,1,1,3],[-1,-1,0,1]])
regions = {0:[0,1],1:[2,3]}
sof = SoficProcessor(gens,regions)
# plot_convergence(sof,300)

gens = np.matrix([[0,1,1],[1,1,1],[1,1,1]])
regions = {0:[0,1],1:[2]}
sof = SoficProcessor(gens,regions)
#plot_convergence(sof,40, true_value=np.log(2))

gens = np.matrix([[0,1,1,1],[1,1,0,0],[1,1,0,1],[0,0,1,1]])
regions = {0:[0,1],1:[2,3]}
sof = SoficProcessor(gens,regions)
plot_convergence(sof,40, true_value=np.log(2))

plt.show()
