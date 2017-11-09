## Process this file with automake to produce Makefile.in

test_stub = testing/ble_core_stubs.c testing/ble_core_stubs.h
system_SOURCES = system/startup.c system/startup.h
kinetic_SOURCES = kinetic/kalman.c kinetic/kalman.h kinetic/matrix.c kinetic/matrix.h kinetic/kinetic.c kinetic/kinetic.h
sensors_SOURCES = sensors/sensor_data_types.h sensors/imu/LSM9DS1_types.h sensors/camera/cam_controller.c sensors/camera/cam_controller.h sensors/touch/CPT112S.c sensors/touch/CPT112S.h sensors/imu/LSM9DS1.c sensors/imu/LSM9DS1.h sensors/imu/LSM9DS1_regs.c sensors/imu/LSM9DS1_regs.h 
AM_CFLAGS = -DCHECK_TESTING

lib_LTLIBRARIES = lib_ble_system.la lib_ble_kalman.la lib_ble_kinetic.la lib_ble_matrix.la

lib_ble_system_la_SOURCES	= $(test_stub) $(system_SOURCES)
lib_ble_kalman_la_SOURCES 	= $(test_stub) $(kinetic_SOURCES) 
lib_ble_kinetic_la_SOURCES 	= $(test_stub) $(kinetic_SOURCES) $(sensors_SOURCES)
lib_ble_matrix_la_SOURCES 	= $(test_stub) $(kinetic_SOURCES) 

bin_PROGRAMS = main
main_SOURCES = main.c
main_LDADD = lib_ble_system.la lib_ble_kalman.la lib_ble_kinetic.la lib_ble_matrix.la