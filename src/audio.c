/*
  Copyright (C) 2015  Robert Alm Nilsson <rorialni@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "audio.h"
#include "common.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>


union sample_bytes {
  Sint16 sample;
  Uint8 bytes[2];
};


static void audio_callback(void *userdata, Uint8 *stream, int len)
{
  struct audio_info *ai = (struct audio_info*)userdata;
  union sample_bytes sb;
  struct sound *sound;
  float volmult = 1.5;
  float value;
  int rel_byte, abs_byte;
  int i_sounds;
  double seconds;
  size_t i_channels;
  float frequency;

  /* +=4 because 2 bytes/sample and 2 channels. */
  for (rel_byte = 0; rel_byte < len; rel_byte += 4) {
    abs_byte = rel_byte + ai->bytes_played;
    seconds = abs_byte / (double)ai->spec.freq;
    for (i_channels = 0;
         i_channels < sizeof sound->channels / sizeof (double);
         i_channels++) {
      value = 0;
      for (i_sounds = 0; i_sounds < N_SOUNDS; i_sounds++) {
        sound = ai->sounds + i_sounds;
        frequency = (float)i_sounds / N_SOUNDS * FREQ_RANGE + MIN_FREQ;
        value += sinf(seconds * frequency * M_PI)
          * sound->channels[i_channels] * volmult * 0x8000;
      }
      if (value >= 0x7999) {
        value = 0x7999;
      } else if (value <= -0x7999) {
        value = -0x7999;
      }
      sb.sample = (Sint16)value;
      stream[rel_byte + i_channels * 2 + 0] = sb.bytes[0];
      stream[rel_byte + i_channels * 2 + 1] = sb.bytes[1];
    }
  }
  ai->bytes_played += len;
}


void play_audio(struct audio_info *ai)
{
  ai->spec.userdata = ai;
  SDL_PauseAudioDevice(ai->dev, 0);
}


struct audio_info *audio_init()
{
  SDL_AudioSpec spec;
  struct audio_info *ai = calloc(1, sizeof (struct audio_info));

  SDL_zero(spec);
  spec.freq = 48000;
  spec.format = AUDIO_S16;
  spec.channels = 2;
  spec.samples = 0x1000;
  spec.callback = audio_callback;
  spec.userdata = ai;
  ai->dev = SDL_OpenAudioDevice(NULL, 0, &spec, &ai->spec, 0);
  ai->bytes_played = 0;

  return ai;
}

void audio_end(struct audio_info *ai)
{
  SDL_CloseAudioDevice(ai->dev);
  free(ai);
}
