/***********************************************************************************************//**
 * \file   kinetic.c
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
//static double         delta_t;

quaternion_t qd, qd_, qs, qs_, qc, qc_, qr, qg, qa;

double  cos_precalc, sin_precalc;

/** Local positional and rotational vectors */

/***********************************************************************************************//**
 *  \brief  Initialize Kinetic Sensors
 **************************************************************************************************/
void Kinetic_Init( LSM9DS1_t * imu, kinetic_t * kinetics )
{
    Camera_Rotation_Init();
	Filters_Init( imu, kinetics );
}

/***********************************************************************************************//**
 *  \brief  Initialize Filters for Kinetic Data
 **************************************************************************************************/
void Filters_Init( LSM9DS1_t * imu, kinetic_t * kinetics )
{
	IMU_Update_All( imu );
    Kalman_Init( &kinetics->rotationFilter[0], imu->data.roll, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY);
    Kalman_Init( &kinetics->rotationFilter[1], imu->data.pitch, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY);
    Kalman_Init( &kinetics->rotationFilter[2], imu->data.yaw, MOTION_MAX_KALMAN_LIFE, MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY);
}

void Camera_Rotation_Init( void )
{
    ang3_t c_a;
    c_a.x = CAMERA_OFFSET_ANGLE_X;
    c_a.y = CAMERA_OFFSET_ANGLE_Y;
    c_a.z = CAMERA_OFFSET_ANGLE_Z;
    Euler_To_Quaternion( &c_a, &qc );
}

/***********************************************************************************************//**
 *  \brief  Update IMU data and filter
 **************************************************************************************************/
void Kinetic_Update_Rotation( LSM9DS1_t * imu, kinetic_t * kinetics )
{
    IMU_Update_All( imu );
    
    ang3_t ang;
    ang.x      = imu->data.roll;
    ang.y      = imu->data.pitch;
    ang.z      = imu->data.yaw;
    
    /* Restrict pitch */
    double v = kinetics->rotationFilter[0].value;
    if( ( ang.x < -HALF_PI && v >  HALF_PI ) ||
        ( ang.x >  HALF_PI && v < -HALF_PI ) )
    {
        kinetics->rotationFilter[0].value  = ang.x;
        kinetics->rotation[0]              = ang.x;
    }
    else
    {
        /* Calculate the true pitch using a kalman_t filter */
        Kalman_Update( &kinetics->rotationFilter[0], ang.x, imu->data.gyro[0] * DEG_TO_RAD );
        kinetics->rotation[0] = kinetics->rotationFilter[0].value;
    }
    
    if ( kinetics->rotation[0] > 90 )
    {
        /* Invert rate, so it fits the restricted accelerometer reading */
        imu->data.gyro[0] = -imu->data.gyro[0];
    }
    /* Calculate the true roll using a kalman_t filter */
    Kalman_Update( &kinetics->rotationFilter[1], ang.y, imu->data.gyro[1] * DEG_TO_RAD );
    kinetics->rotation[1] = kinetics->rotationFilter[1].value;
    
    /* Calculate the true yaw using a kalman_t filter */
    Kalman_Update( &kinetics->rotationFilter[2], ang.z, imu->data.gyro[2] * DEG_TO_RAD );
    kinetics->rotation[2] = kinetics->rotationFilter[2].value;
    
}

/***********************************************************************************************//**
 *  \brief  Calculates system's absolute position and places value in position[]
 **************************************************************************************************/
void Kinetic_Update_Position( LSM9DS1_t * imu, kinetic_t * kinetics, cartesian2_t beacons[2] )
{
    /*** LOCAL VARIABLES ***/
    mat3x3_t m;
    
    /* Local angles */
    ang3_t a, r;
    double the_B, the_A, phi_B, phi_A, psi_r, sigma_A, sigma_r, nu, up;
    
    /* Local lengths */
    double x_B, x_A, y_B, y_A, d_x, d_y, A_l_sq, d__l, n_l, u_l, r_l;
   
    /*** CALCULATING QUATERNIONS ***/
    x_B = beacons[0].x - CAMERA_HALF_WIDTH;
    x_A = beacons[1].x - CAMERA_HALF_WIDTH;
    y_B = beacons[0].y - CAMERA_HALF_HEIGHT;
    y_A = beacons[1].y - CAMERA_HALF_HEIGHT;
    d_x = x_A - x_B;
    d_y = y_A - y_B;
    
    /* Get pixel distance from (0,0) to beacon 1 */
    A_l_sq = ( ( x_A * x_A ) + ( y_A * y_A ) );
    
    /* Get unit distance between beacons */
    d__l = sqrt( ( d_x * d_x ) + ( d_y * d_y ) ) * PIXEL_TO_UNIT;
    
    
    /* Get beacon angles */
    the_B   = atan2( ( x_B * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    phi_B   = atan2( ( y_B * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    the_A   = atan2( ( x_A * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    phi_A   = atan2( ( y_A * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    psi_r   = atan2( d_y, d_x );
    
    /* Get angles between beacons */
    ///NOTE: acos(cos()cos()) only works for combining frame-perpendicular vectors (i.e. theta & phi) [ang3 is used instead for sigma_r]
    sigma_A = acos( cos( the_A ) * cos( phi_A ) );
    
    vec3_t A, B;
    A.i = x_B;
    A.j = y_B;
    A.k = FOCAL_LENGTH * UNIT_TO_PIXEL;
    B.i = x_A;
    B.j = y_A;
    B.k = FOCAL_LENGTH * UNIT_TO_PIXEL;
    sigma_r = ang3( &A, &B );
    
    n_l = ZDIV( fabs( ( phi_A * ( the_B - the_A ) ) - ( the_A * ( phi_B - phi_A ) ) ), d__l );
    u_l = sqrt( A_l_sq - ( n_l * n_l ) );
    nu = atan2( ( n_l * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    up = atan2( ( u_l * PIXEL_TO_UNIT ), FOCAL_LENGTH);
    
    /* Get proper device angles from kinetic */
    r.x = kinetics->rotation[0];
    r.y = kinetics->rotation[1];
    r.z = kinetics->rotation[2];
    
    /* Generate Camera-Beacon quaternion */
    a.x = r.x + nu;
    a.y = r.y + psi_r;
    a.z = r.z - up;
    Euler_To_Quaternion( &a, &qd );
    Quaternion_Combine( &qd, &qc, &qa );
    
    ang3Rad_To_Deg(&a);
    
    ///NOTE: Initializing another quaternion QD' cuts a quaternion combination
    a.x = r.x;
    /*  a.y should doens't change from the calculation of QD
     *  a.y = r.y + psi_r; */
    a.z = r.z;
    Euler_To_Quaternion( &a, &qd_ );
    ang3Rad_To_Deg(&a);
    
    /** CALCULATING R ***/
    double chi, mu, gam;
    double sin_sigma_r = sin( sigma_r );
    
    /* Calculate Chi */
    chi = RASIN( ZDIV( ZDIV( FOCAL_LENGTH, cos( sigma_A ) ), d__l ) * sin_sigma_r );
    
    /* Calculate mu */
    Quaternion_To_Matrix(&qd_, &m);
    mu = -asin( m.jj );
    
    /* Calculate gamma */
    gam  = chi - mu;
    
    /* Calculate r vector length */
    r_l = ZDIV( sin( gam ), sin_sigma_r ) * D_FIXED;
    
    /* r_vec - Vector length r on X-axis */
    vec3_t r_u, r_f;
    r_u.i = 0;
    r_u.j = r_l;
    r_u.k = 0;
    
    Quaternion_To_Euler(&qa, &a);
    a.z -= M_PI/2;
    Euler_To_Quaternion(&a, &qa);
    Quaternion_To_Euler(&qa, &a);
    ang3Rad_To_Deg(&a);
    
    /*** FILTERING R VECTOR ***/
    /* Calculate final r vector */
    Rotate_Vector_By_Quaternion( &r_u, &qa, &r_f );
    kinetics->positionFilter[0].value = r_f.i;
    kinetics->positionFilter[1].value = r_f.j;
    kinetics->positionFilter[2].value = r_f.k;
    
    kinetics->position[0] = r_f.i;
    kinetics->position[1] = r_f.j;
    kinetics->position[2] = r_f.k;
    
//    printf("Yaw:%4d | Nu:%4d | Up:%4d | Sig:%4d | Chi:%4d | Mu:%4d | Gamma:%4d | H_a: <%4d,%4d,%4d> | r_l: %.4f\n", (int)(r.z*RAD_TO_DEG), (int)(nu*RAD_TO_DEG), (int)(up*RAD_TO_DEG), (int)(sigma_r*RAD_TO_DEG), (int)(chi*RAD_TO_DEG), (int)(mu*RAD_TO_DEG), (int)(gam*RAD_TO_DEG), (int)(a.x), (int)(a.y), (int)(a.z), r_l);
    
    return;
    
    /* Kalman filter position */
    /* Get non-gravitational acceleration */
    vec3_t ngacc;
    IMU_Get_Non_Grav( imu, &ngacc );
    double delta_time = 0;
    kinetics->positionFilter[0].value = ngacc.i;
    kinetics->positionFilter[1].value = ngacc.j;
    kinetics->positionFilter[2].value = ngacc.k;

    /* Filter calculated r_vec with acceleration > velocity */
    float n;
    n = kinetics->positionFilter[0].value;
    delta_time = seconds_since( kinetics->positionFilter[0].timestamp );
    double x_vel = ngacc.i * delta_time;
    Kalman_Update( &kinetics->positionFilter[0], r_f.i, x_vel );

    n = kinetics->positionFilter[1].value;
    delta_time = seconds_since( kinetics->positionFilter[0].timestamp );
	double y_vel = ngacc.j * delta_time;
	Kalman_Update( &kinetics->positionFilter[1], r_f.j, y_vel );

	n = kinetics->positionFilter[2].value;
    delta_time = seconds_since( kinetics->positionFilter[0].timestamp );
	double z_vel = ngacc.k * delta_time;
	Kalman_Update( &kinetics->positionFilter[2], r_f.k, z_vel );
    
    kinetics->position[0] = kinetics->positionFilter[0].value;
    kinetics->position[1] = kinetics->positionFilter[1].value;
    kinetics->position[2] = kinetics->positionFilter[2].value;
}
