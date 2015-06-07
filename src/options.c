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

#include "options.h"

#include <stdlib.h>
#include <getopt.h>


void parse_options(int argc, char **argv)
{
  int c;
  struct option longopts[] = {
    {"continuous", no_argument, NULL, 'c'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
  };

  options.argv0 = argv[0];

  while ((c = getopt_long(argc, argv, "ch", longopts, NULL)) != -1) {
    switch (c) {
    case 'c':
      options.continuous = 1;
      break;
    case 'h':
      options.help = 1;
      break;
    }
  }

  if (optind < argc) {
    options.mask_file = argv[optind];
  } else {
    options.mask_file = NULL;
  }
}
