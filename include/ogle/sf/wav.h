#ifndef OGLE_SF_WAV_H
#define OGLE_SF_WAV_H

#include "ogle/sf/sfx.hpp"
//#include <OpenAL/al.h>

namespace ogle {
namespace sf {

  class WavFx : public SFx {
public:
  WavFx(const char *file);
};
}
}

#endif // OGLE_SF_WAV_H
