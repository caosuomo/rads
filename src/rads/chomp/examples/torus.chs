; This is an example of a chain complex corresponding to a surface of a torus.
; The resulting Betti numbers are: 1 (dim 0), 2 (dim 1) and 1 (dim 2).

; Note: This example is very STRANGE - it is supposed to show how the program
; "homchain" interpretes the data.
; All the tricks used here are explained in comments.


;	*** DO NOT MAKE YOUR EXAMPLES SIMILAR TO THIS ONE!!! ***


chain complex

maximal dimension 2

; there is no need to define how many cells there are in each dimension
; as long as all the cells appear in the formulas for boundaries,
; in particular no definition for dimension 0 is necessary

; dimensions do not need to be defined in any specific order
dimension 2
	boundary one	=  C - G + A
; you can use numbers for names - they will not be confused with coefficients!
	boundary two	= -10 - 5 + G
; formulas for boundaries may be complicated; however, make sure
; that the operators and the numbers are separated with spaces
	boundary three	=  D - 3 * H + 2 * 10 - 10 + 2 * H
; cell names can be really weird as long as each of them forms one word
; which is different from words which already have their special meaning
; and no name begins with the characters '=', ':', '+', '-', '*', ';'.
	boundary @#!?	= -A - F + H
	boundary <*>	=  5 - I + #$%^&
	boundary 13+12	= -L - C + I
; boundaries of some other cells of this diemension can be defined later on

dimension 1
; you can use a colon instead of "=" if you prefer that
	boundary A : p - q
	boundary #$%^& : q - p
	boundary C : r - p
; you can use the symbol '#' as an abbreviation of the word "boundary"
	# D : p - r
	# 5 : s - q
	# F : q - s

dimension 2
; if a boundary is repeated, all its appearances are added together
	boundary seven	=  F
	boundary seven	= -J
	boundary seven	=  L
	boundary eight	= -#$%^& - D + J

dimension 1
; you do not need to use any character to separate the cell from its boundary
	boundary G   r - q
	boundary H   p - s
; you can even play with signs (this may not be supported in the next versions)
	boundary I   s + + - - + - + p
	boundary J   q - r
	boundary 10  r - s
	boundary L   s - r
