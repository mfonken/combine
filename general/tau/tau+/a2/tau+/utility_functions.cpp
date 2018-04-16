//
//  utility_functions.cpp
//  tau+
//
//  Created by Matthew Fonken on 10/6/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "utility_functions.hpp"

Vec3b preset(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);

/*** UTILITY FUNCTIONS ***/
void initImg( cimage_t i, int w, int h )
{
    printf("mallocing %lu-%dx%d %d bytes\n", sizeof(pixel_base_t), h, w, (int)sizeof(pixel_base_t) * h * w);
    i = (pixel_base_t *)malloc(sizeof(pixel_base_t) * h * w);
}

void MatToCimage( uint16_t width, uint16_t height, cv::Mat mat, cimage_t img, int threshold )
{
    int x, y, z = 0;
    for(y = height; y > 0; --y)
    {
        for(x = width; x > 0; --x)
        {
            Vec3b pixel = mat.at<Vec3b>(y, x);
            img[z] = pixel[1] > threshold;
            z++;
        }
    }
}

void cimageToBandW( uint16_t width, uint16_t height, cimage_t img, Mat mat )
{
    int x, y, z = 0;
    for(y = height; y > 0; --y)
    {
        for(x = width; x > 0; --x)
        {
            if(img[z++])
                mat.at<Vec3b>(y, x) = preset;
        }
    }
}

void deriveDensityMap( density_map_t * m )
{
    int l = m->length;
    for(int i = 1; i < l; i++)
    {
        m->map[i-1] -= m->map[i];
    }
    m->map[l-1] = 0;
}
