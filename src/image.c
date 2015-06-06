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

#include "image.h"
#include "options.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <pam.h>


Image *read_image(FILE *file)
{
  Image *image;
  struct pam inpam;
  tuple * tuplerow;
  int row;

  pm_init(options.argv0, 0);
  pnm_readpaminit(file, &inpam, sizeof inpam.tuple_type);
  tuplerow = pnm_allocpamrow(&inpam);

  image = malloc(sizeof (Image));
  image->width = inpam.width;
  image->height = inpam.height;
  image->pixels = malloc(sizeof (struct pixel)
                         * image->width * image->height);

  for (row = 0; row < inpam.height; row++) {
    int column;
    pnm_readpamrow(&inpam, tuplerow);
    if (inpam.depth != 3) {
      fputs("depth != 3", stderr);
      break;
    }
    for (column = 0; column < inpam.width; column++) {
      get_pixel(image, column, row)->r = tuplerow[column][0] / 0x100;
      get_pixel(image, column, row)->g = tuplerow[column][1] / 0x100;
      get_pixel(image, column, row)->b = tuplerow[column][2] / 0x100;
    }
  }

  pnm_freepamrow(tuplerow);

  return image;
}


void free_image(Image *image)
{
  free(image->pixels);
  free(image);
}


float rgb_to_hue(struct pixel *pixel)
{
  uint8_t max_color = max(max(pixel->r, pixel->g), pixel->b);
  uint8_t min_color = min(min(pixel->r, pixel->g), pixel->b);
  float hue;

  if (max_color == min_color) {
    return 0;
  }

  if (pixel->r == max_color) {
    hue = (float)(pixel->g - pixel->b) / (max_color - min_color);
  } else if (pixel->g == max_color) {
    hue = 2 + (float)(pixel->b - pixel->r) / (max_color - min_color);
  } else if (pixel->b == max_color) {
    hue = 4 + (float)(pixel->r - pixel->g) / (max_color - min_color);
  }
  hue /= 6;
  if (hue < 0) {
    hue += 1;
  } else if (hue > 1) {
    hue -= 1;
  }

  return hue;
}


struct pixel *get_pixel(Image const *image, size_t x, size_t y)
{
  return image->pixels + (y * image->width + x);
}
