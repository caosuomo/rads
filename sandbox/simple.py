# This file was *autogenerated* from the file simple.sage.
from sage.all_cmdline import *   # import sage library
_sage_const_2 = Integer(2)
"""
simple.sage
this is a simple sage script
"""

__tmp__=var("x"); f = symbolic_expression(x**_sage_const_2).function(x)
print f(_sage_const_2 )

def solve_square(n):
    """ returns the value of f(x) in
    f(x) = x**2
    """
    return f(n)
