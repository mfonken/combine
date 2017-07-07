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
#include "../ble-module/kinetic/kinetic.h"
#include "../ble-module/sensors/sensor_data_types.h"
#include "../ble-module/sensors/imu/LSM9DS1.h"
#include "../ble-module/sensors/camera/cam_controller.h"


#define RESULT_STRING_LEN 100
#define DOUBLE_ASSERT_TOL 0.001

LSM9DS1_t       l;
kinetic_t       k;
cartesian2_t    b[2];

void setup(void)
{
}

void teardown(void)
{
}

START_TEST(test_Filters_Init)
{
    Filters_Init(&l, &k);
    ck_assert_double_eq_tol(k.rotationFilter[0].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[1].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[2].value, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Kinetic_Init)
{
    Kinetic_Init(&l, &k);
    ck_assert_double_eq_tol(k.rotationFilter[0].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[1].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[2].value, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Camera_Rotation_Init)
{
    Camera_Rotation_Init();
}
END_TEST


START_TEST(test_Kinetic_Update_Rotation)
{
    Kinetic_Update_Rotation(&l, &k);
    ck_assert_double_eq_tol(k.rotationFilter[0].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[1].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[2].value, 0, DOUBLE_ASSERT_TOL);
}
END_TEST

START_TEST(test_Kinetic_Update_Position)
{
    b[0].x = 0;
    b[0].y = 0;
    b[1].x = 0;
    b[1].y = 0;
    Kinetic_Update_Position(&l, &k, b);
    ck_assert_double_eq_tol(k.rotationFilter[0].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[1].value, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rotationFilter[2].value, 0, DOUBLE_ASSERT_TOL);
}
END_TEST


Suite * kinetic_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    
    s = suite_create("Kinetic");
    
    /* Core test case */
    tc_core = tcase_create("Core");
    /* Limits test case */
    tc_limits = tcase_create("Limits");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_checked_fixture(tc_limits, setup, teardown);
    
    tcase_add_test(tc_core, test_Filters_Init);
    tcase_add_test(tc_core, test_Kinetic_Init);
    tcase_add_test(tc_core, test_Camera_Rotation_Init);
    tcase_add_test(tc_core, test_Kinetic_Update_Rotation);
    tcase_add_test(tc_core, test_Kinetic_Update_Position);

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
    
    s = kinetic_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
