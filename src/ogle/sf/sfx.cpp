#include "ogle/sf/sfx.hpp"

namespace ogle {
namespace sf {

SFx::SFx() : buffer(0) {}

SFx::~SFx() {
  if (buffer) {
    alDeleteBuffers(1, &buffer);
    buffer = 0;
  }
}

ALuint SFx::get() const { return buffer; }
}
}
