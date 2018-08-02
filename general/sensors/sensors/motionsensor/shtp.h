//
//  shtp.h
//  sensors
//
//  Created by Matthew Fonken on 7/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef shtp_h
#define shtp_h

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c_template.h"
#include "sh2.h"
#include <string.h>

#define SHTP_CLIENT_BUFFER_LEN 100

#define SHTP_HEADER_LENGTH 4
#define SHTP_MAX_PACKET_SIZE 128 // bytes - packets can be up to 32k but we don't have that much RAM.
#define SHTP_MAX_MEMORY_SIZE 128 // bytes
#define SHTP_MAX_METADATA_SIZE 9 // words

#define SHTP_REPORT_STATUS_MASK 0x03
#define SHTP_FRS_STATUS_MASK 0x0f

typedef struct
{
    uint16_t x, y, z;
} axis3_t;
typedef enum
{
    I2C = 0x01,
    SPI = 0x02
} comm_channel;

typedef struct
{
    comm_channel channel;
    uint8_t address;
} shtp_client_header;

typedef struct
{
    uint16_t length;
    uint16_t offset;
    uint8_t data[SHTP_CLIENT_BUFFER_LEN];
} shtp_client_buffer;
void PushBytesToSHTPClientBufferNext(uint8_t *, uint16_t, shtp_client_buffer * );
void PushAxis3ToSHTPClientBufferNext( axis3_t *, shtp_client_buffer * );
uint8_t * GetSHTPClientBufferNext( shtp_client_buffer * );
typedef struct
{
    void (*Push)(uint8_t *, uint16_t, shtp_client_buffer * );
    void (*PushAxis3)( axis3_t *, shtp_client_buffer * );
    uint8_t *(*GetNext)( shtp_client_buffer *  );
} shtp_client_buffer_functions;
static shtp_client_buffer_functions SHTPClientBufferFunctions =
{
    .Push = PushBytesToSHTPClientBufferNext,
    .PushAxis3 = PushAxis3ToSHTPClientBufferNext,
    .GetNext = GetSHTPClientBufferNext
};

typedef struct
{
    uint8_t
    reset_cause,
    sw_version_major,
    sw_version_minor;
    uint32_t
    sw_part_number,
    sw_build_number;
    uint16_t
    sw_version_patch;
} shtp_client_product_id;

typedef struct
{
    shtp_client_header header;
    shtp_client_buffer buffer;
    shtp_client_product_id product;
} shtp_client_t;

static shtp_client_t * active_client;

typedef enum
{
    SHTP_EXECUTABLE_COMMAND_RESET = 1,
    SHTP_EXECUTABLE_COMMAND_ON,
    SHTP_EXECUTABLE_COMMAND_SLEEP
} SHTP_EXECUTABLE_COMMAND;

typedef enum
{
    SHTP_EXECUTABLE_RESPONSE_RESET_COMPLETE = 1
} SHTP_EXECUTABLE_RESPONSE;

typedef enum
{
    SHTP_COMMAND_CHANNEL = 0,
    SHTP_EXECUTABLE_CHANNEL,
    SHTP_CONTROL_CHANNEL,
    SHTP_INPUT_REPORT_CHANNEL,
    SHTP_WAKE_REPORT_CHANNEL,
    SHTP_ROTATION_CHANNEL
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
} shtp_uart_rvc_packet;

typedef struct
{
    shtp_packet_header_t
    header;
    sh2_base_timestamp_reference_record
    timestamp;
    uint8_t
        data[SHTP_MAX_PACKET_SIZE];
    uint32_t
    metadata[SHTP_MAX_METADATA_SIZE];
} shtp_t;

static shtp_t local;

void SetSHTPRotationVector( uint32_t );

void SendSHTPPacket(void);
void SendSHTPExecutableCommand(SHTP_EXECUTABLE_COMMAND command);
void SendSHTPConfigurationReportCommandRequest(void);
void SendSHTPConfigurationFRSReadRequest(uint16_t record_id, uint16_t read_offset, uint16_t block_size);
void SendSHTPConfigurationFRSWriteDataRequest(void);
void SendSHTPConfigurationFRSWriteRequest(void);
void SendSHTPConfigurationProductIDRequest(void);
void SendSHTPFeatureCommand( uint8_t, uint32_t, uint32_t );

void ReceiveSHTPHeader( shtp_packet_header_t * );
void ReceiveSHTPPacket(void);
void ParseSHTPControlReport(void);
void ParseSHTCommandResponse(void);
SHTP_EXECUTABLE_RESPONSE ParseSHTPExecutableResponse(void);
void ParseSHTPControlResponse(void);
void ParseSHTPInputReport(void);
void ParseSHTPWakeReport(void);
void ParseSHTPRotation(void);
 
void ParseSHTPConfigurationReportCommandResponse(sh2_command_response *, shtp_client_buffer *);
bool ParseSHTPConfigurationFRSReadResponse(void);
bool ParseSHTPConfigurationFRSWriteResponse(void);
void ParseSHTPConfigurationProductIDResponse(void);

static i2c_event_t GetSHTPHeaderReceiveI2CEvent(void) { return (i2c_event_t){ I2C_READ_REG_EVENT, active_client->header.address, NO_REG, SHTP_HEADER_LENGTH }; }
static i2c_event_t GetSHTPPacketReceiveI2CEvent(uint8_t len) { return (i2c_event_t){ I2C_READ_REG_EVENT, active_client->header.address, NO_REG, len }; }
static i2c_event_t GetSHTPHeaderSendI2CEvent(void) { return (i2c_event_t){ I2C_WRITE_REG_EVENT, active_client->header.address, NO_REG, SHTP_HEADER_LENGTH }; }
static i2c_event_t GetSHTPPacketSendI2CEvent(uint8_t len) { return (i2c_event_t){ I2C_WRITE_REG_EVENT, active_client->header.address, NO_REG, len }; }

typedef struct
{
    void (*SetRotationVector)( uint32_t );
    void (*SetActiveClient)( shtp_client_t * );
    void (*SendPacket)(void);
    void (*SendExecutableCommand)(SHTP_EXECUTABLE_COMMAND command);
    void (*SendConfigurationReportCommandRequest)(void);
    void (*SendConfigurationFRSReadRequest)(uint16_t record_id, uint16_t read_offset, uint16_t block_size);
    void (*SendConfigurationFRSWriteDataRequest)(void);
    void (*SendConfigurationFRSWriteRequest)(void);
    void (*SendConfigurationProductIDRequest)(void);
    void (*SendFeatureCommand)( uint8_t, uint32_t, uint32_t );
    
    void (*ReceiveHeader)( shtp_packet_header_t * );
    void (*ReceivePacket)(void);
    
    void (*ParseControlReport)(void);
    void (*ParseSHTPCommandResponse)(void);
    SHTP_EXECUTABLE_RESPONSE (*ParseExecutableResponse)(void);
    void (*ParseControlResponse)(void);
    void (*ParseInputReport)(void);
    void (*ParseWakeReport)(void);
    void (*ParseRotation)(void);
    
    void (*ParseConfigurationReportCommandResponse)(sh2_command_response *, shtp_client_buffer *);
    bool (*ParseConfigurationFRSReadResponse)(void);
    bool (*ParseConfigurationFRSWriteResponse)(void);
    void (*ParseConfigurationProductIDResponse)(void);
    
    i2c_event_t (*GetHeaderReceiveEvent)(void);
    i2c_event_t (*GetPacketReceiveEvent)(uint8_t);
    i2c_event_t (*GetHeaderSendEvent)(void);
    i2c_event_t (*GetPacketSendEvent)(uint8_t);
} shtp_functions;

static shtp_functions SHTPFunctions =
{
    .SetRotationVector = SetSHTPRotationVector,
    .SendPacket = SendSHTPPacket,
    .SendFeatureCommand = SendSHTPFeatureCommand,
    .ReceiveHeader = ReceiveSHTPHeader,
    .ReceivePacket = ReceiveSHTPPacket,
    .ParseInputReport = ParseSHTPInputReport,
    .GetHeaderReceiveEvent = GetSHTPHeaderReceiveI2CEvent,
    .GetPacketReceiveEvent = GetSHTPPacketReceiveI2CEvent,
    .GetHeaderSendEvent = GetSHTPHeaderSendI2CEvent,
    .GetPacketSendEvent = GetSHTPPacketSendI2CEvent
};

void ParseAccelerometerReport(sh2_accelerometer_input_report *, shtp_client_buffer * );
void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report *, shtp_client_buffer * );
void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report *, shtp_client_buffer * );
void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report *, shtp_client_buffer * );
void ParseRotationVectorReport(sh2_rotation_vector_input_report *, shtp_client_buffer * );
void ParseGravityReport(sh2_gravity_input_report *, shtp_client_buffer * );
void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report *, shtp_client_buffer * );
void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report *, shtp_client_buffer * );
void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report *, shtp_client_buffer * );
void ParsePressureReport(sh2_pressure_input_report *, shtp_client_buffer * );
void ParseAmbientLightReport(sh2_ambient_light_input_report *, shtp_client_buffer * );
void ParseHumidityReport(sh2_humidity_input_report *, shtp_client_buffer * );
void ParseProximityReport(sh2_proximity_input_report *, shtp_client_buffer * );
void ParseTemperatureReport(sh2_temperature_input_report *, shtp_client_buffer * );
void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report *, shtp_client_buffer * );
void ParseTapDetectorReport(sh2_tap_detector_input_report *, shtp_client_buffer * );
void ParseStepCounterReport(sh2_step_counter_input_report *, shtp_client_buffer * );
void ParseSignificantMotionReport(sh2_significant_motion_input_report *, shtp_client_buffer * );
void ParseStabilityClassifierReport(sh2_stability_classifier_input_report *, shtp_client_buffer * );
void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report *, shtp_client_buffer * );
void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report *, shtp_client_buffer * );
void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report *, shtp_client_buffer * );
void ParseStepDetectorReport(sh2_step_detector_input_report *, shtp_client_buffer * );
void ParseShakeDetectorReport(sh2_shake_detector_input_report *, shtp_client_buffer * );
void ParseFlipDetectorReport(sh2_flip_detector_input_report *, shtp_client_buffer * );
void ParsePickupDetectorReport(sh2_pickup_detector_input_report *, shtp_client_buffer * );
void ParseStabilityDetectorReport(sh2_stability_detector_input_report *, shtp_client_buffer * );
void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report *, shtp_client_buffer * );
void ParseSleepDetectorReport(sh2_sleep_detector_input_report *, shtp_client_buffer * );
void ParseTiltDetectorReport(sh2_tilt_detector_input_report *, shtp_client_buffer * );
void ParsePocketDetectorReport(sh2_pocket_detector_input_report *, shtp_client_buffer * );
void ParseCircleDetectorReport(sh2_circle_detector_input_report *, shtp_client_buffer * );
void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report *, shtp_client_buffer * );
void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report *, shtp_client_buffer * );
void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report *, shtp_client_buffer * );
void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report *, shtp_client_buffer * );
void ParseFlushCompletedReport(sh2_sensor_flush *, shtp_client_buffer * );
void ParseGetFeatureResponseReport( sh2_common_dynamic_feature_report *, shtp_client_buffer * );
#endif /* shtp_h */
