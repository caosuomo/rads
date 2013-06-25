import networkx as nx
from rads.graphs import algorithms
from rads.misc import utils
import numpy as np
import random, time
import subprocess as sp

debug = True

# from SO http://stackoverflow.com/questions/6800193
factors = lambda n: set(reduce(list.__add__, 
                  ([i, n//i] for i in range(1, int(n**0.5) + 1)
                   if n % i == 0)))


class ProcessHomology( object ):

    def __init__( self, adj, mvm, tree, region, mapname='', prefix='/var/tmp/' ):
        """
        Process the homology of a map in a region of phase space.

        adj : adjacency matrix of boxes in phase space

        mvm : combinatorial multivalued map on boxes in phase space

        tree : box tree object containing boxes in phase space

        region : indices of boxes for region of interest 

        mapname : name of the map. Serves as a prefix for all cubical files written to disk. Eg.,

        If mapname == 'tent', then 'tent-X.cub', 'tent-A.cub',
        etc. will be created when the index pair is foud.

        prefix : The full path to the directory in which the cubical
        files will be stored.

        """
        self.adj = adj
        self.mvm = mvm
        self.tree = tree
        self.region = region
        self.mapname = mapname
        self.prefix = prefix

        # to be populated later
        self.isolating_nbhd = None
        self.X = None
        self.A = None
        self.Y = None
        self.B = None

    # def isolated( self, A, oI, S ):
    #     """
    #     Check whether the maximal invariant set is in the interior of oI.

    #     A : adjacency graph

    #     oI : one box neighborhood

    #     S : maximal invariant set
    #     """
    #     inv_map = self.adj.inv_nodemap #convert.invert_dictionary( A.nodemap )
    #     # does the inv_set have any one-box neighbors that are not in oI?
    #     # Find adjacent boxes in order to figure this out.
    #     oI_adj = set( [inv_map[u] for u in oI] )
    #     S_adj = set( [inv_map[u] for u in S] )
    #     # Are there any neighbors of u in S that are not in oI?  If
    #     # so, then S can't be in the interior.
    #     for u in S_adj:
    #         nbrs_u = set( A.graph.neighbors( u ) )
    #         if not nbrs_u.issubset( oI_adj ):
    #             return False
    #     else:
    #         return True

    def grow_isolating_neighborhood( self ): #, invariant, adjacency, transition ):
        """
        invariant : list of boxes in invariant set (any Python iterable)

        adjacency : Adjacency graph  

        transition : transition graph

        Grow a combinatorial isolating neighborhood N containing S. Return
        None if condition is not met.

        Algorithm 1 in Day, et al, 2008.
        """
        S = self.region
        while 1:							
            N = set( algorithms.graph_mis( self.mvm.subgraph( S ) ) )
            oN = self.get_onebox( N )
            if N.issubset( oN ):
                self.isolating_nbhd = oN
                return oN
            N = oN

    def get_onebox( self, S ):
        """
        Return an isolating, one-box neighborhood around the boxes in
        S. 

        S : indices of boxes corresponding to nodes in transition map

        A : adjacency graph

        Returns S \cup combinatorial neighborhood of S
        """
        # container for (x,y) box tuples
        thehood = set()
        oS = set() 

        # loop over nodes contained in S, keep only those that are
        # adjacent but not already in S.
        for idx in S:
            nbrs = self.adj.neighbors( idx )
            oS.update( nbrs )
        return oS

    def max_inv_set( self, I ):
        """
        Restrict the map P to the maximal isolated invariant neighborhood.

        Return indices of nodes in the maximal invariant set.
        """
        R = self.mvm.subgraph( I )
        S = algorithms.graph_mis( R )
        return set( S )

    def build_index_pair( self ):
        """
        Building modified combinatorial index pairs for the combinatorial
        isolating neighborhood I.

        iso : isolating 'hood. List or set object

        adjacency : adjacency matrix (spacial)

        transittion : combinatorial map object

        Returns the index pair X,A
        """
        # a little loop optimization
        I = self.isolating_nbhd
        A = self.adj
        P = self.mvm

        # create one box nhd about the invariant set
        I = set( I )
        # this include I, so we subtract it to get just the 'hood
        oI = self.get_onebox( I )
        onebox = oI - I

        # initial X as the invariant set; this avoid in-place update
        #X = I.copy()
        FI = set()

        # map the invariant set forward to get I \cup F(I)
        for u in I:
            FI.update( P.neighbors( u ) )
        # just keep the image that intersects the one box nhd
        X = I.union( FI )
        X = X & oI

        # initial exit set A intersected with onebox 'hood
        A = X - I
        A = A & oI
        #onebox = oI - I

        # grow the exit set
        while 1:

            print "A", A

            FA = set()
            # image of exit set, F(A)
            for u in A:
                FA.update( P.neighbors( u ) )
            # only keep images that intersect the one box 'hood            
            FA = FA & onebox # oI
            if FA.intersection( I ):
                print "eek!", len( FA.intersection( I ) )
            # images we haven't seen in the one box 'hood
            new_boxes = FA - A
            # print "new boxes", len(new_boxes)
            A.update( new_boxes )
            if not new_boxes:
                break
        X = I.union(A)
        self.X = X
        self.A = A

    def computeYB( self ):
        """
        Finding the image, (Y,B) of (X,A) under F.

        X : from index pair (X,A)

        A : Exit set. So we can find I=X\A below

        Returns Y=F(X) and B=F(X)\I
        """
        # if we don't copy, union below will be in-place
        X_ = self.X.copy()
        I = set( self.isolating_nbhd ).copy()
        FX = set()
        # Find F(X)
        for u in self.X:
            FX.update( self.mvm.neighbors( u ) )

        # F(X)
        Y = X_.union(FX)

        # B = Y - I, where I = X\A
        B = Y - I
        self.Y = Y
        self.B = B

    def make_index_pair( self, return_values=False ):
        """
        iso : list (set) of boxes in isolating neighborhood.

        transition : transition graph on phase space (combinatorial enclosure)

        adjacency : adjacency graph on boxes in phase space

        returns X,A,Y,B
        """
        self.build_index_pair()
        self.computeYB()
        if return_values:
            return self.X, self.A, self.Y, self.B

    def scale_boxes( self, idx=None ):
        """
        Scale box corners so that they land on an integer grid.
        """
        if idx is not None:
            corners = self.tree.boxes().corners[idx]
        else:
            # scale the entire phase space
            corners = self.tree.boxes().corners

        # scaling in each dimension. uniform grid guarantees that we
        # can just pull off first entry in each dimension
        r = corners[1:] - corners[:-1]

        self.boxes= np.empty_like( corners )
        for i in range( self.tree.dim ):
            # assume uniform box size in each dimension
            self.boxes[:,i] = corners[:,i] / r[0,i]

        # convert to int array since we should be on an integer
        # lattice based on the r vector, this should convert to an int
        # array just fine (i.e. no loss of precision)
        self.boxes = np.asarray( self.boxes, dtype=np.int )

    def box2cub( self, idx, fname ):
        """Write corners of boxes[idx], stored in tree, to disk.

        The scaling, done in order to put the corners on a unit grid,
        assumes that the grid is *uniform* in all dimension. 

        The format is that required by Homcubes (see
        http://www.PawelPilarczyk.com/)

        tree : box tree associated with the combinatorial enclosure.

        idx : list of indices of the set of boxes to write to disk

        fname : full path where we should write the cub file.

        """
        idx = list( idx )
        boxes = self.boxes[idx]

        with open( fname, 'w' ) as fh:
            fh.write( 'dimension '+str( self.tree.dim )+'\n' )
            # converts row \in A |--> (x,y,...) str coords in implicit
            # loop
            rows = map( lambda x: str(x)+'\n', map( tuple, iter( boxes ) ) ) 
            fh.writelines( rows )

    def map_writer( self, region=None, suffix='map' ):
        """tree : box tree associated with the combinatorial enclosure.

        transition : combinatorial multivalued map (MVM)

        mapname : filename to write map to in 'homcubes format'. Eg., each
        grid box's image under the MVM is represented as:

        (-145,-52)  -> {(-154,-44) (-153,-44) (-152,-44) (-151,-44) (-150,-44) }

        P is a transition graph representing a combinatorial
        enclosure. Write out the adjecency structure in a format readable
        by homcubes.

        The domain is X, and the range is Y. The points are scaled to an integer grid.

        """
        #dim = self.tree.dim
        #boxes = self.tree.boxes()
        #corners = boxes.corners
    
        if region is not None:
            xbunch = set( self.mvm.subgraph( region ).nodes() )
        else:
            xbunch = set( self.mvm.nodes() )
        
        # scale all corners in domain since we will need more than just
        # those the region being analyzed. A more efficient way to do this
        # may be to call scale_boxes for domidx, and then for each u \in
        # domidx
        # sets self.scaled to the array of scaled box corners
        #boxes = self.scale_boxes()

        xmap = {}
        for u in xbunch:
            # image of every element of the domain of interest
            ximage = set( self.mvm[u].keys() )
            xmap[u] = ximage 

        mapname = self.prefix + self.mapname+'.' + suffix
        with open( mapname, 'w' ) as fh:
            lines = []
            for u in xmap:
                # cube corresponding to the u'th index
                cub_x = str( tuple( self.boxes[u] ) )
                image = list( xmap[u] )

                # image of u as a tuple of box corners
                cub_y = map( tuple, iter( self.boxes[image] ) ) 
                line = cub_x + ' -> ' + '{' 
                for cub in cub_y:
                    # cubes are space-separated
                    line += str( cub ) + ' '
                # strip off the last space
                line = line[:-1]
                line += '}\n'
                lines.append( line )
            fh.writelines( lines )


    def run_homcubes( self, suffix='cub', debug=False ):
        """
        prefix : name of the map being analyzed (including full path to the files).

        save_prefix : prefix of 

        Sample command:

        homcubes -i henon.map henon-X.dat henon-A.dat henon-Y.dat henon-B.dat -R

        where prefix='henon' == full path to data.
        """
        space = ' '
        prefix = self.prefix + self.mapname
        command = space.join( ['homcubes_rads', '-i', '-a',
                               prefix+'.map',
                               prefix+'-X.'+suffix,
                               prefix+'-A.'+suffix,
                               prefix+'-Y.'+suffix,
                               prefix+'-B.'+suffix,   #] )
                               '-g', 
                               prefix+'.gen' ] )
                               #'>'+prefix+'.out'] )
        if debug:
            print "homcubes called with the following command:"
            print command
        try:
            p=sp.check_output( command, shell=True )
            self.index_map = self._homcubes2pydict( p )
        except:
            pass

    def _homcubes2pydict( self, s ):
        """Strip off the rest of homcubes' invisible characters. Search for
        the first '{' that starts the Python dictionary.
        """
        pos = s.find( '{' )
        s = s[ pos: ]
        return eval( s )

##========================================================

def get_cycles( P, max_period=2 ):
    """
    P : Directed graph

    P will be converted to sparse transition matrix for multiplication
    and cycle detection.
    """
    C = utils.to_sparse( P )
    nodes = P.nodes()
    nodes.sort()

    # Create a node map, index --> node number.  The graph
    # representation typically respects the original box ordering (at
    # least at that depth of the tree), whereas the sparse matrix
    # indexing is 0,...,len(nbunch).  The hash allows a quick link
    # between a sparse matrix representation of a combinatorial map
    # and a graph representation.
    # Note: searching a dictionary (eg. x in d) is O(1), while search
    # for containment in a list is O(n). Test using %timeit x in d ==>
    # 23.5 us (list), 70.6 ns (dict).
    nodemap = utils.make_node_hash( nodes )

    # convert to compressed sparse row format for efficiency
    #C = sp.tocsr()
    # period 1,2,..., max_period. This dict format will make plotting
    # different cycles and connections easier in the future.
    if debug:
        print "looking for cycles..."
    cycles = {}
    for k in range( 1, max_period+1 ):
        Ck = C**k 
        pk = set( np.where( Ck.diagonal() )[0] )

        # don't repeat multiples of cycles
        if k > 1:
            # find all factors of k
            facs = list( factors( k ) )[:-1]
            # remove all nodes from cycles of length j < k if j is a
            # factor of k (excluding k itself)
            for j in range( 1, k ):
                # if a shorter cycle of length j is a factor of the
                # cycles length k then we want to exclude it. see
                # above.
                if j in facs:
                    pk.difference_update( cycles[ j ] )
        if len( pk ) == 0:
            continue
        cycles[ k ] = pk

    Pcycles = {}
    for k,cyc in cycles.iteritems():
        pk = set()
        for c in cyc:
            # apply the node map
            pk.add( nodemap[ c ] )
        Pcycles[ k ] = pk
        
    if debug:
        print "Cycle stats:\n"
        ckeys = cycles.keys()
        ckeys.sort()
        for c in ckeys:
            print "    "+str(len(cycles[c]))+" period "+str( c )+\
                " cycles."
        print ""
    return Pcycles

def connect_cycles( A, B, P ):
    """
    A : set of period k orbits

    B : a set of period k' orbits

    P : transition graph

    TODO : Split up and choose only one node/box from each cycle. This
    is redundant as it loops through the cycles.
    """
    C = set()
    for u in A:
        for v in B:
            if u != v:
                # conns holds connecting nodes in path, but not the
                # start/end nodes.
                conn = connections( P, u, v )
                C.update( conn )
    return C

def connections( G, u, v ):
    """
    Find connections from one set of nodes to another (preds and
    succs, resp.).
    """
    conns = nx.dijkstra_path( G.graph, u, v )
    return conns[1:-1] # trim endpoints
