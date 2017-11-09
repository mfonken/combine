/*
 * Check: a unit test framework for C
 * Copyright (C) 2001, 2002 Arien Malec
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "../check.h"
#include "../math_extern.h"
#include <check_stdint.h>
#include <sys/types.h>

#include "../ble-module/kinetic/kinetic_types.h"
#include "../ble-module/kinetic/matrix.h"

#define RESULT_STRING_LEN 100
#define DOUBLE_ASSERT_TOL 0.001

/* Variables */
vec2_t a, b;
vec3_t A, B, C;

double result_double = 0;
char   result_string[RESULT_STRING_LEN];

/*************/

void setup(void)
{
    result_double = 0;
    memset(result_string, 0, RESULT_STRING_LEN);
}

void teardown(void)
{
}

START_TEST(test_subtractvec3_t)
{
    A.i = 1;
    A.j = 2;
    A.k = 3;
    
    B.i = 1;
    B.j = 2;
    B.k = 3;
    
    subtractvec3_t(&A, &B);
    
    ck_assert_double_eq_tol(A.i, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.j, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.k, 0, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_subtractvec3_t_neg)
{
    A.i = 0;
    A.j = 0;
    A.k = 0;
    
    B.i = 1;
    B.j = 2;
    B.k = 3;
    
    subtractvec3_t(&A, &B);
    
    ck_assert_double_eq_tol(A.i, -1, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.j, -2, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.k, -3, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_lengthOfvec3_t)
{
    A.i = 1;
    A.j = 2;
    A.k = 3;
    
    B.i = 1;
    B.j = 2;
    B.k = 3;
    
    result_double = lengthOfvec3_t(&A);
    ck_assert_double_eq_tol(result_double, sqrt(14), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_lengthOfvec3_t_neg)
{
    A.i = -1;
    A.j = -2;
    A.k = -3;
    
    result_double = lengthOfvec3_t(&A);
    ck_assert_double_eq_tol(result_double, sqrt(14), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_lengthOfvec3_t_zero)
{
    A.i = 0;
    A.j = 0;
    A.k = 0;
    
    result_double = lengthOfvec3_t(&A);
    ck_assert_double_eq_tol(result_double, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_lengthOfvec2_t)
{
    a.i = 1;
    a.j = 2;
    
    result_double = lengthOfvec2_t(&a);
    ck_assert_double_eq_tol(result_double, sqrt(5), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_lengthOfvec2_t_neg)
{
    a.i = -1;
    a.j = -2;
    
    result_double = lengthOfvec2_t(&a);
    ck_assert_double_eq_tol(result_double, sqrt(5), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_lengthOfvec2_t_zero)
{
    a.i = 0;
    a.j = 0;
    
    result_double = lengthOfvec2_t(&a);
    ck_assert_double_eq_tol(result_double, sqrt(0), DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_normalizevec3_t)
{
    A.i = 1;
    A.j = 2;
    A.k = 3;
    
    normalizevec3_t(&A);
    
    double len = sqrt(14);
    ck_assert_double_eq_tol(A.i, 1/len, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.j, 2/len, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.k, 3/len, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_normalizevec3_t_neg)
{
    A.i = -1;
    A.j = -2;
    A.k = -3;
    
    normalizevec3_t(&A);
    double len = sqrt(14);
    ck_assert_double_eq_tol(A.i, -1/len, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.j, -2/len, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.k, -3/len, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_normalizevec3_t_zero)
{
    A.i = 0;
    A.j = 0;
    A.k = 0;
    
    normalizevec3_t(&A);
    
    ck_assert_double_eq_tol(A.i, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.j, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(A.k, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_get2dDistance)
{
    cartesian2_t m, n;
    m.x = 1;
    m.y = 2;
    n.x = 3;
    n.y = 4;
    
    result_double = get2dDistance(&m,&n);
    ck_assert_double_eq_tol(result_double, sqrt(20), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_get2dDistance_neg)
{
    cartesian2_t m, n;
    m.x = -1;
    m.y = -2;
    n.x = -3;
    n.y = -4;
    
    result_double = get2dDistance(&m,&n);
    ck_assert_double_eq_tol(result_double, sqrt(20), DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_get2dDistance_zero)
{
    cartesian2_t m, n;
    m.x = 1;
    m.y = 2;
    n.x = 1;
    n.y = 2;
    
    result_double = get2dDistance(&m,&n);
    ck_assert_double_eq_tol(result_double, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Multiply_Vec_3x1)
{
    mat3x3_t o;
    o.ii = 1;
    o.ij = 2;
    o.ik = 3;
    o.ji = 4;
    o.jj = 5;
    o.jk = 6;
    o.ki = 7;
    o.kj = 8;
    o.kk = 9;
    
    A.i = 10;
    A.j = 11;
    A.k = 12;
    
    
    Multiply_Vec_3x1(&o,&A,&B);
    ck_assert_double_eq_tol(B.i, 68, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.j, 167, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.k, 266, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Multiply_Vec_3x1_neg)
{
    mat3x3_t o;
    o.ii = -1;
    o.ij = -2;
    o.ik = -3;
    o.ji = -4;
    o.jj = -5;
    o.jk = -6;
    o.ki = -7;
    o.kj = -8;
    o.kk = -9;
    
    A.i = 10;
    A.j = 11;
    A.k = 12;
    
    
    Multiply_Vec_3x1(&o,&A,&B);
    ck_assert_double_eq_tol(B.i, -68, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.j, -167, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.k, -266, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Multiply_Vec_3x1_zero)
{
    mat3x3_t o;
    o.ii = 0;
    o.ij = 0;
    o.ik = 0;
    o.ji = 0;
    o.jj = 0;
    o.jk = 0;
    o.ki = 0;
    o.kj = 0;
    o.kk = 0;
    
    A.i = 0;
    A.j = 0;
    A.k = 0;
    
    
    Multiply_Vec_3x1(&o,&A,&B);
    ck_assert_double_eq_tol(B.i, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.j, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(B.k, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Euler_To_Quaternion)
{
    ang3_t a;
    a.x = M_PI/4;
    a.y = M_PI/6;
    a.z = M_PI/8;
    quaternion_t H;
    
    Euler_To_Quaternion(&a,&H);
    ck_assert_double_eq_tol(H.x, 0.315892, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.y, 0.306637, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.z, 0.076956, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.w, 0.894575, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Euler_To_Quaternion_neg)
{
    ang3_t a;
    a.x = -M_PI/4;
    a.y = -M_PI/6;
    a.z = -M_PI/8;
    quaternion_t H;
    
    Euler_To_Quaternion(&a,&H);
    ck_assert_double_eq_tol(H.x, -0.409191, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.y, -0.162409, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.z, -0.271241, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.w,  0.855929, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Euler_To_Quaternion_zero)
{
    ang3_t a;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    quaternion_t H;
    
    Euler_To_Quaternion(&a,&H);
    ck_assert_double_eq_tol(H.x, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.y, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.z, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H.w, 1, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Quaternion_To_Matrix)
{
    ang3_t a;
    a.x = M_PI/4;
    a.y = M_PI/6;
    a.z = M_PI/8;
    quaternion_t H;
    Euler_To_Quaternion(&a,&H);

    mat3x3_t b;
    Quaternion_To_Matrix(&H, &b);
    ck_assert_double_eq_tol(b.ii, 0.800103, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ji, 0.0560425, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ki, 0.597239, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ij, 0.331414, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jj, 0.78858, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kj, -0.517983, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ik, -0.5, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jk, 0.612373, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kk, 0.612372, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Quaternion_To_Matrix_neg)
{
    ang3_t a;
    a.x = -M_PI/4;
    a.y = -M_PI/6;
    a.z = -M_PI/8;
    quaternion_t H;
    Euler_To_Quaternion(&a,&H);

    mat3x3_t b;
    Quaternion_To_Matrix(&H, &b);
    ck_assert_double_eq_tol(b.ii, 0.800103, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ji, 0.597239, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ki, -0.0560424, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ij, -0.331413, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jj, 0.517982, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kj, 0.788581, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ik, 0.5, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jk, -0.612373, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kk, 0.612372, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Quaternion_To_Matrix_zero)
{
    ang3_t a;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    quaternion_t H;
    Euler_To_Quaternion(&a,&H);
    
    mat3x3_t b;
    Quaternion_To_Matrix(&H, &b);
    ck_assert_double_eq_tol(b.ii, 1, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ji, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ki, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ij, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jj, 1, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kj, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.ik, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.jk, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(b.kk, 1, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Rotate_Vector_By_Quaternion)
{
    ang3_t a;
    a.x = M_PI/4;
    a.y = M_PI/6;
    a.z = M_PI/8;
    quaternion_t H;
    Euler_To_Quaternion(&a,&H);
    
    vec3_t v, r;
    v.i = 1;
    v.j = 2;
    v.k = 3;
    r.i = 0;
    r.j = 0;
    r.k = 0;
    
    Rotate_Vector_By_Quaternion(&v, &H, &r);

    ck_assert_double_eq_tol(r.i, 2.70391, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.j, 0.354625, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.k, 2.56186, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Rotate_Vector_By_Quaternion_neg)
{
    ang3_t a;
    a.x = -M_PI/4;
    a.y = -M_PI/6;
    a.z = -M_PI/8;

    quaternion_t H;
    Euler_To_Quaternion(&a,&H);
    
    vec3_t v, r;
    v.i = -1;
    v.j = -2;
    v.k = -3;
    r.i = 0;
    r.j = 0;
    r.k = 0;
    
    Rotate_Vector_By_Quaternion(&v, &H, &r);

    ck_assert_double_eq_tol(r.i, -1.82645, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.j, -3.07029, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.k, -1.11237, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Rotate_Vector_By_Quaternion_zero)
{
    ang3_t a;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    quaternion_t H;
    Euler_To_Quaternion(&a,&H);
    
    vec3_t v, r;
    v.i = 1;
    v.j = 2;
    v.k = 3;
    r.i = 0;
    r.j = 0;
    r.k = 0;
    
    Rotate_Vector_By_Quaternion(&v, &H, &r);
    
    ck_assert_double_eq_tol(r.i, 1, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.j, 2, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(r.k, 3, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Quaternion_Combine)
{
    ang3_t a, b, c, d, r;
    a.x =  -M_PI/2;
    a.y =   M_PI/3;
    a.z =   M_PI/4;
    b.x =  -M_PI/5;
    b.y =   M_PI/6;
    b.z =   M_PI/7;
    c.x =  -M_PI/8;
    c.y =   M_PI/9;
    c.z =   M_PI/10;
    quaternion_t H_a, H_b, H_c, H_d, H_r;
    
    /* Solve as quaterions */
    Euler_To_Quaternion(&a,&H_a);
    Euler_To_Quaternion(&b,&H_b);
    Euler_To_Quaternion(&c,&H_c);
    Quaternion_Combine(&H_a, &H_b, &H_c, &H_d);
    
    /* Solve as rotation matrices */
    mat3x3_t m_a, m_b, m_c, m_d, m_r;
    Euler_To_Rotation_Matrix(&a, &m_a);
    Euler_To_Rotation_Matrix(&b, &m_b);
    Multiply_Matrix_33x33(&m_a, &m_b, &m_d);
    Euler_To_Rotation_Matrix(&c, &m_c);
    Multiply_Matrix_33x33(&m_d, &m_c, &m_r);
    Rotation_Matrix_To_Euler(&m_r, &r);
    Euler_To_Quaternion(&r,&H_r);
    
    /* Check if opposite (I think from the atan2 function) */
    if( (int)(H_d.x*100) == -(int)(H_r.x*100) ) H_r.x *= -1; H_r.y *= -1; H_r.z *= -1;
    if( (int)(H_d.w*100) == -(int)(H_r.w*100) ) H_r.w *= -1;
    
    ck_assert_double_eq_tol(H_d.x, H_r.x, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.y, H_r.y, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.z, H_r.z, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.w, H_r.w, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Quaternion_Combine_over_rotate)
{
    ang3_t a, b, c, d, r;
    a.x =  -M_PI*1;
    a.y =   M_PI*2;
    a.z =   M_PI*3;
    b.x =  -M_PI*4;
    b.y =   M_PI*5;
    b.z =   M_PI*6;
    c.x =  -M_PI*7;
    c.y =   M_PI*8;
    c.z =   M_PI*9;
    
    quaternion_t H_a, H_b, H_c, H_d, H_r;
    
    /* Solve as quaterions */
    Euler_To_Quaternion(&a,&H_a);
    Euler_To_Quaternion(&b,&H_b);
    Euler_To_Quaternion(&c,&H_c);
    Quaternion_Combine(&H_a, &H_b, &H_c, &H_d);
    
    /* Solve as rotation matrices */
    mat3x3_t m_a, m_b, m_c, m_d, m_r;
    Euler_To_Rotation_Matrix(&a, &m_a);
    Euler_To_Rotation_Matrix(&b, &m_b);
    Multiply_Matrix_33x33(&m_a, &m_b, &m_d);
    Euler_To_Rotation_Matrix(&c, &m_c);
    Multiply_Matrix_33x33(&m_d, &m_c, &m_r);
    Rotation_Matrix_To_Euler(&m_r, &r);
    Euler_To_Quaternion(&r,&H_r);
    
    /* Check if opposite (I think from the atan2 function) */
    if( (int)(H_d.x*100) == -(int)(H_r.x*100) ) H_r.x *= -1;
    if( (int)(H_d.y*100) == -(int)(H_r.y*100) ) H_r.y *= -1;
    if( (int)(H_d.z*100) == -(int)(H_r.z*100) ) H_r.z *= -1;
    if( (int)(H_d.w*100) == -(int)(H_r.w*100) ) H_r.w *= -1;
    
    ck_assert_double_eq_tol(H_d.x, H_r.x, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.y, H_r.y, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.z, H_r.z, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.w, H_r.w, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Quaternion_Combine_zero)
{
    ang3_t a, b, c, d, r;
    a.x = 0;
    a.y = 0;
    a.z = 0;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    c.x = 0;
    c.y = 0;
    c.z = 0;
    
    quaternion_t H_a, H_b, H_c, H_d, H_r;
    
    /* Solve as quaterions */
    Euler_To_Quaternion(&a,&H_a);
    Euler_To_Quaternion(&b,&H_b);
    Euler_To_Quaternion(&c,&H_c);
    Quaternion_Combine(&H_a, &H_b, &H_c, &H_d);
    
    /* Solve as rotation matrices */
    mat3x3_t m_a, m_b, m_c, m_d, m_r;
    Euler_To_Rotation_Matrix(&a, &m_a);
    Euler_To_Rotation_Matrix(&b, &m_b);
    Multiply_Matrix_33x33(&m_a, &m_b, &m_d);
    Euler_To_Rotation_Matrix(&c, &m_c);
    Multiply_Matrix_33x33(&m_d, &m_c, &m_r);
    Rotation_Matrix_To_Euler(&m_r, &r);
    Euler_To_Quaternion(&r,&H_r);
    
    /* Check if opposite (I think from the atan2 function) */
    if( (int)(H_d.x*100) == -(int)(H_r.x*100) ) H_r.x *= -1; H_r.y *= -1; H_r.z *= -1;
    if( (int)(H_d.w*100) == -(int)(H_r.w*100) ) H_r.w *= -1;
    
    ck_assert_double_eq_tol(H_d.x, H_r.x, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.y, H_r.y, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.z, H_r.z, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(H_d.w, H_r.w, DOUBLE_ASSERT_TOL);
}
END_TEST

Suite * matrix_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    
    s = suite_create("Matrix");
    
    /* Core test case */
    tc_core = tcase_create("Core");
    /* Limits test case */
    tc_limits = tcase_create("Limits");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_checked_fixture(tc_limits, setup, teardown);
    
    tcase_add_test(tc_core, test_subtractvec3_t);
    tcase_add_test(tc_limits, test_subtractvec3_t_neg);
    
    tcase_add_test(tc_core, test_lengthOfvec3_t);
    tcase_add_test(tc_limits, test_lengthOfvec3_t_neg);
    tcase_add_test(tc_limits, test_lengthOfvec3_t_zero);
    
    tcase_add_test(tc_core, test_lengthOfvec2_t);
    tcase_add_test(tc_limits, test_lengthOfvec2_t_neg);
    
    tcase_add_test(tc_core, test_normalizevec3_t);
    tcase_add_test(tc_limits, test_normalizevec3_t_neg);
    tcase_add_test(tc_limits, test_normalizevec3_t_zero);
    
    tcase_add_test(tc_core, test_get2dDistance);
    tcase_add_test(tc_limits, test_get2dDistance_neg);
    tcase_add_test(tc_limits, test_get2dDistance_zero);
    
    tcase_add_test(tc_core, test_Multiply_Vec_3x1);
    tcase_add_test(tc_limits, test_Multiply_Vec_3x1_neg);
    tcase_add_test(tc_limits, test_Multiply_Vec_3x1_zero);
    
    tcase_add_test(tc_core, test_Euler_To_Quaternion);
    tcase_add_test(tc_limits, test_Euler_To_Quaternion_neg);
    tcase_add_test(tc_limits, test_Euler_To_Quaternion_zero);
    
    tcase_add_test(tc_core, test_Quaternion_To_Matrix);
    tcase_add_test(tc_limits, test_Quaternion_To_Matrix_neg);
    tcase_add_test(tc_limits, test_Quaternion_To_Matrix_zero);
    
    tcase_add_test(tc_core, test_Rotate_Vector_By_Quaternion);
    tcase_add_test(tc_limits, test_Rotate_Vector_By_Quaternion_neg);
    tcase_add_test(tc_limits, test_Rotate_Vector_By_Quaternion_zero);
    
    tcase_add_test(tc_core, test_Quaternion_Combine);
    tcase_add_test(tc_limits, test_Quaternion_Combine_over_rotate);
    tcase_add_test(tc_limits, test_Quaternion_Combine_zero);
    
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_limits);
    
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    
//    const char *fname = "My SRunner";
//    void srunner_set_xml(SRunner *sr, const char *fname);
    
    s = matrix_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
