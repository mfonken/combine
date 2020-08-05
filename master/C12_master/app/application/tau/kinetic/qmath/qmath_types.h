//
//  qmath_types.h
//  combine
//
//  Created by Matthew Fonken on 10/19/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef qmath_types_h
#define qmath_types_h

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "global_types.h"

#ifndef M_PI
#define M_PI 	3.14159265358979323846
#define M_PI_2 	1.57079632679489661923
#endif

/*! Simple 3d Vector */
typedef struct _vec3_t
{
    floating_t    i;             /**< \$f\hat{i}\$f value */
    floating_t    j;             /**< \$f\hat{j}\$f value */
    floating_t    k;             /**< \$f\hat{k}\$f value */
} vec3_t;

//typedef struct _vec2_t
//{
//    floating_t    i;             /**< \$f\hat{i}\$f value */
//    floating_t    j;             /**< \$f\hat{j}\$f value */
////    floating_t    k;             /**< \$f\hat{k}\$f value */
//} vec2_t;

///*! Simple 3d Vector */
//typedef struct _mat3x3_t
//{
//    floating_t    ii;               /**< \$f\hat{ii}\$f value */
//    floating_t    ji;               /**< \$f\hat{ji}\$f value */
//    floating_t    ki;               /**< \$f\hat{ki}\$f value */
//    floating_t    ij;               /**< \$f\hat{ij}\$f value */
//    floating_t    jj;               /**< \$f\hat{jj}\$f value */
//    floating_t    kj;               /**< \$f\hat{kj}\$f value */
//    floating_t    ik;               /**< \$f\hat{ik}\$f value */
//    floating_t    jk;               /**< \$f\hat{jk}\$f value */
//    floating_t    kk;               /**< \$f\hat{kk}\$f value */
//} mat3x3_t;

#endif /* qmath_types_h */
