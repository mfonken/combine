/***********************************************************************************************//**
 * \file   kinetic.c
 * \brief  Kinetic Motion code
 ***************************************************************************************************
 *  Created on: Nov 13, 2016
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "kinetic.h"

/***********************************************************************************************//**
 *  \brief  Initialize Kinetic Sensors
 **************************************************************************************************/
void KineticDefaultInit( kinetic_t * k )
{
    kinetic_config_t config = { KINETIC_CAMERA_WIDTH, KINETIC_CAMERA_HEIGHT, FOCAL_LENGTH, D_FIXED };
    KineticFunctions.Init( k, config );
}

void KineticInit( kinetic_t * k, kinetic_config_t config )
{
    k->W = config.width;
    k->H = config.height;
    k->f_l = config.focal_length;
    k->d_l = config.beacon_distance;
    Camera_Rotation_Init(k);
    Reference_Rotation_Init(k);
}

static void KineticUpdatePosition( kinetic_t * k, quaternion_t * O, kpoint_t * A, kpoint_t * B )
{
    /* Step 1: Calculate Minor Angles */
    KineticFunctions.MinorAngles( k, A, B );
    
    /* Step 2: Calculate Quaternion Rotations */
    KineticFunctions.Quaternions( k, O );
    
    /* Step 3: Calculate Major Angles */
    KineticFunctions.MajorAngles( k );
    
    /* Step 4: Calculate r vector */
    KineticFunctions.R( k );
    
    /* Step 5A: Update position values for tests */
    k->values.position[0] = k->r.i;
    k->values.position[1] = k->r.j;
    k->values.position[2] = k->r.k;
    
//    Kalman.update( &k->filters.position[0], k->r.i, 0, VELOCITY );
//    Kalman.update( &k->filters.position[1], k->r.j, 0, VELOCITY );
//    Kalman.update( &k->filters.position[2], k->r.k, 0, VELOCITY );
    
//    printf("Yaw:%4d | Nu:%4d | Up:%4d | Sig:%4d | Chi:%4d | Mu:%4d | Gamma:%4d |  | r_l: %.4f\n", (int)(k->e.z*RAD_TO_DEG), (int)(k->nu*RAD_TO_DEG), (int)(k->upsilon*RAD_TO_DEG), (int)(k->sigmaR*RAD_TO_DEG), (int)(k->chi*RAD_TO_DEG), (int)(k->mu*RAD_TO_DEG), (int)(k->gamma*RAD_TO_DEG), /* H_a: <%4d,%4d,%4d> (int)(a.x), (int)(a.y), (int)(a.z),*/ k->r_l);
//    return;
    
    /* Step 5B: Calculate Non-gravitational Data */
//    Kinetic.nongrav( k, n );
}

static void KineticMinorAngles( kinetic_t * k, kpoint_t * A, kpoint_t * B )
{
    KPoint.copy(A, &k->A);
    KPoint.copy(B, &k->B);
    /* Get distance between beacons and origin */
    KPoint.init( &k->A, k->W/2, k->H/2, PIXEL_TO_UNIT, k->f_l );
    KPoint.init( &k->B, k->W/2, k->H/2, PIXEL_TO_UNIT, k->f_l );
    kpoint_t d_ = { ( k->B.x - k->A.x ), ( k->B.y - k->A.y ) };
    k->d__l = KPoint.dist( &d_ );
    
    /* Get beacon tilt on camera plane */
    k->omega = KPoint.angl( &d_ );
    
    /* Get angles between beacons */
    k->sigmaA = KPoint.anga( &k->A );
    
    vec3_t Av, Bv;
    KPoint.toVec3( &k->A, &Av );
    KPoint.toVec3( &k->B, &Bv );
    k->sigmaR = Vector3.ang3( &Av, &Bv );
}

static void KineticQuaternions( kinetic_t * k, quaternion_t * O )
{
    /* Get proper device angles from kinetic */
    Quaternion.copy( O, &k->qd );
    
    /* Rotate beacon A around origin by roll(r.y) and calculate nu and upsilon as horizonatl and vertical angle offset */
    double cos_ry = cos(-k->e.y), sin_ry = sin(-k->e.y);
    k->nu      = atan2( ( sin_ry * k->A.x + cos_ry * k->A.y ), k->f_l );
    k->upsilon = atan2( ( sin_ry * k->A.y - cos_ry * k->A.x ), k->f_l );
    
    /* Generate Camera-Beacon quaternion */
    ang3_t a = { k->e.x + k->nu, k->e.y + k->omega, k->e.z + k->upsilon };
    Quaternion.fromEuler( &a, &k->qd_ );
    Quaternion.combine( &k->qd_, &k->qc, &k->qa );
    
    ang3_t b = { k->e.x, k->e.y + k->omega, k->e.z };
    Quaternion.fromEuler( &b, &k->qc_ );
    
/* OP1: Potential parallelization */
//    Quaternion.fromEuler( &k->e, &k->qd );
//    Quaternion.fromEuler( &a, &k->qd_ );
//    Quaternion.fromEuler( &b, &k->qc_ );
//    Quaternion.combine( &k->qd_, &k->qc, &k->qa );
}

static void KineticMajorAngles( kinetic_t * k )
{
    KineticFunctions.Chi( k );
    KineticFunctions.Mu(  k );
    KineticFunctions.Gamma( k );
}

/* Calculation of chi - Secondary angle of inner triangle
 * a - focal length
 * b - length of vec d'
 */
static int KineticChi( kinetic_t * k )
{
    double m, n;
    if( ( fabs( k->sigmaA ) != M_PI_2 ) && ( k->d__l > 0 ) )
    {
        m = k->f_l / cos(k->sigmaA);
        n = m / k->d__l;
        k->chi = asin( n * sin(k->sigmaR) );
        return 0;
    }
    else k->chi = M_PI_2;
    return 0;
}

/* Calculation of mu - Real-visualized d vector offset angle
 * a - quaternion x factor
 * b - quaternion z factor
 */
static int KineticMu( kinetic_t * k )
{
    double m, n, a = k->qc_.x, b = k->qc_.z;
    m = ( a * a ) + ( b * b );
    n = 1.0 - ( 2 * m );
    //OP2A: k->mu = -RASIN( n );
    if( fabs(n) < 1 )
    {
        k->mu = asin( n );
        return 1;
    }
    else k->mu = SIGN( n ) * M_PI_2;
    return 0;
}

/* Calculation of gammama */
static void KineticGamma( kinetic_t * k )
{
    //OP2B: k->gamma = k->chi - k->mu;
    k->gamma = k->chi + k->mu;
}

/* Calculation of r vec length
 * a - sin sigmaR
 * b - d vec real length
 */
static int KineticR_l( kinetic_t * k )
{
    double m;
    if( fabs(k->sigmaR) < 1 )
    {
        m = sin( k->gamma ) / sin( k->sigmaR );
        k->r_l = m * k->d_l;
        return 1;
    }
    else k->r_l = ZDIV_LNUM;
    return 0;
}

/* Calculation of r vec */
static void KineticR( kinetic_t * k )
{
    KineticFunctions.R_l(k);
    vec3_t r_u = { 0, k->r_l, 0 };
    Quaternion.rotVec( &r_u, &k->qa, &k->r );
}

kinetic_functions KineticFunctions =
{
    .DefaultInit = KineticDefaultInit,
    .Init = KineticInit,
    .UpdatePosition = KineticUpdatePosition,
    .MinorAngles = KineticMinorAngles,
    .Quaternions = KineticQuaternions,
    .MajorAngles = KineticMajorAngles,
    .Chi = KineticChi,
    .Mu = KineticMu,
    .Gamma = KineticGamma,
    .R_l = KineticR_l,
    .R = KineticR,
};

void Camera_Rotation_Init( kinetic_t * k )
{
    ang3_t c_a = { CAMERA_OFFSET_ANGLE_X, CAMERA_OFFSET_ANGLE_Y, CAMERA_OFFSET_ANGLE_Z };
    Quaternion.fromEuler( &c_a, &k->qc );
}

void Reference_Rotation_Init( kinetic_t * k )
{
    ang3_t r_a = { REFERENCE_OFFSET_ANGLE_X, REFERENCE_OFFSET_ANGLE_Y, REFERENCE_OFFSET_ANGLE_Z };
    Quaternion.fromEuler( &r_a, &k->qr );
}

