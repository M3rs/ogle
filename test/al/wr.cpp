#include <stdio.h>
#include <stdlib.h>

#include "wr.h"

void get_word(FILE* f, char* w)
{
  fread(w, sizeof(char), 4, f);
}

void get_uint(FILE* f, WRuint* i)
{
  fread(i, sizeof(WRuint), 1, f);
}

void get_short(FILE* f, short* s)
{
  fread(s, sizeof(short), 1, f);
}

WAV* WRCreate(const char* filename)
{
  FILE* f = fopen(filename, "rb");
  if (!f) {
    printf("File does not exist %s", filename);
    return NULL;
  }

  WAV* wav = (WAV*)malloc(sizeof(struct wav_t));
  
  char word[5];
  word[4] = '\0';
  
  get_word(f, word); // 'RIFF'

  WRuint size;
  get_uint(f, &size);
  get_word(f, word); // 'WAVE'
  get_word(f, word); // 'fmt '

  WRuint ssize;
  get_uint(f, &ssize);

  get_short(f, &wav->format);
  get_short(f, &wav->num_channels);
  get_uint(f, &wav->sample_rate); // frequency

  WRuint byte_rate;
  get_uint(f, &byte_rate);

  short balign;
  get_short(f, &balign);

  get_short(f, &wav->bitsPerSample);
  get_word(f, word); // 'data'
  get_uint(f, &wav->size);

  wav->data = (char*)malloc(wav->size);
  fread(wav->data, sizeof(char), wav->size, f);

  fclose(f);
  
  return wav;
}

void WRDestroy(WAV* wav)
{
  free(wav->data);
  free(wav);
}
  
