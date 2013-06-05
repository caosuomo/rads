#include "mapper.h"

class RickerMapper : public Mapper
{
<<<<<<< HEAD
  enum param { r=0, d=1 };
public:
  RickerMapper();
  IPoint map_point( const IPoint &curr ) const;
=======
  // fitness ('r') and dispersal ('d')
    enum param { r=1, d=0 };

    public:
    RickerMapper();
    /* RickerMapper( int fitness, int dispersal ) */
    /*   { */
    /* 	param[r] = fitness; */
    /* 	param[d] = disperal; */
    /*   } */

  IPoint map_point( const IPoint &v ) const;
>>>>>>> adding-capd
};
