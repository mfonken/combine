//
//  fisheye.h
//  ots-proto
//
//  Created by Matthew Fonken on 5/31/22.
//

#ifndef fisheye_h
#define fisheye_h

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdbool.h>

#define FISHEYE_EPS 1e-8

typedef struct
{
    double K[3][3];
    double D[4];
} camera_intrinsics_t;

//static void populate_intrinsics( camera_intrinsics_t * intrinsics, double * fx, double * fy, double * cx, double * cy, double * k1, double * k2, double * p1, double * p2 )
//{
//    *fx = intrinsics->K[0][0];
//    *fy = intrinsics->K[1][1];
//    *cx = intrinsics->K[0][2];
//    *cy = intrinsics->K[1][2];
//    *k1 = intrinsics->D[0];
//    *k2 = intrinsics->D[1];
//    *p1 = intrinsics->D[2];
//    *p2 = intrinsics->D[3];
//}

void unfisheyePixel(double x, double y, camera_intrinsics_t * cam, int iter_num, double * xo, double * yo);

#ifdef __cplusplus
}
#endif

#endif /* fisheye_h */
