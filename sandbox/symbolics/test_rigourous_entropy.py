from rads.symbolics.rigorous_entropy import RigorousEntropy
from rads.misc import utils

# TEXT and NPY
fname_npy = 'leslie_index.npy'
#fname_txt = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/test_array.txt'

reg_fname =  'leslie_gens.pkl'

# MAT
fname_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.mat'
reg_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.mat'
matname = 'hom_matrix'

# load from file
print "Testing with Numpy matices..."
re1 = RigorousEntropy()
re1.load_from_file( fname_npy, reg_fname )
re1.prepare_regions()
re1.compute_entropy()
re1.get_max_entropy()

print "Drawing all verified semi-conjugate subshifts..."
#re1.draw()

print ""

## EXPECTED OUTPUT
# >>> Testing with Numpy matices...
# >>> Maximum entropy found:  0.150632111601


##############################
# test matlab functionality
##############################

# load from matrix and dict
print "Testing ability to start off with Matlab matrices..." 
hom_matrix = utils.load_matlab_matrix( fname_mat, matname )
region2gen = utils.convert_matlab_gens( reg_mat )

re2 = RigorousEntropy( index_map=hom_matrix,
                       reg2gen=region2gen )    
re2.prepare_regions()
re2.compute_entropy()
re2.get_max_entropy()

## EXPECTED OUTPUT
# Testing ability to start off with Matlab matrices...
# Maximum entropy found:  0.150632111601
