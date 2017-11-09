#ifndef MATH_EXTERN_H
#define MATH_EXTERN_H

#include "../ble-module/kinetic/kinetic_types.h"
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <float.h>

void Multiply_Matrix_33x33( mat3x3_t * a, mat3x3_t * b, mat3x3_t * c )
{
    c->ii = a->ii * b->ii + a->ij * b->ji + a->ik * b->ki;
    c->ij = a->ii * b->ij + a->ij * b->jj + a->ik * b->kj;
    c->ik = a->ii * b->ik + a->ij * b->jk + a->ik * b->kk;
    c->ji = a->ji * b->ii + a->jj * b->ji + a->jk * b->ki;
    c->jj = a->ji * b->ij + a->jj * b->jj + a->jk * b->kj;
    c->jk = a->ji * b->ik + a->jj * b->jk + a->jk * b->kk;
    c->ki = a->ki * b->ii + a->kj * b->ji + a->kk * b->ki;
    c->kj = a->ki * b->ij + a->kj * b->jj + a->kk * b->kj;
    c->kk = a->ki * b->ik + a->kj * b->jk + a->kk * b->kk;
}

void Multiply_Matrix_33x33x33( mat3x3_t * a, mat3x3_t * b, mat3x3_t * c, mat3x3_t * d )
{
    mat3x3_t t;
    Multiply_Matrix_33x33(a,b,&t);
    Multiply_Matrix_33x33(&t,c,d);
}

void Euler_To_Rotation_Matrix( ang3_t * a, mat3x3_t * m )
{
    double cx = cos(a->x);
    double cy = cos(a->y);
    double cz = cos(a->z);
    double sx = sin(a->x);
    double sy = sin(a->y);
    double sz = sin(a->z);
    
    mat3x3_t x, y, z, w;
    
    x.ii =  1;
    x.ij =  0;
    x.ik =  0;
    x.ji =  0;
    x.jj =  cx;
    x.jk = -sx;
    x.ki =  0;
    x.kj = sx;
    x.kk = cx;
    
    y.ii =  cy;
    y.ij =  0;
    y.ik =  sy;
    y.ji =  0;
    y.jj =  1;
    y.jk =  0;
    y.ki = -sy;
    y.kj =  0;
    y.kk =  cy;
    
    z.ii =  cz;
    z.ij = -sz;
    z.ik =  0;
    z.ji =  sz;
    z.jj =  cz;
    z.jk =  0;
    z.ki =  0;
    z.kj =  0;
    z.kk =  1;
    
    Multiply_Matrix_33x33x33(&z,&y,&x,m);
}

void Rotation_Matrix_To_Euler( mat3x3_t * m, ang3_t * a )
{
    a->x = atan2(  m->kj, m->kk );
    a->y = atan2( -m->ki, sqrt( ( m->kj * m->kj ) + ( m->kk * m->kk ) ) );
    a->z = atan2(  m->ji, m->ii );
}
#endif /* MATH_EXTERN_H */
