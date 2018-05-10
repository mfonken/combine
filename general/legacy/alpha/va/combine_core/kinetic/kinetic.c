/***********************************************************************************************//**
 * \file   k->c
 * \brief  Kinetic Motion code
 ***************************************************************************************************
 *  Created on: Nov 13, 2016
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "kinetic.h"

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/
/** Local change in time */
/** Local positional and rotational vectors */

/***********************************************************************************************//**
 *  \brief  Initialize Kinetic Sensors
 **************************************************************************************************/
static int init( kinetic_t * k )
{
    Camera_Rotation_Init(k);
    Filters_Init(k);
    return 1;
}

static void updateRotation( kinetic_t * k, ang3_t * e, ang3_t * g )
{
    /* Restrict pitch */
    double v = k->filters.rotation[0].value;
    if( ( e->x < -M_PI_2 && v >  M_PI_2 ) ||
       ( e->x >  M_PI_2 && v < -M_PI_2 ) )
    {
        k->filters.rotation[0].value  = e->x;
        k->values.rotation[0] = e->x;
    }
    else
    {
        /* Calculate the true pitch using a kalman_t filter */
        Kalman.update( &k->filters.rotation[0], e->x, g->x, VELOCITY );
        k->values.rotation[0] = k->filters.rotation[0].value;
    }
    
    if ( k->values.rotation[0] > 90 )
    {
        /* Invert rate, so it fits the restricted accelerometer reading */
        g->x = -g->x;
    }
    /* Calculate the true roll using a kalman_t filter */
    Kalman.update( &k->filters.rotation[1], e->y, g->y, VELOCITY );
    k->values.rotation[1] = k->filters.rotation[1].value;
    
    /* Calculate the true yaw using a kalman_t filter */
    Kalman.update( &k->filters.rotation[2], e->z, g->z, VELOCITY );
    k->values.rotation[2] = k->filters.rotation[2].value;
}

static void updatePosition( kinetic_t * k, double ngacc[3], double beaconA[2], double beaconB[2] )
{
    /*** LOCAL VARIABLES ***/
    mat3x3_t m;
    
    /* Local angles */
    ang3_t a, r;
    double the_B, the_A, phi_B, phi_A, psi_r, sigma_A, sigma_r, nu, up;
    
    /* Local lengths */
    double x_B, x_A, y_B, y_A, d_x, d_y, A_l_sq, d__l, n_l, u_l, r_l;
    
    /*** CALCULATING QUATERNIONS ***/
    x_B = ( beaconB[0] - CAMERA_HALF_WIDTH  ) * PIXEL_TO_UNIT;
    x_A = ( beaconA[0] - CAMERA_HALF_WIDTH  ) * PIXEL_TO_UNIT;
    y_B = ( beaconB[1] - CAMERA_HALF_HEIGHT ) * PIXEL_TO_UNIT;
    y_A = ( beaconA[1] - CAMERA_HALF_HEIGHT ) * PIXEL_TO_UNIT;
    d_x = x_A - x_B;
    d_y = y_A - y_B;
    
    /* Get unit distance squared from camera center to beacon 1 */
    A_l_sq = ( ( x_A * x_A ) + ( y_A * y_A ) );
    
    /* Get unit distance between beacons */
    d__l = sqrt( ( d_x * d_x ) + ( d_y * d_y ) );
    
    /* Get beacon angles */
    the_B   = atan2( x_B, FOCAL_LENGTH);
    phi_B   = atan2( y_B, FOCAL_LENGTH);
    the_A   = atan2( x_A, FOCAL_LENGTH);
    phi_A   = atan2( y_A, FOCAL_LENGTH);
    psi_r   = atan2( d_y, d_x );
    
    /* Get angles between beacons */
    ///NOTE: acos(cos()cos()) only works for combining frame-perpendicular angles (i.e. theta & phi) [ang3 is used instead for sigma_r]
    sigma_A = acos( cos( the_A ) * cos( phi_A ) );
    
    vec3_t A, B;
    A.i = x_B;
    A.j = y_B;
    A.k = FOCAL_LENGTH;
    B.i = x_A;
    B.j = y_A;
    B.k = FOCAL_LENGTH;
    sigma_r = Vector.ang3( &A, &B );
    
    double temp = fabs( ( phi_A * ( the_B - the_A ) ) - ( the_A * ( phi_B - phi_A ) ) ), f_len = FOCAL_LENGTH;
//    n_l = ZDIV( fabs( ( phi_A * ( the_B - the_A ) ) - ( the_A * ( phi_B - phi_A ) ) ), d__l );
//    u_l = sqrt( A_l_sq - ( n_l * n_l ) );
    
    /* Get proper device angles from kinetic */
    r.x = k->values.rotation[0];
    r.y = k->values.rotation[1];
    r.z = k->values.rotation[2];
    
    /* Rotate beacon A around origin by roll(r.y) and calculate nu and upsilon as horizonatl and vertical angle offset */
    double cos_ry = cos(r.y), sin_ry = sin(r.y);
    n_l = cos_ry * (x_A-CAMERA_HALF_WIDTH) - sin_ry * (y_A-CAMERA_HALF_HEIGHT) + CAMERA_HALF_WIDTH;
    u_l = sin_ry * (x_A-CAMERA_HALF_WIDTH) + cos_ry * (y_A-CAMERA_HALF_HEIGHT) + CAMERA_HALF_HEIGHT;
    nu = atan2( n_l, FOCAL_LENGTH );
    up = atan2( u_l, FOCAL_LENGTH );
    
    /* Generate Camera-Beacon quaternion */
    a.x = r.x + nu;
    a.y = r.y + psi_r;
    a.z = r.z - up;
    Quaternion.fromEuler( &a, &k->qd );
    Quaternion.combine( &k->qd, &k->qc, &k->qa );
    
    ang3Rad_To_Deg(&a);
    
    ///NOTE: Initializing another quaternion QD' cuts a quaternion combination
    a.x = r.x;
    /*  a.y should doens't change from the calculation of QD
     *  a.y = r.y + psi_r; */
    a.z = r.z;
    Quaternion.fromEuler( &a, &k->qd_ );
    ang3Rad_To_Deg( &a );
    
    /** CALCULATING R ***/
    double chi, mu, gam;
    double sin_sigma_r = sin( sigma_r );
    
    /* Calculate Chi */
    chi = RASIN( ZDIV( ZDIV( FOCAL_LENGTH, cos( sigma_A ) ), d__l ) * sin_sigma_r );
    
    /* Calculate mu */
//    Quaternion.toMatrix(&k->qd_, &m);
//    mu = -asin( m.jj );
    double x = k->qd_.x, z = k->qd_.z;
    mu = -asin( 1.0 - 2 * ( ( x * x ) + ( z * z ) ) );
    
    /* Calculate gamma */
    gam  = chi - mu;
    
    /* Calculate r vector length */
    r_l = ZDIV( sin( gam ), sin_sigma_r ) * D_FIXED;
    
    /* r_vec - Vector length r on X-axis */
    vec3_t r_u, r_f;
    r_u.i = 0;
    r_u.j = r_l;
    r_u.k = 0;
    
    Quaternion.toEuler(&k->qa, &a);
    a.z -= M_PI/2;
    Quaternion.fromEuler(&a, &k->qa);
    ang3Rad_To_Deg(&a);
    
    /*** FILTERING R VECTOR ***/
    /* Calculate final r vector */
    Quaternion.rotVec( &r_u, &k->qa, &r_f );
    
    k->values.position[1] = -r_f.i;
    k->values.position[0] = r_f.j;
    k->values.position[2] = r_f.k;
    
    printf("Yaw:%4d | Nu:%4d | Up:%4d | Sig:%4d | Chi:%4d | Mu:%4d | Gamma:%4d | H_a: <%4d,%4d,%4d> | r_l: %.4f\n", (int)(r.z*RAD_TO_DEG), (int)(nu*RAD_TO_DEG), (int)(up*RAD_TO_DEG), (int)(sigma_r*RAD_TO_DEG), (int)(chi*RAD_TO_DEG), (int)(mu*RAD_TO_DEG), (int)(gam*RAD_TO_DEG), (int)(a.x), (int)(a.y), (int)(a.z), r_l);
    
    Kalman.update( &k->filters.position[1], -r_f.i, 0, VELOCITY );
    Kalman.update( &k->filters.position[0], r_f.j, 0, VELOCITY );
    Kalman.update( &k->filters.position[2], r_f.k, 0, VELOCITY );
    return;
    
    /* Get non-gravitational acceleration */
    /* Create a vector of accelerometer values */
    ang3_t e;
    e.y = k->values.rotation[0];
    e.x = k->values.rotation[1];
    e.z = 0;//imu->data.yaw - M_PI_2;
    
    Quaternion.fromEuler(&e, &k->qg);
    e.x *= RAD_TO_DEG;
    e.y *= RAD_TO_DEG;
    e.z *= RAD_TO_DEG;
    
    /* Rotate acceleration vector by quaternion */
    vec3_t g = {0,0,1},h;
    Quaternion.rotVec(&g, &k->qg, &h);
    
    /* Negate gravity: -(-1g) = +1g */
    ngacc[0] += h.i;
    ngacc[1] += h.j;
    ngacc[2] -= h.k;
    
    
    //#define NON_GRAV_MIN 0.0075
    //    if(fabs(ngacc->i) < NON_GRAV_MIN) ngacc->i = 0;
    //    if(fabs(ngacc->j) < NON_GRAV_MIN) ngacc->j = 0;
    //    if(fabs(ngacc->k) < NON_GRAV_MIN) ngacc->k = 0;
    
    /* Filter calculated r_vec with acceleration > velocity */
    Kalman.update( &k->filters.position[0], r_f.i, ngacc[0], ACCELERATION );
    Kalman.update( &k->filters.position[1], r_f.j, ngacc[1], ACCELERATION );
    Kalman.update( &k->filters.position[2], r_f.k, ngacc[2], ACCELERATION );
    
    k->values.position[0] = k->filters.position[0].value;
    k->values.position[1] = k->filters.position[1].value;
    k->values.position[2] = k->filters.position[2].value;
}

const kinetic Kinetic =
{
    .init = init,
    .updateRotation = updateRotation,
    .updatePosition = updatePosition
};

/***********************************************************************************************//**
 *  \brief  Initialize Filters for Kinetic Data
 **************************************************************************************************/
void Filters_Init( kinetic_t * k )
{
    Kalman.init(&k->filters.rotation[0], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
    Kalman.init(&k->filters.rotation[1], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
    Kalman.init(&k->filters.rotation[2], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
    Kalman.init(&k->filters.position[0], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
    Kalman.init(&k->filters.position[1], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
    Kalman.init(&k->filters.position[2], 0.0, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY );
}

void Camera_Rotation_Init( kinetic_t * k )
{
    ang3_t c_a;
    c_a.x = CAMERA_OFFSET_ANGLE_X;
    c_a.y = CAMERA_OFFSET_ANGLE_Y;
    c_a.z = CAMERA_OFFSET_ANGLE_Z;
    Quaternion.fromEuler( &c_a, &k->qc );
}

