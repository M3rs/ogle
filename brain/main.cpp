//#include <algorithm>
//#include <cstdint>
//#include <fstream>
#include <iostream>
#include <vector>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//#include <vorbis/vorbisfile.h>

#include "sff.hpp"
#include "audio.h"
#include "wav.hpp"
#include "ogg.hpp"
#include "source.h"

// ALuint LoadOGG(const char* file)
// {
//   FILE* f = fopen(file, "rb");
//   if (f == nullptr) {
//     std::cout << "error finding ogg file" << std::endl;
//     return 0;
//   }

//   vorbis_info *pInfo;
//   OggVorbis_File oggFile;

//   if (ov_open(f, &oggFile, NULL, 0) != 0) {
//     std::cout << "error opening ogg file" << std::endl;
//     return 0;
//   }

//   ALenum format;
//   pInfo = ov_info(&oggFile, -1);
//   if (pInfo->channels == 1) {
//     format = AL_FORMAT_MONO16;
//   } else {
//     format = AL_FORMAT_STEREO16;
//   }

//   //pInfo->rate; // frequency
//   static const int BUF_SIZE = 32768;
//   char array[BUF_SIZE];

//   std::vector<char> data;
//   long bytes;
//   int bitStream;
//   do {
//     bytes = ov_read(&oggFile, array, BUF_SIZE, 0, 2, 1, &bitStream);
//     if (bytes < 0) {
//       ov_clear(&oggFile);
//       std::cout << "error decoding file" << std::endl;
//       return 0;
//     }
//     data.insert(data.end(), array, array + bytes);
//   } while (bytes > 0);

//   ALuint buffer;
//   alGenBuffers(1, &buffer);
//   alBufferData(buffer, format, &data[0], data.size(), pInfo->rate);

//   return buffer;
// }

int main(int argc, char **argv) {
  /*
  ALCdevice *dev;
  ALCcontext *ctx;

  dev = alcOpenDevice(NULL);
  if (!dev) {
    std::cout << "oops" << std::endl;
    return 1;
  }

  ctx = alcCreateContext(dev, NULL);
  alcMakeContextCurrent(ctx);
  if (!ctx) {
    std::cout << "oops" << std::endl;
    return 1;
  }
  */

  // std::cout << alGetString(AL_EXTENSIONS) << std::endl;

  //ALuint source; //, buffer;

  //alGenBuffers(1, &buffer);
  //alGenSources(1, &source);
  //sf::check_err("gen source");

  sf::Audio audio;
  if (!audio.init()) {
    std::cout << "Failed to init audio" << std::endl;
    return 1;
  }

  // // TODO: means to set audio listener / attributes
  
  // alListener3f(AL_POSITION, 0, 1.5, 0);
  // alListener3f(AL_VELOCITY, 0, 0, 0);
  // float orient[6] = {/*fwd*/ 0, 0, -1, /*up*/ 0, 1, 0};
  // alListenerfv(AL_ORIENTATION, orient);

  //sf::WavFx wbuf("bgm1.wav");
  //ALuint wbuf = LoadOGG("Holst-_venus.ogg");
  //sf::OggFx wbuf("Holst-_venus.ogg");
	sf::WavFx wbuf("footsteps-1.wav");
  sf::Source source;
  source.set_buffer(wbuf);
  source.set_position(0, 2, 0);
  // alSourcei(source, AL_BUFFER, buffer);
  //alSourcei(source, AL_BUFFER, wbuf.get());
  //alSource3f(source, AL_POSITION, 0, 2, 0);
  source.set_relative(true);
  source.set_repeat(true);
  source.play();
  //alSourcePlay(source);
  //sf::check_err("play source");

  //ALenum state;
  bool playing = true;
  do {
    //alGetSourcei(source, AL_SOURCE_STATE, &state);
    playing = source.is_playing();
  } while (playing); //state == AL_PLAYING);

  // clean up
  //alDeleteBuffers(1, &wbuf);
  //alDeleteSources(1, &source);
  //alDeleteBuffers(1, &buffer);
  /*
  alcMakeContextCurrent(NULL);
  alcDestroyContext(ctx);
  alcCloseDevice(dev);
  */
}
