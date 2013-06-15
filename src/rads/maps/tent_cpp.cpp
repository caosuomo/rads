#include "tent_cpp.h"

// First argument is dimension, second map name
TentMapper::TentMapper() : Mapper( 1, "tent" )
{
    params[r] = 0.5;  // can be accessed and changed at Python level
    param_names[r] = 'r';
}

IPoint TentMapper::map_point(const IPoint &v) const
{
    // dim is an attribute of Mapper class 
    IPoint w( dim() );
    //double midpoint = 0.5; 

    if ( v[0] <= 0.5 )
      w[0] = params[r] * v[0];
    else 
      w[0] = params[r] * (1 - v[0]);
    
    return w;
}
