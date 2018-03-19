#ifndef OGLE_SF_WAV_H
#define OGLE_SF_WAV_H

#include "sfx.hpp"

namespace sf {

class WavFx : public SFx {
public:
  WavFx(const char *file);
};
}

#endif // OGLE_SF_WAV_H
