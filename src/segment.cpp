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


    Adapted to handle 3D images by Luiz Alberto Lima - EXP/GEOP/TGEO - Jan/2017
*/

#include <cstdio>
#include <cstdlib>
#include <image.h>
#include <misc.h>
#include "rgb_volume.h"
#include "segment-image.h"

int segment(float sigma, float k, int min_size, char * img_file)
{
  image<rgb> *input = loadRGBVolume(img_file);
	
  printf("processing\n");
  int num_ccs; 
  image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs); 

  saveRGBVolume(seg, argv[5]);

  printf("got %d components\n", num_ccs);
  printf("done! uff...thats hard work.\n");

  return 0;
}

