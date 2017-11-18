/***********************************************************************************************//**
 * \file   kinetic.c
 * \brief  Kinetic Motion code
 ***************************************************************************************************
 *  Created on: Nov 13, 2016
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "kinetic.h"

#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) Y==0?ZDIV_LNUM:X/Y

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/
/** Local change in time */
//static double         delta_t;

quaternion_t qd, qc, qr, qg, qa;
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
    Kalman_Init( &kinetics->rotationFilter[0], imu->data.roll );
    Kalman_Init( &kinetics->rotationFilter[1], imu->data.pitch );
    Kalman_Init( &kinetics->rotationFilter[2], imu->data.yaw   );
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
 *  \brief  Calculates system's absolute position and places value in truePosition[]

 \f{eqnarray*}{
    &a_a = rot_{f_0}, a_b = rot_{f_1}, a_c = rot_{f_2} \\

    &\mathbf{d} =
    \begin{cases}
        &d_{\hat{i}} = vis_{1_x} - vis_{0_x} \\
        &d_{\hat{j}} = vis_{1_y} - vis_{0_y} \\
        &d_{\hat{k}} = 0 \\
    \end{cases} \\
    &\mathbf{r} = \text{dAugment}(\mathbf{d}, \mathbf{a}) \\
    &\mathbf{e} =
    \begin{cases}
        &e_{\hat{i}} = V_{center_x} - vis_{0_x} \\
        &e_{\hat{j}} = V_{center_y} - vis_{0_y} \\
        &e_{\hat{k}} = 0 \\
    \end{cases} \\
    &\mathbf{e_{true}} = \text{zxyTranform}(\mathbf{e}, \mathbf{a}, 1) \\
    &\mathbf{p_{true}} = \mathbf{r} - \mathbf{e_{true}} \\
    &\mathbf{n} = \text{getNonGravAcceleration()} \\
    &\mathbf{v} =
    \begin{cases}
        &v_x = &n_{\hat{i}}\Delta{t} \\
        &v_y = &n_{\hat{j}}\Delta{t} \\
        &v_z = &n_{\hat{k}}\Delta{t} \\
    \end{cases} \\
   &\text{Update all position kalman_ts with } \mathbf{p_{true}}, \mathbf{v}, \text{ and } \Delta{t}
 \f}
 **************************************************************************************************/
void Kinetic_Update_Position( LSM9DS1_t * imu, kinetic_t * kinetics, cartesian2_t beacons[2] )
{
    /* Tait-Bryan (intrinsic) angles of imu */
    ang3_t tbd, tbr;
    tbd.x = kinetics->rotationFilter[0].value; // phi'
    tbd.y = kinetics->rotationFilter[1].value; // theta'
    tbd.z = kinetics->rotationFilter[2].value; // psi'
    Euler_To_Quaternion( &tbd, &qd );
    
    double the_1, the_2, the_r, phi_1, phi_2, phi_r, psi_r, rho_1, rho_r;
    double x_1, x_2, y_1, y_2, d_x, d_y, d__l, r_l;
    
    x_1 = beacons[0].x;
    x_2 = beacons[1].x;
    y_1 = beacons[0].y;
    y_2 = beacons[1].y;
    d_x = x_2 - x_1;
    d_y = y_2 - y_1;
    
    /* Get beacon angles */
    the_1   = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( x_1 / CAMERA_WIDTH  ) - 0.5 );
    phi_1   = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( y_1 / CAMERA_HEIGHT ) - 0.5 );
    the_2   = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( x_2 / CAMERA_WIDTH  ) - 0.5 );
    phi_2   = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( y_2 / CAMERA_HEIGHT ) - 0.5 );
    psi_r   = atan2( d_y, d_x );
    
    /* Get angles between beacons */
    the_r = fabs( the_2 - the_1 );
    phi_r = fabs( phi_2 - phi_1 );
    rho_1 = acos( cos( the_1 ) * cos( phi_1 ) );
    rho_r = acos( cos( the_r ) * cos( phi_r ) );
    
    /* Get distance between beacons */
    d__l = sqrt( ( d_x * d_x ) + ( d_y * d_y ) ) * PIXELS_TO_UNITS;
    
    double chi, mu, gam, gam_;
    double sin_rho_r = sin( rho_r );
    /* Calculate Chi */
    chi = asin( ZDIV( ( FOCAL_LENGTH * UNITS_TO_PIXELS ), d__l ) * ZDIV( sin_rho_r, cos( rho_1 ) ) );
    
    /* Generate Camera-Beacon quaternion */
    tbr.x = phi_1;
    tbr.y = the_1;
    tbr.z = psi_r;
    Euler_To_Quaternion( &tbr, &qr );
    
    /* Calculate mu */
    mu = ( qr.w * qr.w ) - ( qr.x * qr.x ) - ( qr.y * qr.y ) + ( qr.z * qr.z );
    
    /* Calculate gamma */
    gam  = mu + chi;
    gam_ = 180 * DEG_TO_RAD - gam;
    
    /* Calculate r vector length */
    r_l = ( sin( gam_ ) / sin_rho_r ) * d__l;
    
    /* r_vec - Vector length r on X-axis */
    vec3_t r, r_f;
    r.i = r_l;
    r.j = 0;
    r.k = 0;
    
    Quaternion_Combine( &qd, &qc, &qr, &qa );

    /* Calculate final r vector */
    Rotate_Vector_By_Quaternion( &r, &qa, &r_f );
    
    /* Kalman filter position */
    /* Get non-gravitational acceleration */
    vec3_t ngacc;
    IMU_Non_Grav_Get( imu, &qa, &ngacc );
    double delta_time = 0;
    kinetics->truePositionFilter[0].value = ngacc.i;
    kinetics->truePositionFilter[1].value = ngacc.j;
    kinetics->truePositionFilter[2].value = ngacc.k;

    /* Filter calculated r_vec with acceleration > velocity */
    float n;
    n = kinetics->truePositionFilter[0].value;
    delta_time = seconds_since( kinetics->truePositionFilter[0].timestamp );
    double x_vel = ngacc.i * delta_time;
    Kalman_Update( &kinetics->truePositionFilter[0], r_f.i, x_vel );

    n = kinetics->truePositionFilter[1].value;
    delta_time = seconds_since( kinetics->truePositionFilter[0].timestamp );
	double y_vel = ngacc.j * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[1], r_f.j, y_vel );

	n = kinetics->truePositionFilter[2].value;
    delta_time = seconds_since( kinetics->truePositionFilter[0].timestamp );
	double z_vel = ngacc.k * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[2], r_f.k, z_vel );
}
