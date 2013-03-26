#include "ricker_cpp.h"

RickerMapper::RickerMapper() : Mapper( 3, "ricker" )
{
  params[r] = 5;
  params[d] = 0.1;
  param_names[r] = 'r';
  param_names[d] = 'd';
}

IPoint RickerMapper::map_point(const IPoint &curr) const
{
    int row, col;
    int d = dim();

    // square set of patches of size dim x dim
    IPoint w( d * d );
    
    // growth phase, can just run across array
    for ( row = 0; row < d * d; row ++ )
      {
	w[ row ] = params[ r ] * curr[ row ] * exp( -curr[ row ] );
      }

    // dispersal phase
    for ( row=0; row < d; row ++ )
      for ( col=0; col < d; col ++ )
	{
	  // left and right dispersal
	  if ( col != 0 )
	    w[ d * row + (col-1) ] = params[ d ] * curr[ d * row + col] / 4.0;
	  if ( col != d - 1 )
	    w[ d * row + (col+1) ] = params[ d ] * curr[ d * row + col] / 4.0;
	  // up and down dispersal
	  if ( row != 0 )
	    w[ d * (row - 1) + col ] = params[ d ] * curr[ d * row + col] / 4.0;
	  if ( row != d - 1 )
	    w[ d * (row + 1) + col ] = params[ d ] * curr[ d * row + col] / 4.0;
	  // Now remove the dispersed mass from the current patch
	  w[ d * row + col ] = ( 1 - params[ d ] ) * curr[ d * row + col ];    
  return w;
}
