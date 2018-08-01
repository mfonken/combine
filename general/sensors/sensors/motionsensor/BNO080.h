//
//  BNO080.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef BNO080_h
#define BNO080_h

#include "i2c_template.h"
#include "sh2.h"

#define SHTP_HEADER_LENGTH 4
#define SHTP_MAX_PACKET_SIZE 128 //Packets can be up to 32k but we don't have that much RAM.
#define BNO080_MAX_METADATA_SIZE 9
#define BNO080_DEFAULT_ADDRESS 0x4B
#define BNO080_UART_RVC_HEADER 0xAAAA

typedef enum
{
    SHTP_EXECUTABLE_COMMAND_RESET = 1,
    SHTP_EXECUTABLE_COMMAND_ON,
    SHTP_EXECUTABLE_COMMAND_SLEEP
} shtp_executable_commands;

typedef enum
{
    SHTP_EXECUTABLE_RESPONSE_RESET_COMPLETE = 1
} shtp_executable_responses;

typedef enum
{
    SHTP_COMMAND = 0,
    SHTP_EXECUTABLE,
    SHTP_CONTROL,
    SHTP_INPUT_REPORT,
    SHTP_WAKE_REPORT,
    SHTP_ROTATION
} shtp_channel;

typedef struct
{
    uint16_t        length:15;
    shtp_channel    channel:8;
    uint8_t         seqnum:8;
} shtp_packet_header_t;

typedef struct
{
uint16_t
    header,
    index,
    yaw,
    pitch,
    roll;
    struct
    {
        uint16_t x, y, z;
    } accel;
uint8_t
    reserved[3],
    checksum;
} bno_uart_rvc_packet;

typedef struct
{
    shtp_packet_header_t
        header;
    sh2_base_timestamp_reference_record
        timestamp;
    uint8_t
        data[SHTP_MAX_PACKET_SIZE];
    uint32_t
        metadata[BNO080_MAX_METADATA_SIZE];
} shtp_t;

static i2c_event_t bno080GetSHTPHeaderI2CEvent(void) { return (i2c_event_t){ I2C_READ_REG_EVENT, BNO080_DEFAULT_ADDRESS, NO_REG, SHTP_HEADER_LENGTH }; }
static i2c_event_t bno080GetSHTPPacketI2CEvent(uint8_t len) { return (i2c_event_t){ I2C_READ_REG_EVENT, BNO080_DEFAULT_ADDRESS, NO_REG, len }; }


typedef struct
{
    i2c_event_t (*GetSHTPHeaderEvent)(void);
    i2c_event_t (*GetSHTPPacketEvent)(uint8_t);
} bno080_functions;

static bno080_functions BNO080Functions =
{
    .GetSHTPHeaderEvent = bno080GetSHTPHeaderI2CEvent,
    .GetSHTPPacketEvent = bno080GetSHTPPacketI2CEvent
};

void ParseAccelerometerReport(sh2_accelerometer_input_report * );
void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report * );
void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report * );
void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report * );
void ParseRotationVectorReport(sh2_rotation_vector_input_report * );
void ParseGravityReport(sh2_gravity_input_report * );
void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report * );
void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report * );
void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report * );
void ParsePressureReport(sh2_pressure_input_report * );
void ParseAmbientLightReport(sh2_ambient_light_input_report * );
void ParseHumidityReport(sh2_humidity_input_report * );
void ParseProximityReport(sh2_proximity_input_report * );
void ParseTemperatureReport(sh2_temperature_input_report * );
void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report * );
void ParseTapDetectorReport(sh2_tap_detector_input_report * );
void ParseStepCounterReport(sh2_step_counter_input_report * );
void ParseSignificantMotionReport(sh2_significant_motion_input_report * );
void ParseStabilityClassifierReport(sh2_stability_classifier_input_report * );
void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report * );
void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report * );
void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report * );
//void ParseSARReport(sh2_sar_input_report * );
void ParseStepDetectorReport(sh2_step_detector_input_report * );
void ParseShakeDetectorReport(sh2_shake_detector_input_report * );
void ParseFlipDetectorReport(sh2_flip_detector_input_report * );
void ParsePickupDetectorReport(sh2_pickup_detector_input_report * );
void ParseStabilityDetectorReport(sh2_stability_detector_input_report * );
void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report * );
void ParseSleepDetectorReport(sh2_sleep_detector_input_report * );
void ParseTiltDetectorReport(sh2_tilt_detector_input_report * );
void ParsePocketDetectorReport(sh2_pocket_detector_input_report * );
void ParseCircleDetectorReport(sh2_circle_detector_input_report * );
void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report * );
void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report * );
void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report * );
void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report * );
//void ParseFlushCompletedReport(sh2_flush_completed_input_report * );
//void ParseForceSensorFlushReport(sh2_force_sensor_flush_input_report * );
//void ParseFeatureResponseReport(sh2_feature_response_input_report * );
//void ParseFeatureCommandReport(sh2_feature_command_input_report * );
//void ParseFeatureRequestReport(sh2_feature_request_input_report * );

#endif /* BNO080_h */
