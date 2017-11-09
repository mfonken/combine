/***********************************************************************************************//**
 * \file   kinetic.c
 * \brief  Kinetic Motion code
 ***************************************************************************************************
 *  Created on: Nov 13, 2016
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "kinetic.h"

#define CAMERA_FOCAL_LENGTH 1

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/
quaternion_t qp, qc, qr, qa;
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
    
    double delta_time = 0;
    
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
    Kinetic_Update_Rotation( imu, kinetics );
    /* Tait-Bryan (intrinsic) angles of device */
    ang3_t p_a;
    p_a.y = kinetics->rotationFilter[0].value; // phi'
    p_a.x = kinetics->rotationFilter[1].value; // theta'
    p_a.z = kinetics->rotationFilter[2].value; // psi'
    Euler_To_Quaternion( &p_a, &qp );

    /* Calculate beacon angles */
    ang3_t b_r, b_diff;
    double b_l_;
    double sigma_1, sigma_r, mu, chi, gamma, gamma_;
    
    /* Get first beacon angles */
    b_r.x = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( beacons[0].x / CAMERA_HEIGHT ) - 0.5 );
    b_r.y = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( beacons[0].y / CAMERA_WIDTH  ) - 0.5 );
    
    /* Get angles between beacons */
    b_diff.x   = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( beacons[1].x / CAMERA_HEIGHT ) - 0.5 ) - b_r.x;
    b_diff.y   = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( beacons[1].y / CAMERA_WIDTH  ) - 0.5 ) - b_r.y;
    b_r.z = atan2( b_diff.y, b_diff.x );
    Euler_To_Quaternion( &b_r, &qr );
    
    b_l_ = sqrt( ( b_diff.x * b_diff.x ) + ( b_diff.y * b_diff.y ) );
    
    sigma_1 = acos( cos( b_r.x ) * cos( b_r.y ) );
    sigma_r = acos( cos( b_diff.x ) * cos( b_diff.y ) );

    /* Combine quaternions */
    Quaternion_Combine( &qp, &qc, &qr, &qa );
    
    chi = asin( ( CAMERA_FOCAL_LENGTH / b_l_ ) * ( sin( sigma_r ) / sin( sigma_1 ) ) );
    /* NOTE: This uses the non-homogenized orthogonal rotation matrix */
    mu = ( qa.w * qa.w ) - ( qa.x * qa.x ) - ( qa.y * qa.y ) + ( qa.z * qa.z );//acos( 1 - 2 * ( qa.y * qa.y + qa.z * qa.z ) );
    
    gamma = mu + chi;
    gamma_ = PI - gamma;
    
    /* r_l - Distance to beacons */
    double r_l = ( sin( gamma_ ) / sin( sigma_r ) ) * D_FIXED;
    
    vec3_t r_t, r_f;
    r_t.i = r_l;
    r_t.j = 0;
    r_t.k = 0;
    Rotate_Vector_By_Quaternion( &r_t, &qa, &r_f );

    kinetics->truePosition[0] = r_f.i;
    kinetics->truePosition[1] = r_f.j;
    kinetics->truePosition[2] = r_f.k;
    /*
    / * Get non-gravitational acceleration * /
    vec3_t ngacc;
    IMU_Non_Grav_Get( imu, &qp, &ngacc );
    kinetics->truePositionFilter[0].value = ngacc.i;
    kinetics->truePositionFilter[1].value = ngacc.j;
    kinetics->truePositionFilter[2].value = ngacc.k;
    
    / * Filter calculated r_vec with acceleration > velocity * /
    double x_vel = ngacc.i * delta_time;
    Kalman_Update( &kinetics->truePositionFilter[0], r_f.i, x_vel );

	double y_vel = ngacc.j * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[1], r_f.j, y_vel );

	double z_vel = ngacc.k * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[2], r_f.k, z_vel );
    */
}
