from rads.symbolics.rigorous_entropy import RigorousEntropy
from rads.misc import utils

# TEXT and NPY
fname_npy = 'leslie_index.npy'
reg_fname =  'leslie_gens.pkl'

# MAT -- Using Bill's data
fname_mat = './bill_kalies_data/index2.mat'
reg_mat = './bill_kalies_data/generators2.mat'
matname = 'hom_matrix'

# load from file
# print "Testing with Numpy matices..."
# re1 = RigorousEntropy()
# re1.load_from_file( fname_npy, reg_fname )
# re1.prepare_regions()
# re1.compute_entropy()
# re1.get_max_entropy()

#print "Drawing all verified semi-conjugate subshifts..."
#re1.draw()

print ""

## EXPECTED OUTPUT for Leslie map files
# >>> Testing with Numpy matices...
# >>> Maximum entropy found:  0.150632111601


##############################
# test matlab functionality
##############################

max_path = 6

# load from matrix and dict
print "Testing ability to start off with Matlab matrices..." 
hom_matrix = utils.load_matlab_matrix( fname_mat, matname )
region2gen = utils.convert_matlab_gens( reg_mat )

re2 = RigorousEntropy( index_map=hom_matrix,
                       reg2gen=region2gen )    
re2.prepare_regions()

# drawing the map on regions
print "Drawing the maps on strongly connected components (disjoint regions in the MVM)...\n"
fig = re2.map_on_regions.draw()

re2.compute_entropy( max_path_length=max_path )
re2.get_max_entropy()

