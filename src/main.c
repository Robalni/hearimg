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

#include "image_audio.h"
#include "options.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>


static void print_help(void);


int main(int argc, char **argv)
{
  Image *image;
  Image *mask;
  struct audio_info *ai1, *ai2;
  FILE *file;

  parse_options(argc, argv);
  if (options.help) {
    print_help();
    return 0;
  }

  SDL_Init(SDL_INIT_AUDIO);
  ai1 = audio_init();
  ai2 = malloc(sizeof ai2);
  memcpy(ai2, ai1, sizeof (struct audio_info));

  if (options.mask_file) {
    file = fopen(options.mask_file, "rb");
    mask = read_image(file);
    fclose(file);
  }

  while (1) {
    image = read_image(stdin);
    ai1->sounds = image_to_audio(image, mask);
    play_audio(ai1);
    free(ai2->sounds);
    free_image(image);

    image = read_image(stdin);
    ai2->sounds = image_to_audio(image, mask);
    play_audio(ai2);
    free(ai1->sounds);
    free_image(image);
  }

  if (mask) {
    free_image(mask);
  }
  audio_end(ai1);
  free(ai2);
  SDL_Quit();
  return 0;
}


static void print_help(void)
{
  puts("HELP");
}
