#ifndef OGLE_SF_SFX_H
#define OGLE_SF_SFX_H

#include <OpenAL/al.h>

namespace ogle {
namespace sf {

class SFx {
public:
  SFx();
  ~SFx();

  ALuint get() const;

protected:
  ALuint buffer;
};
}
}

#endif // OGLE_SF_SFX_H
