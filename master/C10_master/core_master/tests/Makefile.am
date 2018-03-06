## Process this file with automake to produce Makefile.in

check_override_CFLAGS = -DCHECK_TESTING

TESTS = system_test kalman_test kinetic_test matrix_test

check_PROGRAMS = system_test kalman_test kinetic_test matrix_test

system_test_SOURCES = system_tests/startup_test.c $(top_builddir)/ble-module/system/startup.h
system_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
system_test_LDADD = $(top_builddir)/ble-module/lib_ble_system.la @CHECK_LIBS@

kalman_test_SOURCES = $(top_builddir)/ble-module/testing/ble_core_stubs.h kinetic_tests/kalman_test.c $(top_builddir)/ble-module/kinetic/kalman.h 
kalman_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
kalman_test_LDADD = $(top_builddir)/ble-module/lib_ble_kalman.la @CHECK_LIBS@

kinetic_test_SOURCES = $(top_builddir)/ble-module/testing/ble_core_stubs.h kinetic_tests/kinetic_test.c $(top_builddir)/ble-module/kinetic/kinetic.h #$(top_builddir)/ble-module/sensors/sensor_data_types.h $(top_builddir)/ble-module/sensors/imu/LSM9DS1_regs.c $(top_builddir)/ble-module/sensors/imu/LSM9DS1_regs.h $(top_builddir)/ble-module/sensors/imu/LSM9DS1.c $(top_builddir)/ble-module/sensors/imu/LSM9DS1.h 
kinetic_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
kinetic_test_LDADD = $(top_builddir)/ble-module/lib_ble_kinetic.la @CHECK_LIBS@

matrix_test_SOURCES = $(top_builddir)/ble-module/testing/ble_core_stubs.h kinetic_tests/matrix_test.c $(top_builddir)/ble-module/kinetic/matrix.h
matrix_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
matrix_test_LDADD = $(top_builddir)/ble-module/lib_ble_matrix.la @CHECK_LIBS@
