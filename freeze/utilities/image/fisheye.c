//
//  fisheye.c
//  ots-proto
//
//  Created by Matthew Fonken on 5/31/22.
//

#include "fisheye.h"

void unfisheyePixel(double x, double y, camera_intrinsics_t intrinsics, int iter_num, double * xo, double * yo)
{
    double fx, fy, cx, cy, k1, k2, p1, p2;
    populate_intrinsics( &intrinsics, &fx, &fy, &cx, &cy, &k1, &k2, &p1, &p2 );
    double x0 = x = ( x - cx ) / fx;
    double y0 = y = ( y - cy ) / fy;
    for( int i = 0; i < iter_num; i++ )
    {
        double xx = x * x;
        double yy = y * y;
        double xy = x * y;
        double r2 = xx + yy;
        double k_inv = 1 / ( 1 + ( k1 * r2 ) + ( k2 * r2 * r2 ) );
        double delta_x = ( 2 * p1 * xy ) + ( p2 * ( r2 + ( 2 * xx ) ) );
        double delta_y = ( p1 * ( r2 + ( 2 * yy ) ) ) + ( 2 * p2 * xy );
        x = ( x0 - delta_x ) * k_inv;
        y = ( y0 - delta_y ) * k_inv;
    }
    *xo = x * fx + cx;
    *yo = y * fy + cy;
}
