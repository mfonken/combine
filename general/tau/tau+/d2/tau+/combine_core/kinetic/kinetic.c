/***************************************************************************************************
 * \file   kinetic.c
 * \brief  Kinetic Motion code
 ***************************************************************************************************
 *  Created on: Nov 13, 2016
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "kinetic.h"

/**************************************************************************************************
 *  Initialize Kinetic Sensors
 **************************************************************************************************/
static int init( kinetic_t * k, int W, int H, double F, double L )
{
    k->W = W;
    k->H = H;
    k->f_l = F;
    k->d_l = L;
    Camera_Rotation_Init(k);
    Reference_Rotation_Init(k);
    Filters_Init(k);
    
    return 1;
}

/**************************************************************************************************
 * Update Kinetic Rotation
 **************************************************************************************************/
static void updateRotation( kinetic_t * k, ang3_t * e, ang3_t * g )
{
    if( g == NULL )
    {
        k->values.rotation[0] = e->x;// - k->offset.x;
        k->values.rotation[1] = e->y;// - k->offset.y;
        k->values.rotation[2] = e->z;// - k->offset.z;
    }
    else
    {
        /* Step 1: Update Pitch (Restricted) */
        double v = k->filters.rotation[0].value;
        if( ( e->x < -M_PI_2 && v >  M_PI_2 ) ||
           ( e->x >  M_PI_2 && v < -M_PI_2 ) )
        {
            k->filters.rotation[0].value = e->x;
            k->values.rotation[0] = e->x;
        }
        else
        {
            Kalman.update( &k->filters.rotation[0], e->x, g->x, VELOCITY );
            k->values.rotation[0] = k->filters.rotation[0].value;// - k->offset.x;
        }
        
        if ( k->values.rotation[0] > M_PI_2 )
        {
            /* Invert rate, so it fits the restricted accelerometer reading */
            g->x = -g->x;
        }
        
        /* Step 2: Update Roll */
        Kalman.update( &k->filters.rotation[1], e->y, g->y, VELOCITY );
        k->values.rotation[1] = k->filters.rotation[1].value;// - k->offset.y;
        
        /* Step 3: Update Yaw */
        Kalman.update( &k->filters.rotation[2], e->z, g->z, VELOCITY );
        k->values.rotation[2] = k->filters.rotation[2].value;// - k->offset.z;
    }
}

/**************************************************************************************************
 * Update Kinetic Position
 **************************************************************************************************/
static void updatePosition( kinetic_t * k, vec3_t * n, kpoint_t * A, kpoint_t * B )
{
    /* Step 1: Calculate Minor Angles */
    Kinetic.minorAngles( k, A, B );
    
    /* Step 2: Calculate Quaternion Rotations */
    Kinetic.quaternions( k );
    
    /* Step 3: Calculate Major Angles */
    Kinetic.majorAngles( k );
    
    
    /* Step 4: Calculate r vector */
    Kinetic.r( k );
    
    /* Step 5A: Update position values for tests */
    k->values.position[0] =  k->r.i;
    k->values.position[1] =  k->r.j;
    k->values.position[2] =  k->r.k;
    
    Kalman.update( &k->filters.position[0],  k->r.i, 0, VELOCITY );
    Kalman.update( &k->filters.position[1],  k->r.j, 0, VELOCITY );
    Kalman.update( &k->filters.position[2],  k->r.k, 0, VELOCITY );
    
//    printf("Yaw:%4d | Ome:%4d | Nu:%4d | Up:%4d | Sig:%4d | Chi:%4d | Mu:%4d | Gamma:%4d |  | r_l: %.4f\n", (int)(k->e.z*RAD_TO_DEG), (int)(k->omega*RAD_TO_DEG), (int)(k->nu*RAD_TO_DEG), (int)(k->upsilon*RAD_TO_DEG), (int)(k->sigmaR*RAD_TO_DEG), (int)(k->chi*RAD_TO_DEG), (int)(k->mu*RAD_TO_DEG), (int)(k->gam*RAD_TO_DEG), /* H_a: <%4d,%4d,%4d> (int)(a.x), (int)(a.y), (int)(a.z),*/ k->r_l);
    //    return;
    
    /* Step 5B: Calculate Non-gravitational Data */
    if( n != NULL )  Kinetic.nongrav( k, n );
    
//    ang3_t o;
//    Quaternion.toEuler( &k->qa, &o );
//    printf("X:%4d | Y:%4d | Z:%4d | nu:%4d | up:%4d\n", (int)(o.x*RAD_TO_DEG), (int)(o.y*RAD_TO_DEG), (int)(o.z*RAD_TO_DEG), (int)(k->nu*RAD_TO_DEG), (int)(k->upsilon*RAD_TO_DEG));
    
//    ang3_t ac_;
//    Quaternion.toEuler( &k->qc_, &ac_ );
//    printf("C'>B(%4d, %4d, %4d) | mu: %4d\n", (int)(k->qc_.x*RAD_TO_DEG), (int)(k->qc_.y*RAD_TO_DEG), (int)(k->qc_.z*RAD_TO_DEG), (int)(k->mu*RAD_TO_DEG));
}

static void minorAngles( kinetic_t * k, kpoint_t * A, kpoint_t * B )
{
    KPoint.copy(A, &k->A);
    KPoint.copy(B, &k->B);
    
    /* Get distance between beacons and origin */
    KPoint.init( &k->A, k->W/2, k->H/2, PIXEL_TO_UNIT, k->f_l );
    KPoint.init( &k->B, k->W/2, k->H/2, PIXEL_TO_UNIT, k->f_l );
    kpoint_t d_ = { ( k->A.x - k->B.x ), ( k->A.y - k->B.y ) };
    k->d__l = KPoint.dist( &d_ );
    
    /* Get beacon tilt on camera plane */
    k->omega = KPoint.angl( &d_ );
    
    /* Get angles between beacons */
    vec3_t Av, Bv;
    KPoint.toVec3( &k->A, &Av );
    KPoint.toVec3( &k->B, &Bv );
    k->sigmaR = Vector.ang3( &Av, &Bv );
    k->sigmaA = KPoint.anga( &k->A );
}

static void quaternions( kinetic_t * k )
{
    /* Get proper device angles from kinetic */
    k->e.x = -k->values.rotation[0];
    k->e.y = -k->values.rotation[1];
    k->e.z = -k->values.rotation[2];
    Quaternion.fromEuler( &k->e, &k->qd );
    
    /* Rotate beacon A around origin by angle of d' vector (omega) */
    KPoint.rot( &k->A, -k->omega );
    
    /* Calculate nu and upsilon as horizontal and vertical angle offsets */
    k->upsilon = atan2( k->A.x, k->f_l );
    k->nu      = atan2( k->A.y, k->f_l );
    
    /* Generate Camera-Beacon quaternion */
    ang3_t a = {k->nu, k->omega, k->upsilon};
    Quaternion.fromEuler( &a, &k->qc_ );

    /* Combine quaternions */
    Quaternion.combine( &k->qr, &k->qd, &k->qr_ );
    Quaternion.combine( &k->qr_, &k->qc, &k->qd_ );
    Quaternion.combine( &k->qd_, &k->qc_, &k->qa );
    
    /* Debug quaternions into angles */
    ang3_t ad, ar, ad_, aa;
    Quaternion.toEuler( &k->qr, &ar );
    Quaternion.toEuler( &k->qd, &ad );
    Quaternion.toEuler( &k->qc_, &ad_ );
    Quaternion.toEuler( &k->qa, &aa );
    printf("B>R(%4d, %4d, %4d) | R>D(%4d, %4d, %4d) | D>C'(%4d, %4d, %4d) | B>C'(%4d, %4d, %4d)\n", (int)(ar.x*RAD_TO_DEG), (int)(ar.y*RAD_TO_DEG), (int)(ar.z*RAD_TO_DEG), (int)(ad.x*RAD_TO_DEG), (int)(ad.y*RAD_TO_DEG), (int)(ad.z*RAD_TO_DEG), (int)(ad_.x*RAD_TO_DEG), (int)(ad_.y*RAD_TO_DEG), (int)(ad_.z*RAD_TO_DEG), (int)(aa.x*RAD_TO_DEG), (int)(aa.y*RAD_TO_DEG), (int)(aa.z*RAD_TO_DEG));
    
    
//    ang3_t b = { k->e.x, k->e.y + k->omega, k->e.z };
//    Quaternion.fromEuler( &b, &k->qc_ );
//    k->qc_ = qt2;
    /* OP1: Potential parallelization */
    //    Quaternion.fromEuler( &k->e, &k->qd );
    //    Quaternion.fromEuler( &a, &k->qd_ );
    //    Quaternion.fromEuler( &b, &k->qc_ );
    //    Quaternion.combine( &k->qd_, &k->qc, &k->qa );
}

static void majorAngles( kinetic_t * k )
{
    Kinetic.chi( k );
    Kinetic.mu(  k );
    Kinetic.gam( k );
}

/* Calculation of chi - Secondary angle of inner triangle
 * a - focal length
 * b - length of vec d' */
static int chi( kinetic_t * k )
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
 * b - quaternion z factor */
static int mu( kinetic_t * k )
{
    double m, n, a = k->qc_.x, b = k->qc_.z;
    m = ( a * a ) + ( b * b );
    n = 1.0 - ( 2 * m );
    //OP2A: k->mu = -asin( n );
    if( fabs(n) < 1 )
    {
        k->mu = asin( n );
        return 1;
    }
    else k->mu = SIGN( n ) * M_PI_2;
    return 0;
}

/* Calculation of gamma */
static void gam( kinetic_t * k )
{
    //OP2B: k->gam = k->chi - k->mu;
    k->gam = k->chi + k->mu;
}

/* Calculation of r vec length
 * a - sin sigmaR
 * b - d vec real length */
static int r_l( kinetic_t * k )
{
    double m;
    if( fabs(k->sigmaR) < 1 )
    {
        m = sin( k->gam ) / sin( k->sigmaR );
        k->r_l = m * k->d_l;
        return 1;
    }
    //    else k->r_l = ZDIV_LNUM;
    return 0;
}

/* Calculation of r vec */
static void r( kinetic_t * k )
{
    Kinetic.r_l(k);
    vec3_t r_u = { 0, k->r_l, 0 };
    Quaternion.rotVec( &r_u, &k->qa, &k->r );
}

/* Calculation and filtering of nongraviation data */
static void nongrav( kinetic_t * k, vec3_t * n )
{
    Quaternion.rotVec( n, &k->qd, &k->n );
    
    Kalman.update( &k->filters.position[0], k->r.i, k->n.i, ACCELERATION );
    Kalman.update( &k->filters.position[1], k->r.j, k->n.j, ACCELERATION );
    Kalman.update( &k->filters.position[2], k->r.k, k->n.k, ACCELERATION );
    
    k->values.position[0] = k->filters.position[0].value;
    k->values.position[1] = k->filters.position[1].value;
    k->values.position[2] = k->filters.position[2].value;
}

/* Update beacon offset from geo-North */
static void updateReference( kinetic_t * k, ang3_t * r_a )
{
    Quaternion.fromEuler( r_a, &k->qr );
    k->offset = *r_a;
}

const kinetic Kinetic =
{
    .init = init,
    .updateRotation = updateRotation,
    .updatePosition = updatePosition,
    .minorAngles = minorAngles,
    .quaternions = quaternions,
    .majorAngles = majorAngles,
    .chi = chi,
    .mu = mu,
    .gam = gam,
    .r_l = r_l,
    .r = r,
    .nongrav = nongrav,
    .updateReference = updateReference
};

/**************************************************************************************************
 * Initialize Filters & Static Quaternions for Kinetic Data
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

/* Camera offset from imu (should be fixed) */
void Camera_Rotation_Init( kinetic_t * k )
{
    ang3_t c_a = { CAMERA_OFFSET_ANGLE_X, CAMERA_OFFSET_ANGLE_Y, CAMERA_OFFSET_ANGLE_Z };
    Quaternion.fromEuler( &c_a, &k->qc );
}

/* Beacon offset from geo-North */
void Reference_Rotation_Init( kinetic_t * k )
{
    ang3_t r_a = { REFERENCE_OFFSET_ANGLE_X, REFERENCE_OFFSET_ANGLE_Y, REFERENCE_OFFSET_ANGLE_Z };
    k->offset = r_a;
    Quaternion.fromEuler( &r_a, &k->qr );
}

