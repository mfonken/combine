//
//  unfisheye.h
//  
//
//  Created by Matthew Fonken on 10/30/16.
//
//

#ifndef unfisheye_hpp
#define unfisheye_hpp

#include <stdint.h>
#include <math.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "global_types.h"

//#define PERFORMANCE_TEST
//#define QUICK
//#define ALT_QUICK

#define INR(X,A,B) (X>=A&&X<B)

using namespace cv;

#define LOV_constant 0.414213562
static inline double qdist(double ax, double ay) { return (ax > ay )? ax + ay*LOV_constant:ay + ax*LOV_constant; }
static inline double qatanr_r( double x ) { return 1.0301 - x*(0.0663*x - 0.1784); }

typedef struct fisheye fisheye;

struct fisheye
{
    int     width,
            height,
            half_width,
            half_height;
    double  radius,
            correction,
            zoom,
            strength,
            ztheta;
    void (*init)(int, int, double, double);
    void (*unfisheyePoint)(point2_t, point2_t);
};

extern fisheye Fisheye;

void quickUnfisheye(Mat I, Mat O);
void unfisheye(Mat I, Mat O);
    
#endif /* unfisheye_h */
