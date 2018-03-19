#include "ogg.hpp"

#include <iostream>
#include <vector>

namespace {
  
#include <vorbis/vorbisfile.h>

  ALuint LoadOGG(const char* file)
{
  FILE* f = fopen(file, "rb");
  if (f == nullptr) {
    std::cout << "error finding ogg file" << std::endl;
    return 0;
  }

  vorbis_info *pInfo;
  OggVorbis_File oggFile;

  if (ov_open(f, &oggFile, NULL, 0) != 0) {
    std::cout << "error opening ogg file" << std::endl;
    return 0;
  }

  ALenum format;
  pInfo = ov_info(&oggFile, -1);
  if (pInfo->channels == 1) {
    format = AL_FORMAT_MONO16;
  } else {
    format = AL_FORMAT_STEREO16;
  }

  //pInfo->rate; // frequency
  static const int BUF_SIZE = 32768;
  char array[BUF_SIZE];

  std::vector<char> data;
  long bytes;
  int bitStream;
  do {
    bytes = ov_read(&oggFile, array, BUF_SIZE, 0, 2, 1, &bitStream);
    if (bytes < 0) {
      ov_clear(&oggFile);
      std::cout << "error decoding file" << std::endl;
      return 0;
    }
    data.insert(data.end(), array, array + bytes);
  } while (bytes > 0);

  ALuint buffer;
  alGenBuffers(1, &buffer);
  alBufferData(buffer, format, &data[0], data.size(), pInfo->rate);

  return buffer;
}
}

namespace sf {
  OggFx::OggFx(const char* file)
  {
    buffer = LoadOGG(file);
  }
}
