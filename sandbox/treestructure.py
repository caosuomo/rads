
def printvalsfull(c,levs):
	for d in range(2,20):
		gaionodes = (2**(d*levs+1)-1)
		expnodes = ((2**d)**(levs)-1)/(2**d-1)
		gaio = (c+3) * gaionodes
		exp = (c+2**d) * expnodes + (c+1)*(2**(d*levs))
#		print "d =",d,":  gaio =",gaio, " exp =",exp
		print "d =",d,":  e/g =",gaio*1.0/exp, " g-e =",gaio-exp
#		print "  gaionodes =",gaionodes, " expnodes =",expnodes

printvalsfull(3,1)
printvalsfull(3,2)
printvalsfull(3,3)
