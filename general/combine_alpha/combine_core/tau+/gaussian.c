//
//  gaussian.c
//  tau+
//
//  Created by Matthew Fonken on 11/6/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "gaussian.h"

void gaussianBlurInt( gaussian_t * g, int * arr, size_t len, int scale )
{
    size_t n, k_len = g->length;

    double res[len + k_len - 1];

    for (n = 0; n < len + k_len - 1; n++)
    {
        size_t kmin, kmax, k;

        res[n] = 0;

        kmin = (n >= k_len - 1) ? n - (k_len - 1) : 0;
        kmax = (n < len - 1) ? n : len - 1;

        for (k = kmin; k <= kmax; k++)
        {
            res[n] += (GAUSS_FLOAT)arr[k] * g->array[n - k];
        }
    }
    int offset = (int)(( k_len - 1 ) / 2);
    for (n = 0; n < len; n++)
    {
        arr[n] = (int)(res[n + offset]*scale);
    }
}


void gaussianBlur( gaussian_t * g, GAUSS_FLOAT * arr, size_t len )
{
    size_t n, k_len = g->length;

    double res[len + k_len - 1];

    for (n = 0; n < len + k_len - 1; n++)
    {
        size_t kmin, kmax, k;

        res[n] = 0;

        kmin = (n >= k_len - 1) ? n - (k_len - 1) : 0;
        kmax = (n < len - 1) ? n : len - 1;

        for (k = kmin; k <= kmax; k++)
        {
            res[n] += (GAUSS_FLOAT)arr[k] * g->array[n - k];
        }
    }
    int offset = (int)(( k_len - 1 ) / 2);
    for (n = 0; n < len; n++)
    {
        arr[n] = res[n + offset];
    }
}

GAUSS_FLOAT gaussianDistribution( int x, int mu, GAUSS_FLOAT sigma)
{
    int d = x - mu;
    GAUSS_FLOAT n = 1.0 / (sqrt(2 * M_PI) * sigma);
    return exp(-d*d/(2 * sigma * sigma)) * n;
};

void generateGaussian( gaussian_t * g, int l, GAUSS_FLOAT s )
{
    if( l == g->length && s == g->sigma) return;
    int m = floor(l/2);
    for(int i = 0, d = l; i <= m; i++, d--)
    {
        g->array[i] = gaussianDistribution(i,m,s);
        if(i != d) g->array[d] = g->array[i];
    }
    g->length = l;
    g->sigma = s;
}

void initGaussian( gaussian_t * g, int l )
{
    (*g).array = (GAUSS_FLOAT*)malloc(sizeof(GAUSS_FLOAT)*l);
    (*g).length = l;
}
