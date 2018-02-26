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
#include "../ble-module/system/startup.h"


/* Variables */
system_t sys;
/*************/

void setup(void)
{
    startup(&sys);
}

void teardown(void)
{
}

START_TEST(test_startup)
{
    startup(&sys);
    uint8_t A = *(uint8_t *)&sys;
    ck_assert_int_eq(A, SYSTEM_DEFAULT);
}
END_TEST


Suite * startup_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;
    
    s = suite_create("Startup");
    
    /* Core test case */
    tc_core = tcase_create("Core");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_startup);
    suite_add_tcase(s, tc_core);
    
    /* Limits test case */
//    tc_limits = tcase_create("Limits");
//
//    tcase_add_checked_fixture(tc_limits, setup, teardown);
//    tcase_add_test(tc_limits, test_subtractvec3_t_neg);
//    suite_add_tcase(s, tc_limits);
    
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    
//    const char *fname = "My SRunner";
//    void srunner_set_xml(SRunner *sr, const char *fname);
    
    s = startup_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
