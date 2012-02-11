"""
Base class for a symbolic system used to compute the entropy of a
dynamical system, F. (See [1]_)

A Symbolic_System can be intialized empty. Minimally, the user must
provide the index map and generators on homology computed for a
dynamical system in order to verify the entropy of the system F.

Inherits from
----------

Index_Map

Homology_Generators

Parameters
---------

index_map : induced map on homology (dictionary keyed by ordered pairs, with values giving the map)

generators : generators of homology for disjoint regions of phase
  space (dictionary keyed by region).

Example
-------

index_map = {  (38,1)   :    -1
                         (68,2)   :    -1
                         (69,2)   :    -1
                         ....
                         }

generators = { region_1 : 

References
---------

.. [1] S. Day, R. Frongillo, R. Trevino, Algorithms for rigorous entropy bounds and symbolic dynamics. SIAM J. Appl. Dyn. Sys. 2008
"""
