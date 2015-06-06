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

#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>


struct pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};


typedef struct image {
  size_t width;
  size_t height;
  struct pixel *pixels;
} Image;


Image *read_image(FILE *file);

struct pixel *get_pixel(Image const *image, size_t x, size_t y);

/* Return a value between 0 and 1. */
float rgb_to_hue(struct pixel *pixel);

void free_image(Image *image);


#endif /* IMAGE_H */
