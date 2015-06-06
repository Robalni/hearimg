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

#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>

#define N_SOUNDS 24
#define MIN_FREQ 220
#define FREQ_RANGE 660


struct sound {
  double channels[2];
};


struct audio_info {
  SDL_AudioSpec spec;
  SDL_AudioDeviceID dev;
  size_t bytes_played;
  struct sound *sounds;
};


struct audio_info *audio_init();

void play_audio(struct audio_info *ai);

void audio_end(struct audio_info *ai);


#endif /* AUDIO_H */
