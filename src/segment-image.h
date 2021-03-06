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

#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include <cstdlib>
#include <image.h>
#include <misc.h>
#include <filter.h>
#include "segment-graph.h"

// random color
rgb random_rgb(){ 
  rgb c;
  double r;
  
  c.r = (uchar)random();
  c.g = (uchar)random();
  c.b = (uchar)random();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b,
			 int x1, int y1, int z1, int x2, int y2, int z2) {
  return sqrt(square(imRef(r, x1, y1, z1)-imRef(r, x2, y2, z2)) +
	      square(imRef(g, x1, y1, z1)-imRef(g, x2, y2, z2)) +
	      square(imRef(b, x1, y1, z1)-imRef(b, x2, y2, z2)));
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
image<rgb> *segment_image(image<rgb> *im, float sigma, float c, int min_size,
			  int *num_ccs) {
  int width = im->width();
  int height = im->height();
  int depth = im->depth();

  image<float> *r = new image<float>(width, height, depth);
  image<float> *g = new image<float>(width, height, depth);
  image<float> *b = new image<float>(width, height, depth);

  // smooth each color channel  
  for (int z = 0; z < depth; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {     
        imRef(r, x, y, z) = imRef(im, x, y, z).r;
        imRef(g, x, y, z) = imRef(im, x, y, z).g;
        imRef(b, x, y, z) = imRef(im, x, y, z).b;
      }
    }
  }
  image<float> *smooth_r = smooth(r, sigma);
  image<float> *smooth_g = smooth(g, sigma);
  image<float> *smooth_b = smooth(b, sigma);
  delete r;
  delete g;
  delete b;
 
  // build graph
  edge *edges = new edge[width*height*depth*5];
  int num = 0;
  for (int z = 0; z < depth; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        if (x < width-1) {
	  edges[num].a = z * width * height + y * width + x;
	  edges[num].b = z * width * height + y * width + (x+1);
	  edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, z, x+1, y, z);
	  num++;
        }

        if (y < height-1) {
	  edges[num].a = z * width * height + y * width + x;
	  edges[num].b = z * width * height + (y+1) * width + x;
	  edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, z, x, y+1, z);
	  num++;
        }

/*
        if ((x < width-1) && (y < height-1)) {
	  edges[num].a = z * width * height + y * width + x;
	  edges[num].b = z * width * height + (y+1) * width + (x+1);
	  edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, z, x+1, y+1, z);
	  num++;
        }

        if ((x < width-1) && (y > 0)) {
	  edges[num].a = z * width * height + y * width + x;
	  edges[num].b = z * width * height + (y-1) * width + (x+1);
	  edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, z, x+1, y-1, z);
	  num++;
        }
*/

        if (z < depth-1) {
	  edges[num].a = z * width * height + y * width + x;
	  edges[num].b = (z+1) * width * height + y * width + x;
	  edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, z, x, y, z+1);
	  num++;
        }
      }
    }
  }
  delete smooth_r;
  delete smooth_g;
  delete smooth_b;

  // segment
  universe *u = segment_graph(width*height*depth, num, edges, c);
  
  // post process small components
  for (int i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  *num_ccs = u->num_sets();

  image<rgb> *output = new image<rgb>(width, height, depth);

  // pick random colors for each component
  rgb *colors = new rgb[width*height*depth];
  for (int i = 0; i < width*height*depth; i++)
    colors[i] = random_rgb();
 
  for (int z = 0; z < depth; z++) { 
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int comp = u->find(z * width * height + y * width + x);
        imRef(output, x, y, z) = colors[comp];
      }  
    }
  }  

  delete [] colors;  
  delete u;

  return output;
}

#endif
