#ifndef OGLE_SF_AUDIO_H
#define OGLE_SF_AUDIO_H

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace sf {
class Audio {
public:
  Audio();
  ~Audio();

  /// Init must be called before audio can do anything
  /// Sets some default listener parameters
  bool init();

  /// listener methods
  void set_position(float x, float y, float z);
  void set_velocity(float x, float y, float z);
  /// float[6] fwd:x,y,z, up:x,y,z
  void set_orientation(float* orient);

private:
  ALCdevice *dev;
  ALCcontext *ctx;
};
 
}

#endif // OGLE_SF_AUDIO_H
