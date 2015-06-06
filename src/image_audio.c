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
#include "image.h"
#include "audio.h"
#include "common.h"


static void calc_pixel_colors(Image *image, size_t x, size_t y,
                              int *index, struct sound *sound);

static void calc_pixel_contrast(Image *image, size_t x, size_t y,
                                int *index, struct sound *sound);


struct sound *image_to_audio(Image *image, Image *mask)
{
  size_t x, y;
  struct pixel *mask_pixel;
  int index;
  struct sound sound;
  struct sound *sounds = calloc(N_SOUNDS, sizeof (struct sound));

  if (1) {
    calc_pixel_colors(image, image->width / 2, image->height / 2,
                      &index, &sound);
    sounds[index].channels[0] = sound.channels[0];
    sounds[index].channels[1] = sound.channels[1];
  } else {
    for (x = 0; x < image->width; x++) {
      for (y = 0; y < image->height; y++) {
        if (mask) {
          mask_pixel = get_pixel(mask, mask->width * x / image->width,
                                 mask->height * y / image->height);
          if ((mask_pixel->r + mask_pixel->g + mask_pixel->b) < 0xff + 0x80) {
            continue;
          }
        }
        calc_pixel_contrast(image, x, y, &index, &sound);
        sounds[index].channels[0] += (sound.channels[0]
                                      / (image->width * image->height));
        sounds[index].channels[0] += (sound.channels[0]
                                      / (image->width * image->height));

      }
    }
  }

  return sounds;
}


static void calc_pixel_colors(Image *image, size_t x, size_t y,
                              int *index, struct sound *sound)
{
  struct pixel *pixel;
  double amplitude;
  double max_amp = 0xff + 1; /* This is max amplitude + 1. */
  pixel = get_pixel(image, x, y);
  *index = rgb_to_hue(pixel) * N_SOUNDS;
  amplitude = max(max(pixel->r, pixel->g), pixel->b)
    - min(min(pixel->r, pixel->g), pixel->b);

  sound->channels[0] = amplitude / max_amp;
  sound->channels[1] = amplitude / max_amp;
}


static void calc_pixel_contrast(Image *image, size_t x, size_t y,
                                int *index, struct sound *sound)
{
  uint16_t p1, p2, p3;
  struct pixel *pixel;
  double diff;
  double max_diff = pow(0xff * 3, 2) + 1; /* This is max diff value + 1. */

  if (x == 0 || y == 0) {
    return;
  }

  pixel = get_pixel(image, x, y);
  p1 = pixel->r + pixel->g + pixel->b;
  pixel = get_pixel(image, x - 1, y);
  p2 = pixel->r + pixel->g + pixel->b;
  pixel = get_pixel(image, x, y - 1);
  p3 = pixel->r + pixel->g + pixel->b;

  diff = pow(max(max(p1, p2), p3) - min(min(p1, p2), p3), 2) * 0.2;
  *index = (double)(p1 + p2 + p3) / (256 * 3 * 3) * N_SOUNDS;

  sound->channels[0] = (double)abs(x - image->width) * diff / max_diff;
  sound->channels[1] = (double)x * diff / max_diff;
}
