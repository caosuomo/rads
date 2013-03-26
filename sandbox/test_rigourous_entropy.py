from rads.symbolics.rigorous_entropy import RigorousEntropy

# TEXT and NPY
fname_npy = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.npy'
#fname_txt = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/test_array.txt'

reg_fname =  '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.pkl'

# MAT
fname_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_index.mat'
reg_mat = '/Users/jberwald/github/local/caja-matematica/rads/sandbox/leslie_gens.mat'
matname = 'hom_matrix'

# load from file
re1 = RigorousEntropy()
re1.load_from_file( fname_npy, reg_fname )
re1.prepare_regions()
re1.compute_entropy()


# load from matrix and dict
hom_matrix = utils.load_matlab_matrix( fname_mat, matname )
region2gen = utils.convert_matlab_gens( reg_mat )

re2 = RigorousEntropy( index_map=hom_matrix,
                       reg2gen=region2gen )    
re2.prepare_regions()
re2.compute_entropy()
