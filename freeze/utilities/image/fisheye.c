//
//  fisheye.c
//  ots-proto
//
//  Created by Matthew Fonken on 5/31/22.
//

#include "fisheye.h"

void unfisheyePixel(double x, double y, camera_intrinsics_t * cam, int iter_num, double * xo, double * yo)
{
//    populate_intrinsics( intrinsics, &fx, &fy, &cx, &cy, &k1, &k2, &k3, &k4 );
    double x0 = ( x - cam->K[0][2] ) / cam->K[0][0];
    double y0 = ( y - cam->K[1][2] ) / cam->K[1][1];
    double theta_d = sqrt( ( x0 * x0 ) + ( y0 * y0 ) );
    if( theta_d > M_PI_2 ) theta_d = M_PI_2;
    else if( theta_d < -M_PI_2 ) theta_d = -M_PI_2;

    bool converged = false;
    double theta = theta_d;
    double scale = 0.0;
    if ( fabs(theta_d) > FISHEYE_EPS)
    {
        for( int j = 0; j < iter_num; j++ )
        {
            double theta2 = theta * theta;
            double theta4 = theta2 * theta2;
            double theta6 = theta4 * theta2;
            double theta8 = theta4 * theta4;
            double k0_theta2 = cam->D[0] * theta2;
            double k1_theta4 = cam->D[1] * theta4;
            double k2_theta6 = cam->D[2] * theta6;
            double k3_theta8 = cam->D[3] * theta8;

            double n = theta * ( 1 + k0_theta2 + k1_theta4 + k2_theta6 + k3_theta8 ) - theta_d;
            double d = 1 + 3 * k0_theta2 + 5 * k1_theta4 * 7 * k2_theta6 + 9 * k3_theta8;
            double theta_fix = n / d;
            theta -= theta_fix;
            
            if( fabs(theta_fix) < FISHEYE_EPS )
            {
                converged = true;
                break;
            }
        }
        scale = tan(theta) / theta_d;
    }
    else converged = true;
    
    bool theta_flipped = (theta_d < 0 && theta > 0) || (theta_d > 0 && theta < 0);
    
    double u = -1000000.;
    double v = -1000000.;
    if( converged && !theta_flipped)
    {
        u = x0 * scale;
        v = y0 * scale;
    }
    *xo = u * cam->K[0][0] + cam->K[0][2];
    *yo = v * cam->K[1][1] + cam->K[1][2];
}


