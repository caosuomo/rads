import networkx as nx
from rads.graphs import algorithms
from rads.misc import utils
import numpy as np
import random, time
import subprocess as sp
import cPickle as pkl

debug = True

# from SO http://stackoverflow.com/questions/6800193
factors = lambda n: set(reduce(list.__add__, 
                  ([i, n//i] for i in range(1, int(n**0.5) + 1)
                   if n % i == 0)))


class ComputeIndex( object ):

    def __init__( self, adj, mvm, tree, iso_hood, mapname='', prefix='/var/tmp/' ):
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
        self.isolating_nbhd = iso_hood
        self.mapname = mapname
        self.prefix = prefix

        # scale the self.tree.boxes to an integer grid
        self.scale_boxes()

        # regions to be populated later
        self.X = None
        self.A = None
        self.Y = None
        self.B = None

    def __repr__( self ):
        s = "ComputeIndex object for '"+self.mapname+\
        "' map on isolating neighborhood with "+\
        str( len( self.isolating_nbhd ) )+" boxes."
        return s

    def get_corners( self ):
        return self.tree.boxes().corners

    def mapper( self, nbunch ):
        """
        Apply the combinatorial MVM to a node or set of nodes.
        """
        FX = set()
        if type( nbunch ) == int:
            FX.update( self.mvm.neighbors( nbunch ) )
        else:
            for u in nbunch:
                FX.update( self.mvm.neighbors( u ) )    
        return FX
        

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

        # this includes elements in iso, I, so we subtract it to get
        # just the 'hood
        oI = self._get_onebox( I ) - I

        # map the invariant set forward to get I \cup F(I)
        FI = self.mapper( I )

        # just keep the image that intersects the one box nhd
        X = I.union( FI )
        X = X & oI

        # initial exit set A intersected with onebox 'hood
        A = X - I
        A = A & oI # this should be unnecessary, right?

        # grow the exit set
        while 1:
            # image of exit set, F(A)
            FA = self.mapper( A )

            # only keep images that intersect the one box 'hood            
            FA = FA & oI 
            if FA.intersection( I ):
                print "eek!", len( FA.intersection( I ) )
            # images we haven't seen yet
            new_boxes = FA - A
            # print "new boxes", len(new_boxes)
            A.update( new_boxes )
            if not new_boxes:
                break
        X = I.union(A)
        self.X = X
        self.A = A

    def _get_onebox( self, N ):
        # call function below
        return get_onebox( N, self.adj )

    def computeYB( self ):
        """
        Finding the image, (Y,B) of (X,A) under F.

        X : from index pair (X,A)

        A : Exit set. So we can find I=X\A below

        Returns Y=F(X) and B=F(X)\I
        """
        I = self.isolating_nbhd #.copy()

        # map X forward to get Y = X \cup F(X)
        FX = self.mapper( self.X )
        Y = self.X.union(FX)
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

    def scale_boxes( self ):
        """
        Scale box corners so that they land on an integer grid.
        """
        # scale the entire phase space
        boxes = self.tree.boxes()

        # array of box widths in each dimension
        r = boxes.width
        corners = boxes.corners

        self.scaled_boxes= np.empty_like( corners )
        for i in range( self.tree.dim ):
            # assume uniform box size in each dimension
            self.scaled_boxes[:,i] = corners[:,i] / r[i]

        # Use ceiling function to keep even deparation on integer lattice:
        # Note: ceil( x_i / r ) +1 = ceil( x_{i+1}/ r )
        # Proof: x_{i+1} = x_i + r => (x_{i+1}/r) = 
        # (x_i + r)/r = x_i/r + 1 => ceil( x_i / r) + 1 = ceil( x_{i=1}/r)
        c = np.ceil( self.scaled_boxes )
        self.scaled_boxes = np.asarray( c, dtype=np.int )

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
        boxes = self.scaled_boxes[idx]

        with open( fname, 'w' ) as fh:
            fh.write( 'dimension '+str( self.tree.dim )+'\n' )
            # converts row \in A |--> (x,y,...) str coords in implicit
            # loop
            rows = map( lambda x: str(x)+'\n', map( tuple, iter( boxes ) ) ) 
            fh.writelines( rows )

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
                               prefix+'-B.'+suffix, 
                               '-g', 
                               prefix+'.gen' ] )
                               #'>'+prefix+'.out'] )
        if debug:
            print "homcubes called with the following command:"
            print command
        try:
            self._hom_output=sp.check_output( command, shell=True, stderr=sp.STDOUT )
            if debug:
                print "homcubes output:", self._hom_output
            self.index_map_dict = self._homcubes2pydict()
            # get the map as a matrix
            self._index_map_to_matrix()
        # this is dangerous!
        except:
            pass

    def _homcubes2pydict( self ):
        """Strip off the rest of homcubes' invisible characters. Search for
        the first '{' that starts the Python dictionary.
        """
        self._hom_warnings = self._find_homcubes_warnings()
        if len( self._hom_warnings ) > 0:
            print "Warnings returned by Homcubes!"
            print self._hom_warnings

        s = self._hom_output
        pos = s.find( '{' )
        s = s[ pos: ]
        return eval( s )

    def _find_homcubes_warnings( self ):
        """
        Perform a quick search of self._hom_output for warning messages.
        """
        warnings = []
        output = self._hom_output.split( '\n' )
        for line in output:
            if line.find( 'SERIOUS WARNING' ) > 0:
                warnings.append( line )
            elif line.find( 'WARNING' ) and not line.find( 'SERIOUS' ):
                warnings.append( line )
        return warnings
                
            

    def _index_map_to_matrix( self ):
        self.index_map = index_homcubes_to_matrix( self.index_map_dict, return_mat=True )

    def write_index_map( self, fname ):
        with open( fname+'.pkl', 'w' ) as fh:
            pkl.dump( self.index_map, fh )

##========================================================


def grow_isolating_neighborhood( region, adjacency, transition ):
    """region : list of boxes (indices) to grow an isolating neighborhood
    around. (any Python iterable)

    adjacency : adjacency graph, nodes <--> box ids

    transition : transition graph, nodes <--> box ids

    Grow a combinatorial isolating neighborhood N containing region. Return
    None if condition is not met.

    Algorithm 1 in Day, et al, 2008.

    """
    assert len( region ) > 0, "Number of boxes must be greater than 0."
    N = region
    while 1:
        mis = max_invariant_set( N, transition ) 
        oN = get_onebox( mis, adjacency )

        # if nothing changed with the mis and onebox 'hood in this
        # iteration, return mis
        if oN.issubset( N ):
            return mis
        else:
            N = oN
 
def get_onebox( S, adjacency ):
    """
    Return an isolating, one-box neighborhood around the boxes in
    S consisting of node indices in self.adj

    S : indices of boxes corresponding to nodes in transition map

    A : adjacency graph

    Returns S \cup combinatorial neighborhood of S
    """
    # the one box 'hood
    oS = set() 

    # loop over nodes contained in S, keep only those that are
    # adjacent to nodes in S and not already in oS.
    for idx in S:
        nbrs = adjacency.neighbors( idx )
        oS.update( nbrs )
    return oS

def max_invariant_set( I, transition ):
    """Restrict a region I to the maximal invariant set within that
    region. 

    Return indices of nodes in the maximal invariant set.

    """
    R = transition.subgraph( I )
    S = algorithms.graph_mis( R )
    return set( S )


def index_homcubes_to_matrix( index_map_from_homcubes, 
                              fname=None, dim=None, return_mat=False ):
    """Convert the index map (map on generators) to numpy matrix and save
    to disk.

    index_map_from_homcubes : Python dictionary of index maps on all
    homology levels.

    fname : the name of the file. The dimension will be appended to
    the end. (full path to file.)

    dim : the dimension to write to disk. If dim==None, all
    dimensions, d, are dumped to disk with fname+d.npy
    
    Each dimension lists each generator and its image. 

    Eg., [{ g1 : [(a2,g2),(a3,g3)], g2 : [(-1,g2)] } ]

    means that f(g1) = a2g2 + a3g3, f(g2) = -1g2, where a* are integer
    constants.

    Returns dictionary keyed by dimension with index map matrices as
    values.

    """
    I = index_map_from_homcubes
    index_maps = dict.fromkeys( I.keys() )

    for d in index_maps:
        # list of generators and their maps on d'th
        # dimension. gen_maps is a dictionary keyed by generators on
        # homology level d
        if len( I[d] ) == 0:
            index_maps[d] = np.matrix([0])
            continue
        else:
            n = len( I[d] ) 
            idx_mat = np.zeros( (n,n), dtype=np.int )
            for gen_maps in I[d]:
                for gen,image in gen_maps.items():
                    # now we have a list representing the linear
                    # combination of generators in gen's image
                    for a,h in image:
                        # align with 0-zero based index
                        idx_mat[h-1,gen-1] = a
            index_maps[d] = idx_mat 
    
    return index_maps

def get_cycles( P, max_period=2, debug=False ):
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
        # period-k nodes
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
                print j
                print "cycles", cycles
                print facs
                if j in facs:
                    try:
                        pk.difference_update( cycles[ j ] )
                    except KeyError:
                        pass
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
    try:
        conns = nx.dijkstra_path( G.graph, u, v )
        return conns[1:-1] # trim endpoints
    except nx.NetworkXNoPath:
        return []
        
  
def map_writer( transition, scaled_boxes, mapname, region=None, suffix='.map' ):
    """The transition graph representing a combinatorial enclosure. Write
    out the adjecency structure in a format readable by homcubes.

    tree : box tree associated with the combinatorial enclosure.

    transition : combinatorial multivalued map (MVM)

    scaled_boxes : array of boxes that have been scaled to an integer grid.

    mapname : filename to write map to in 'homcubes format'. Eg., each
    grid box's image under the MVM is represented as:

    (-145,-52)  -> {(-154,-44) (-153,-44) (-152,-44) (-151,-44) (-150,-44) }

    region : Subregion --> subgraph, which will be written to disk instead.

    suffix : [default = .map] homcubes_rads will read anything, but
    user can change this to align with his/her naming convention.

    """
    mvm = transition

    if region is not None:
        xbunch = set( mvm.subgraph( region ).nodes() )
    else:
        xbunch = set( mvm.nodes() )

    xmap = {}
    for u in xbunch:
        # image of every element of the domain of interest
        ximage = set( mvm[u].keys() )
        xmap[u] = ximage 

        #mapname = self.prefix + self.mapname+'.' + suffix
    with open( mapname+suffix, 'w' ) as fh:
        lines = []
        for u in xmap:
            # cube corresponding to the u'th index
            cube_x = str( tuple( scaled_boxes[u] ) )
            image = list( xmap[u] )

            # image of u as a tuple of box corners
            cube_y = map( tuple, iter( scaled_boxes[image] ) ) 
            # if len( cube_y ) == 0:
            #     continue
            line = cube_x + ' -> ' + '{' 
            for cube in cube_y:
                # cubes are space-separated
                line += str( cube ) + ' '
            # strip off the last space
            if not len( cube_y ) == 0:
                line = line[:-1]
            line += '}\n'
            lines.append( line )
        fh.writelines( lines )
