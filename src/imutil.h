/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* some image utilities */

#ifndef IMUTIL_H
#define IMUTIL_H

#include "image.h"
#include "misc.h"

/* compute minimum and maximum value in an image */
template <class T>
void min_max(image<T> *im, T *ret_min, T *ret_max) {
  int width = im->width();
  int height = im->height();
  int depth = im->depth();
  
  T min = imRef(im, 0, 0, 0);
  T max = imRef(im, 0, 0, 0);
  for (int z = 0; z < depth; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        T val = imRef(im, x, y, z);
        if (min > val)
	  min = val;
        if (max < val)
	  max = val;
      }
    }
  }

  *ret_min = min;
  *ret_max = max;
} 

/* threshold image */
template <class T>
image<uchar> *threshold(image<T> *src, int t) {
  int width = src->width();
  int height = src->height();
  int depth = src->depth();
  image<uchar> *dst = new image<uchar>(width, height, depth);
  
  for (int z = 0; z < height; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        imRef(dst, x, y, z) = (imRef(src, x, y, z) >= t);
      }
    }
  }

  return dst;
}

#endif

