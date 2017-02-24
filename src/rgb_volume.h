/*
RGB color volume manipulation functions
*/

#ifndef RGB_VOL_H
#define RGB_VOL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "image.h"

using namespace std;


static image<rgb> *loadRGBVolume(const char *name) {

  std::ifstream file(name);
  
  int dims[3];
  int v;
  for (int i=0; i<3; i++)
    file >> dims[i];

  int width = dims[0];
  int height = dims[1];
  int depth = dims[2];
  image<rgb> *im = new image<rgb>(width, height, depth);

  for (int z=0; z<depth; z++)
    for (int y=0; y<height; y++)
      for (int x=0; x<width; x++)
      {
        file >> v;
        *imPtr(im, x, y, z).r = (uchar)v;
        file >> v;
        *imPtr(im, x, y, z).g = (uchar)v;
        file >> v;
        *imPtr(im, x, y, z).b = (uchar)v;  
      }
  
  return im;
} 

static void saveRGBVolume(image<rgb> *im, const char *name) {

  std::ofstream file(name);
  
  int width = im->width();
  int height = im->height();
  int depth = im->depth();
  printf("width %d height %d depth %d\n", width, height, depth);

  if (file.is_open())
  {
    file << width << '\n';
    file << height << '\n';
    file << depth << '\n';

    for (int z=0; z<depth; z++)
      for (int y=0; y<height; y++)
        for (int x=0; x<width; x++)
          file << (int)imRef(im, x, y, z).r << ' ' << (int)imRef(im, x, y, z).g << ' ' << (int)imRef(im, x, y, z).b << '\n';
  }
} 

#endif
