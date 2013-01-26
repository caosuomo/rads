
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from matplotlib.collections import PatchCollection
import matplotlib.path as mpath
import matplotlib.patches as mpatches
import matplotlib.lines as mlines

def show_uboxes(uboxes,S=None,col='b',ecol='k'):
	if uboxes.dim != 2:
		raise Exception("show_uboxes: dimension must be 2")
	if S is None:
		S = range(uboxes.size)

	patches = []
	for i in S:
		art = mpatches.Rectangle(uboxes.corners[i],uboxes.width[0],uboxes.width[1])
		patches.append(art)

	ax = plt.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.show()


def show_box(b,col='b',ecol='k',alpha=1):
	patches = []

	# lower left corner at b[0], followed by width and height
	xy = b[0]
	width = np.absolute( b[0,1] - b[0,0] )
	height = np.absolute( b[1,1] - b[1,0] )
	art = mpatches.Rectangle( xy, width, height )
	#art = mpatches.Rectangle(b[0],b[1,0],b[1,1])
	patches.append(art)

	ax = plt.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	collection.set_alpha(alpha)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.show()

def show_boxes(boxes,S=None,col='b',ecol='k',alpha=1):
	if boxes.dim != 2:
		raise Exception("show_boxes: dimension must be 2")
	if S is None:
		S = range(boxes.size)

	patches = []
	for i in S:
		art = mpatches.Rectangle(boxes.corners[i],boxes.widths[i][0],boxes.widths[i][1])
		patches.append(art)

	ax = plt.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	collection.set_alpha(alpha)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.show()

