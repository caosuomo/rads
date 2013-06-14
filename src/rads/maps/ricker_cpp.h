#include "mapper.h"

class RickerMapper : public Mapper
{
  // fitness ('r') and dispersal ('d')
    enum param { r=0, d=1 };

    public:
    RickerMapper();
    IPoint map_point( const IPoint &v ) const;
};
