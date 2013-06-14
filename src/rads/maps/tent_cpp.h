#include "mapper.h"

class TentMapper : public Mapper
{
    enum param { r=0 };
public:
  TentMapper();
  IPoint map_point(const IPoint &v) const;
};
