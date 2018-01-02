//
//  imu_math.h
//  IMU Math
//
//  Created by Matthew Fonken on 12/18/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef imu_math_h
#define imu_math_h

#include "math_types.h"
#include "matrix.h"
#include "vector.h"
#include "quaternion.h"

#define RAD_TO_DEG (180 / M_PI)
#define DEG_TO_RAD (M_PI / 180)

#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) Y==0?ZDIV_LNUM:X/Y
#define SIGN(X)   X>=0?1:-1
#define RASIN(X)  X>-1&&X<1?asin(X):SIGN(X)*M_PI/2

#endif /* imu_math_h */
