#include "ogle/sf/sff.hpp"

#include <iostream>
#include <OpenAL/al.h>

namespace ogle {
namespace sf {

void check_err(const char *msg) {
  ALenum err;
  err = alGetError();
  if (err != AL_NO_ERROR) {
    std::cout << msg << ": " << alGetString(err) << std::endl;
    ;
  }
}
  
}
}
