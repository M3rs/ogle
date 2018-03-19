#include "ogle/sf/source.h"

#include "ogle/sf/sff.hpp"
#include "ogle/sf/sfx.hpp"

namespace ogle {
namespace sf {

  Source::Source()
    : source(0)
  {
    alGenSources(1, &source);
    check_err("gen source");
  }

  Source::~Source() {
    if (source) {
      alDeleteSources(1, &source);
    }
  }

  void Source::set_buffer(const sf::SFx &wav)
  {
    alSourcei(source, AL_BUFFER, wav.get());
    check_err("set source buffer");
  }

  void Source::set_position(float x, float y, float z)
  {
    alSource3f(source, AL_POSITION, x, y, z);
    check_err("set source position");
  }

  void Source::play() const {
    alSourcePlay(source);
    check_err("play source");
  }
  void Source::pause() const {
    alSourcePause(source);
    check_err("pause source");
  }
  void Source::stop() const {
    alSourceStop(source);
    check_err("stop source");
  }
  void Source::set_repeat(bool repeat)
  {
    alSourcei(source, AL_LOOPING, repeat ? 1 : 0);
  }
  void Source::set_relative(bool relative)
  {
    alSourcei(source, AL_SOURCE_RELATIVE, relative? AL_TRUE:AL_FALSE);
    check_err("set relative");
  }
  void Source::set_gain(float gain)
  {
    alSourcef(source, AL_GAIN, gain);
    check_err("set gain");
  }
  bool Source::is_playing() const
  {
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state); // err check?
    return state == AL_PLAYING;
  }
    
}
}
