"""
index_processor.py

Author: Jesse Berwald

Opened: Feb. 27, 2012

NOT SURE OF THE INHERITANCE BELOW IS THE FINAL ROAD MAP. 
"""


class IndexMapProcessor( ProhibitedEdges ):
    """
    Process and reduce the index map to a verified symbolic system.
    """
    def __init__( self, index_map, generator_map ):
        """
        """
        self.index_map = index_map
        self.generators = generator_map
    
    
    def verify_cycles( self, max_length=10 ):
        """
        Let N = cl(P\P'), where (P,P') is the index pair, and N=\cup
        N_i, where the N_i are disjoint regions in N, m=1,...,m. Let
        G=<V,E> represents admissible transitions between the regions
        N_i, where V={1,...,m} and (i,j) \in E if F(N_i) \cap N_j \ne
        \emptyset.

        We begin with the (sub)graph computed in
        shift_equivalent(). This allows one to produce a blockmodel
        graph that groups generators by region, and edges by admissible
        transition between region, G = self.map_on_regions.

        A cycle c is verified whenever tr(f_{Pk}^{c}) \ne 0. That is,
        we verify that the Conley index is nontrivial, meaning that
        the invariant corresponding to the trajectory prescribed by c
        is nonempty. Thus, the preimage of the iterinary function,
        \rho is nonempty, giving a semi-conjugacy of the map f with
        the subshift of finite type, \sigma, defined on the symbol
        space of m symbols and transition matrix T==G.

        The index map is represented by an n x n matrix, with the
        submatrix of size nj X ni representing the linear map on
        generators from the Ni to Nj. Thus, f_{Pk}^{c} boils down to a
        product of matrices. To verify nonemptiness of the invariant
        set one then checks that the trace of the product of these
        matrices is nonzero.

        Parameters:
        -----------

        generators : (optional) Dictionary

            Dictionary keyed by region number, with values
            contained a list of generators in each region.

        max_path_length : (optional, but default set very low)

                    
        """
        # Make sure we have a generator map
        if self.generators is None:
            raise AttributeError("Must provide a generator dictionary")  
        regions = self.generators
        # invert the generator map
        inv_map = convert.invert_dictionary( regions )
        # list of lists. blocks = [blocks[i]] =[[gen0,gen1],[gen2],...]
        blocks = list( chain(inv_map.itervalues()) )
        self.map_on_regions = algorithms.blockmodel( self.index_map, blocks )
        algorithms.verified_cycles( self.map_on_regions,
                                    self.index_map,
                                    self.adj_matrix,
                                    max_path_length )



class ProhibitedEdges( EdgeSet ):
    """
    Container object for edge sets. 
    """
    pass

class EdgeSet( object ):
    """
    Given s,t \in V(G), each edge set E \subset E(G) consists of all
    length k path from s -> t.

    An EdgeSet is a container for a collection of dictionaries keyed
    by edges. Dictionary values . Matrix product for resulting
    path
    """
