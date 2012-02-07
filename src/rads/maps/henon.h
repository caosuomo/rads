#include "mapper.h"

class HenonMapper : public Mapper
{
  enum param { A=0, B=1 };
public:
  IPoint map_point(const IPoint &v) const;
};
