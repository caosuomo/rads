
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from matplotlib.collections import PatchCollection
import matplotlib.path as mpath
import matplotlib.patches as mpatches
import matplotlib.lines as mlines

def show_uboxes(uboxes,S=None,col='b',ecol='k', fig=None):
	if uboxes.dim != 2:
		raise Exception("show_uboxes: dimension must be 2")
	if S is None:
		S = range(uboxes.size)

	patches = []
	for i in S:
		art = mpatches.Rectangle(uboxes.corners[i],uboxes.width[0],uboxes.width[1])
		patches.append(art)

	if not fig:
		fig = plt.figure()

	ax = fig.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.draw()
        plt.show()

	return fig

def show_uboxes_corners( corners, width, S=None, col='b', ecol='k' ):
	"""
	This version takes the corners and width arrays as arguments
	instead.
	"""
	if corners.ndim != 2:
		raise Exception("show_uboxes_corners: dimension must be 2")
	if S is None:
		S = range( len(corners) )

	patches = []
	for i in S:
		art = mpatches.Rectangle( corners[i], width[0], width[1] )
		patches.append(art)

	fig = plt.figure()
	ax = fig.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.show()
	
	return fig


def show_box(b,col='b',ecol='k',alpha=1, fig=None):
	patches = []
	
	# lower left corner at b[0], followed by width and height
	art = mpatches.Rectangle(b[0],b[1,0],b[1,1])
	patches.append(art)

	if fig:
		ax = fig.gca()
	else:
                fig = plt.figure()
		ax = fig.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	collection.set_alpha(alpha)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	plt.show()

	return fig

def show_boxes(boxes,S=None,col='b',ecol='k',alpha=1, fig=None):
	if boxes.dim != 2:
		raise Exception("show_boxes: dimension must be 2")
	if S is None:
		S = range(boxes.size)

	patches = []
	for i in S:
		art = mpatches.Rectangle(boxes.corners[i],boxes.widths[i][0],boxes.widths[i][1])
		patches.append(art)
        
	if fig is not None:
		ax = fig.gca()
	else:
                fig = plt.figure()
		ax = fig.gca()
	ax.hold(True)
	collection = PatchCollection(patches)
	collection.set_facecolor(col)
	collection.set_edgecolor(ecol)
	collection.set_alpha(alpha)
	ax.add_collection(collection,autolim=True)
	ax.autoscale_view()
	#plt.show()
        plt.draw()
        return fig
        

def show_boxes1D( boxes, S=None, col='b', alpha=1, 
                  label=None, fig=None, annotate=False ):
        """boxes : tree.boxes object containing corners and width attributes

        S : indices of subset of boxes to plot

        col : color of the line defining each box in S

        alpha : transparency (1==opaque)

        label : text to label boxes in S

        fig : figure object to add additional boxes to (eg. add exit
        set to previously plotted exit set)

        """
        if boxes.dim != 1:
		raise Exception("show_boxes1D: dimension must be 1. See show_boxes() instead")
        if S is None:
                S = range(boxes.size)
                
        # left hand coords of 1D 'boxes'
        corners = boxes.corners 
        width = boxes.width

        xmin = corners.min()
        xmax = corners.max()
        nx = [ xmin, xmax ]
        ny = [ 0, 0 ]
        
        # plot the entire bounding box. This should have been plotted
        # previously if we're plotting an exit set on the ends of
        # isolating 'hood.
        if fig is not None:                        
		ax = fig.gca()
                ax.set_frame_on( False )
	else:
                fig = plt.figure( figsize=(16,3) )
		ax = fig.gca()
                ax.set_frame_on( False )
                ax.plot( nx, ny, 'k-', lw=4, alpha=0.7, label='Root box' )
        
        # plot the subset of boxes
        labeled = False
        for i in S:
                interval = [ corners[i], corners[i]+width ]
                if not labeled:
                        ax.plot( interval, ny, c=col, lw=8, alpha=alpha, label=label )
                        labeled = True
                else:
                        ax.plot( interval, ny, c=col, lw=8, alpha=alpha )


        domain_width = xmax - xmin
        ax.set_xlim( xmin-0.25*domain_width, xmax+0.1 )
        ax.set_ylim( -0.2, 0.2 )
        ax.set_xticks([])
        ax.set_yticks([])

        # now annotate the boxes with numbers, if so desired.
        if annotate:
                for i in S:
                        box_num = str( i )
                        xloc = corners[i] + width/2.
                        yloc = -0.05
                        ax.text( xloc, yloc, box_num,
                                 ha='center', va='center' )
                        ax.vlines( xloc, yloc+0.01, 0 )
        leg = ax.legend( loc='center left' )
        leg.set_frame_on( False )
        
        plt.draw()
        return fig
        
