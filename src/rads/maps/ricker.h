#include "mapper.h"

class RickerMapper : public Mapper
{
  enum param { r=0, d=1 };
public:
  RickerMapper();
  IPoint map_point( const IPoint &curr ) const;
};
