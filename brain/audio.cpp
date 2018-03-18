#include "audio.h"

#include "sff.hpp"
#include <iostream>

namespace sf {

Audio::Audio() : dev(nullptr), ctx(nullptr) {}

Audio::~Audio() {
  if (ctx) {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
  }
  if (dev) {
    alcCloseDevice(dev);
  }
}

bool Audio::init() {
  dev = alcOpenDevice(NULL);
  if (!dev) {
    check_err("alc device");
    std::cout << "alc device err" << std::endl;
    return false;
  }

  ctx = alcCreateContext(dev, NULL);
  alcMakeContextCurrent(ctx);
  if (!ctx) {
    check_err("alc context");
    std::cout << "alc context err" << std::endl;
    return false;
  }

  alListener3f(AL_POSITION, 0, 1.5, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  float orient[6] = {/*fwd*/ 0, 0, -1, /*up*/ 0, 1, 0};
  alListenerfv(AL_ORIENTATION, orient);

  return true;
}

// TODO: check error for these!
void set_position(float x, float y, float z) {
  alListener3f(AL_POSITION, x, y, z);
}
void set_velocity(float x, float y, float z) {
  alListener3f(AL_VELOCITY, x, y, z);
}

/// float[6]
void set_orientation(float* orient) {
  // TOOD: impl with vecs
  alListenerfv(AL_ORIENTATION, orient);
}
}
