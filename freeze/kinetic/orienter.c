//
//  orienter.c
//  ots-proto
//
//  Created by Matthew Fonken on 5/2/22.
//

#include "orienter.h"

void Orienter_Init( orienter_t * o )
{
//    kalman_uncertainty_c motion_uncertainty = { MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY };
//    for( int i = 0; i < 3; i++ )
//        Kalman.init(&o->rotation_filters[i], 0.0, MOTION_MAX_KALMAN_LIFE, motion_uncertainty );
}

void Orienter_Update( orienter_t * o, vec3_t * ang )//, vec3_t * rate )
{
    double * p_ang = (double *)ang;
//    double * p_rate = (double *)rate;
    double * p_rot = (double *)&o->rotation;
//    double * p_rot_raw = (double *)&o->rotation_raw;
    double * p_offset = (double *)&o->offset;
    for( int i = 0; i < 3; i++ )
    {
//        double v = Kalman.update( &o->rotation_filters[i], p_ang[i], p_rate[i], ACCELERATION );
        p_rot[i] = p_ang[i] - p_offset[i];
    }
    Vector3.copy3( ang, (vec3_t *)&o->rotation_raw );
    
//    OrienterFunctions.Print( o );
}

void Orienter_Tare( orienter_t * o )//, vec3_t * gravity )
{
//    Vector3.copy3( gravity, &o->gravity );
//    quaternion_t rot_q;
//    ang3_t rot_e;
//    Vector3.copy3( (vec3_t *)&o->rotation_raw, (vec3_t *)&rot_e);
//    Angle.degToRad( &rot_e );
//    Quaternion.fromEuler( &rot_e, &rot_q );
//    vec3_t u = { 1, 0, 0 }, ori;
//    Quaternion.rotVec( &u, &rot_q, &ori );
    
//    double * p_rot = (double *)&o->rotation_raw;
//    vec3_t rot; double * p_rot = (double *)&rot;
//    for( int i = 0; i < 3; i++ )
//        p_rot[i] = p_rot_raw[i];
//    double pitch = o->rotation_raw.y;// M_PI - Vector3.ang3( &ori, gravity );
//    printf("Pitch: %.2fdeg\n", pitch);
    
    o->offset.i = o->rotation_raw.x;
    o->offset.j = o->rotation_raw.y;// - pitch;
    o->offset.k = o->rotation_raw.z;// - ;
}
void Orienter_Print( orienter_t * o )
{
    ang3_t * r = &o->rotation;
    ang3_t * rr = &o->rotation_raw;
    printf("Orienter: <%.2f, %.2f, %.2f> | <%.2f, %.2f, %.2f>\n", rr->x, rr->y, rr->z, r->x, r->y, r->z);
}

orienter_functions_t OrienterFunctions =
{
    .Init = Orienter_Init,
    .Update = Orienter_Update,
    .Tare = Orienter_Tare,
    .Print = Orienter_Print
};
