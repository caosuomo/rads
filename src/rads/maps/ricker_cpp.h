#include "mapper.h"

class RickerMapper : public Mapper
{
  // fitness ('r') and dispersal ('d')
  enum param { r=1, d=0 };
public:
  RickerMapper();
  IPoint map_point( const IPoint &v ) const;
};
