#ifndef O_WR_H
#define O_WR_H

typedef unsigned int WRuint;

typedef struct wav_t {
  short format;
  short num_channels;
  WRuint sample_rate; // frequency
  short bitsPerSample;
  WRuint size;
  char* data;

} WAV;

WAV* WRCreate(const char* filename);
void WRDestroy(WAV* wav);

#endif // O_WR_H
