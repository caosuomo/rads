#include "ricker_cpp.h"

RickerMapper::RickerMapper() : Mapper( 2, "ricker" )
{
  params[r] = 5.0;
  params[d] = 0.0;
  param_names[r] = 'r';
  param_names[d] = 'd';
}

IPoint RickerMapper::map_point(const IPoint &v) const
{
    int i, row, col;
    //int  = dim();
    int sysdim = dim() * dim();

    // square set of patches of size dim x dim
    IPoint w( sysdim );

    // Just a single patch
    if ( sysdim == 1 )
      {
	w[ 0 ] = params[ r ] * v[ 0 ] * exp( -v[ 0 ] );
      }

    // n > 1 patches
    else
      {
	// growth phase, can just run across array
	for ( i = 0; i < sysdim; i ++ )
	  {
	    w[ i ] = params[ r ] * v[ i ] * exp( -v[ i ] );
	  }	
	// dispersal phase
	if ( params[ d ] != 0 )
	  {
	    for ( row=0; row < dim(); row ++ )
	      {
		for ( col=0; col < dim(); col ++ )
		  {
		    // left and right dispersal
		    if ( col != 0 )
		      w[ dim() * row + (col-1) ] = params[ d ] * v[ dim() * row + col] / 4.0;
		    
		    if ( col != dim() - 1 )
		      w[ dim() * row + (col+1) ] = params[ d ] * v[ dim() * row + col] / 4.0;
		    
		    // up and down dispersal
		    if ( row != 0 )
		      w[ dim() * (row - 1) + col ] = params[ d ] * v[ dim() * row + col] / 4.0;
		    
		    if ( row != dim() - 1 )
		      w[ dim() * (row + 1) + col ] = params[ d ] * v[ dim() * row + col] / 4.0;
		    
		    // Now remove the dispersed mass from the vent patch
		    w[ dim() * row + col ] = ( 1 - params[ d ] ) * v[ dim() * row + col ];    
		  } // end for col
	      } // end for row
	  } // end if
      } // end else

    // TESTING
    for ( i = 0; i < sysdim; i ++ )
      std::cout << "w[" << i << "] = " << w[i] << std::endl;

  return w;
}
