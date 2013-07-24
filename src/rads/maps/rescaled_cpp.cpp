#include "rescaled_cpp.h"

// First argument is dimension, second map name
// Uses second version of constructor to specify the number of params as well
RescaledMapper::RescaledMapper() : Mapper( 1, 3, "rescaled" )
{
    params[r] = 2.0;
    params[a] = 1.0;
    params[b] = 0.0;  
    param_names[r] = 'r';
    param_names[a] = 'a';
    param_names[b] = 'b';
}

IPoint RescaledMapper::map_point(const IPoint &v) const
{
    // dim is an attribute of Mapper class 
    IPoint w( dim() );
    //double midpoint = a/2 + b; 

    if ( v[0] < params[a]/2 + b )
      w[0] = 2 * (v[0] - params[b]) + params[b];
    else 
      w[0] = 2 * (params[a] - (v[0] - params[b])) + params[b];
    
    return w;
}
