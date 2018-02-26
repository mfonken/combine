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
#include "../ble-module/kinetic/kalman.h"

#define RESULT_STRING_LEN 100
#define DOUBLE_ASSERT_TOL 0.001

/* Variables */
kalman_t k;

/*************/

void setup(void)
{
    
}

void teardown(void)
{
}

START_TEST(test_Kalman_Init)
{
    double init_value = 1.23;
    Kalman_Init(&k, init_value);
    ck_assert_double_eq_tol(k.K[0], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.K[1], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[0][0], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[0][1], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[1][0], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[1][1], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rate, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.bias, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.value, init_value, DOUBLE_ASSERT_TOL);
}
END_TEST
START_TEST(test_Kalman_Update)
{
    Kalman_Init(&k, 0);
    Kalman_Update(&k, 3, 2, 1);
    
    ck_assert_double_eq_tol(k.K[0], 0.032258, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.K[1], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[0][0], 0.001, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[0][1], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[1][0], 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.P_k[1][1], 0.003, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.rate, 2, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.bias, 0, DOUBLE_ASSERT_TOL);
    ck_assert_double_eq_tol(k.value, 2.032258, DOUBLE_ASSERT_TOL);
}
END_TEST


Suite * kalman_test(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    
    s = suite_create("Kalman");
    
    /* Core test case */
    tc_core = tcase_create("Core");
    tc_limits = tcase_create("Limits");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_checked_fixture(tc_limits, setup, teardown);
    
    tcase_add_test(tc_core, test_Kalman_Init);
    tcase_add_test(tc_core, test_Kalman_Update);
    
//    tcase_add_test(tc_limits, test_subtractvec3_t_neg);

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
    
    s = kalman_test();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
