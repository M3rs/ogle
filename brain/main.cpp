//#include <algorithm>
//#include <cstdint>
//#include <fstream>
#include <iostream>
//#include <vector>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "sff.hpp"
#include "audio.h"
#include "wav.h"
#include "source.h"

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

  sf::WavFx wbuf("fire.wav");
  sf::Source source;
  source.set_buffer(wbuf);
  source.set_position(0, 2, 0);
  // alSourcei(source, AL_BUFFER, buffer);
  //alSourcei(source, AL_BUFFER, wbuf.get());
  //alSource3f(source, AL_POSITION, 0, 2, 0);
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
  //alDeleteSources(1, &source);
  //alDeleteBuffers(1, &buffer);
  /*
  alcMakeContextCurrent(NULL);
  alcDestroyContext(ctx);
  alcCloseDevice(dev);
  */
}
