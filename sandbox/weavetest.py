import scipy.weave as weave
from scipy.weave import converters
import numpy

def henon(a):
	a = numpy.array(a)
	params = numpy.array([1.4,0.3])
	expr = "[1-params[0]*(a[0]*a[0])+a[1], params[1]*a[0]]"
	return weave.blitz(expr)

def my_sum(a):
    n=int(len(a))
    code="""
    int i;
    long int counter;
    counter =0;
    for(i=0;i<n;i++){
        counter=counter + a(i);
    }
    return_val=counter;
    """

    err=weave.inline(code,['a','n'],type_converters=converters.blitz,compiler='gcc')
    return err

x = numpy.array([1,1])
print henon(x)

a=numpy.array(range(600000))
print my_sum(a)
print sum(range(600000))
