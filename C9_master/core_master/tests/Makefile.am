## Process this file with automake to produce Makefile.in

check_override_CFLAGS = -DCHECK_TESTING

TESTS = system_test kinetic_test

check_PROGRAMS = system_test kinetic_test

#kinetic_tests/kalman_test.c $(top_builddir)/ble-module/kinetic/kalman.h

kinetic_test_SOURCES = $(top_builddir)/ble-module/testing/ble_core_stubs.h kinetic_tests/matrix_test.c $(top_builddir)/ble-module/kinetic/matrix.h
kinetic_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
kinetic_test_LDADD = $(top_builddir)/ble-module/libble.la @CHECK_LIBS@

system_test_SOURCES = system_tests/startup_test.c $(top_builddir)/ble-module/system/startup.h
system_test_CFLAGS = @CHECK_CFLAGS@ $(check_override_CFLAGS)
system_test_LDADD = $(top_builddir)/ble-module/libble.la @CHECK_LIBS@