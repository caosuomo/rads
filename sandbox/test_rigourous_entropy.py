from rads.symbolics.rigorous_entropy import RigorousEntropy
from rads.misc import utils
import subprocess as sp

# TEXT and NPY
fname_npy = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.npy'
#fname_txt = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/test_array.txt'

reg_fname =  '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.pkl'

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

pri# nt ""

# # load from matrix and dict
# print "Testing ability to start off with Matlab matrices..." 
# hom_matrix = utils.load_matlab_matrix( fname_mat, matname )
# region2gen = utils.convert_matlab_gens( reg_mat )

# re2 = RigorousEntropy( index_map=hom_matrix,
#                        reg2gen=region2gen )    
# re2.prepare_regions()
# re2.compute_entropy()
# re2.get_max_entropy()


