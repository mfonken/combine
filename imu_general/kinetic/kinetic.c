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
quaternion_t qp, qc, qb, qa;
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
    /* Tait-Bryan (intrinsic) angles of device */
    ang3_t p_a;
    p_a.y = kinetics->rotationFilter[0].value; // phi'
    p_a.x = kinetics->rotationFilter[1].value; // theta'
    p_a.z = kinetics->rotationFilter[2].value; // psi'
    
    Print_String("angles - (");
	Print_Double_Ascii( p_a.x );
	Print_Char(',');
	Print_Double_Ascii( p_a.y );
	Print_Char(',');
	Print_Double_Ascii( p_a.z );
	Print_Line(")");

//	Print_String("beacons - (");
//	Print_Double_Ascii( beacons[0].x );
//	Print_Char(',');
//	Print_Double_Ascii( beacons[0].y );
//	Print_String(") (");
//	Print_Double_Ascii( beacons[1].x );
//	Print_Char(',');
//	Print_Double_Ascii( beacons[1].y );
//	Print_Line(")");

    /* Calculate beacon angles */
    ang3_t b_a;
    double b[2];
    /* Get first beacon angles */
    b_a.x = 0;
    b_a.y = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( beacons[0].y / CAMERA_HEIGHT ) - 0.5 );
    b_a.z = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( beacons[0].x / CAMERA_WIDTH  ) - 0.5 );
    
    ///TODO: Add phi (b_a.x) correction!
    
    /* Get angles between beacons */
    b[0]   = CAMERA_ALPHA_H * DEG_TO_RAD * ( ( beacons[1].y / CAMERA_HEIGHT ) - 0.5 ) - b_a.y;
    b[1]   = CAMERA_ALPHA_W * DEG_TO_RAD * ( ( beacons[1].x / CAMERA_WIDTH  ) - 0.5 ) - b_a.z;

//    Print_String("beacons - (");
//	Print_Double_Ascii( b_a[0] );
//	Print_Char(',');
//	Print_Double_Ascii( b_a[1] );
//	Print_Char(',');
//	Print_Double_Ascii( b_a[2] );
//	Print_Line(")");

    /* Create quaternions (qc is precalculated in init) */
    Euler_To_Quaternion( &p_a, &qp );
    Euler_To_Quaternion( &b_a, &qb );
    //Quaternion_Combine( &qp, &qc, &qb, &qa );
    
    /* Mu - Angle between d' to X-axis of reference ( mu = acos(X.x) ) */
    /* NOTE: This uses the homogenized orthogonal rotation matrix */
    double mu = acos( 1 - 2 * ( qa.y * qa.y + qa.z * qa.z ) );
    
    /* Sigma - Angle between beacons */
    double alpha = acos( cos( b[0] ) * cos( b[1] ) );
    
    /* r_l - Distance to beacons */
    double sigma = cos( alpha - mu ) / sin( alpha ) * D_FIXED;
    
    /* r_vec - Vector length r on X-axis */
    vec3_t r, r_f;
    r.i = sigma;
    r.j = 0;
    r.k = 0;
    mat3x3_t m;
    Quaternion_To_Matrix( &qp, &m );
    Multiply_Vec_3x1( &m, &r, &r_f );
    
    Print_Char('p');
    Print_Char(',');
	Print_Double_Ascii( r_f.i );
	Print_Char(',');
	Print_Double_Ascii( r_f.j );
	Print_Char(',');
	Print_Double_Ascii( r_f.k );
	Print_Line("");

    /* Get non-gravitational acceleration */
    vec3_t ngacc;
    IMU_Non_Grav_Get( imu, &qp, &ngacc );
    kinetics->truePositionFilter[0].value = ngacc.i;
    kinetics->truePositionFilter[1].value = ngacc.j;
    kinetics->truePositionFilter[2].value = ngacc.k;
    return;
    /* Filter calculated r_vec with acceleration > velocity */
/*
    float n;
    n = kinetics->truePositionFilter[0].value;
    double x_vel = ngacc.i * delta_time;
    Kalman_Update( &kinetics->truePositionFilter[0], r_f.i, x_vel );

    n = kinetics->truePositionFilter[1].value;
	double y_vel = ngacc.j * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[1], r_f.j, y_vel );

	n = kinetics->truePositionFilter[2].value;
	double z_vel = ngacc.k * delta_time;
	Kalman_Update( &kinetics->truePositionFilter[2], r_f.k, z_vel );
*/
}
