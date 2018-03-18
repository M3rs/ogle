#include "wav.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "sff.hpp"

namespace {

  #include <cstdint>
  // https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file#13661263

typedef struct WAV_HEADER
{
  // RIFF CHUNK
  uint8_t RIFF[4]; // RIFF header magic
  uint32_t ChunkSize; // RIFF chunk size
  uint8_t WAVE[4]; // WAVE Header
  // FMT sub-chunk
  uint8_t fmt[4]; // FMT header
  uint32_t SubchunkSize; // Size of the fmt chunk
  uint16_t AudioFormat; // Audio format: 1=PCM, 6=mulaw,7=alwa, 257=IBM Mu-Law, 258= IBM A-Law, 259=ADPCM
  uint16_t NumOfChan; // Number of channels: 1=Mono, 2=Stereo
  uint32_t SamplesPerSec; // Sampling Frequency in Hz
  uint32_t bytesPerSec; // bytes per Second
  uint16_t blockAlign; // 2=16-bit mono, 4=16-bit stereo
  uint16_t bitsPerSample; // Numbers of bits per sample
  // data sub-chunk
  uint8_t Subchunk2ID[4]; // "data" string
  uint32_t Subchunk2Size; // Sampled data length
} wav_hdr;

ALenum wave_format(wav_hdr* wave)
{
  switch (wave->blockAlign) {
  case 1:
    switch (wave->NumOfChan) {
    case 1:
      return AL_FORMAT_MONO8;
    case 2:
      return AL_FORMAT_STEREO8;
    }
    break;
  case 2:
    switch (wave->NumOfChan) {
    case 1:
      return AL_FORMAT_MONO16;
    case 2:
      return AL_FORMAT_STEREO16;
    }
    break;
  }

  return 0;
}
}

namespace sf {

  WavFx::WavFx(const char *file)
    : buffer(0)
  {
    // TODO: check file exists
    std::ifstream ifs(file, std::fstream::binary);
    wav_hdr wh;

    ifs.read((char*)&wh, sizeof(wav_hdr));
    std::vector<int8_t> data;
    std::copy(std::istreambuf_iterator<char>(ifs),
	      std::istreambuf_iterator<char>(),
	      std::back_inserter(data));

    alGenBuffers(1, &buffer);
    check_err("gen buffer");

    alBufferData(buffer,
		 wave_format(&wh),
		 &data[0],
		 data.size(),
		 wh.SamplesPerSec);
    check_err("buffer data");
  }

  WavFx::~WavFx() {
    if (buffer) {
      alDeleteBuffers(1, &buffer);
      buffer = 0;
    }
  }

  ALuint WavFx::get() const {
    return buffer;
  }
}
