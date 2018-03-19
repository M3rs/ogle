#ifndef OGLE_SF_SOURCE_H
#define OGLE_SF_SOURCE_H

#include <OpenAL/al.h>

namespace sf {

  // fwd
  class SFx;
  
  /// Sound Source
  class Source {
  public:
    Source();
    ~Source();

    void set_buffer(ALuint buffer);
    void set_buffer(const SFx& wav);
    // TODO: take glm::vec3
    void set_position(float x, float y, float z);

    void play() const;
    void stop() const;
    void set_repeat(bool repeat);
    void set_relative(bool relative);
    bool is_playing() const;
    
  private:
    ALuint source;
  };
}

#endif // OGLE_SF_SOURCE_H
