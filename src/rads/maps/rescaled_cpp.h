#include "mapper.h"

class RescaledMapper : public Mapper
{
  enum param { r=2, a=1, b=0 };
public:
  RescaledMapper();
  IPoint map_point(const IPoint &v) const;
};
