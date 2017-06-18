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
#include <check.h>
#include "../ble-module/kinetic/kinetic_types.h"
#include "../ble-module/kinetic/matrix.h"


/* Variables */
vec3_t A, B, C;
/*************/

void setup(void)
{
    B.i = 1;
    B.j = 2;
    B.k = 3;
}

void teardown(void)
{
}

START_TEST(test_subtractvec3_t)
{
    A.i = 1;
    A.j = 2;
    A.k = 3;
    
    subtractvec3_t(&A, &B);
    
    ck_assert_int_eq(A.i, 0);
    ck_assert_int_eq(A.j, 0);
    ck_assert_int_eq(A.k, 0);
}
END_TEST

START_TEST(test_subtractvec3_t_neg)
{
    A.i = 0;
    A.j = 0;
    A.k = 0;
    
    subtractvec3_t(&A, &B);
    
    ck_assert_int_eq(A.i, -1);
    ck_assert_int_eq(A.j, -2);
    ck_assert_int_eq(A.k, -3);
}
END_TEST


Suite * money_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    
    s = suite_create("Matrix");
    
    /* Core test case */
    tc_core = tcase_create("Core");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_subtractvec3_t);
    suite_add_tcase(s, tc_core);
    
    /* Limits test case */
    tc_limits = tcase_create("Limits");

    tcase_add_checked_fixture(tc_limits, setup, teardown);
    tcase_add_test(tc_limits, test_subtractvec3_t_neg);
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
    
    s = money_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
