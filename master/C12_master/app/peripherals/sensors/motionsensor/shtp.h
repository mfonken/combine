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
#include <string.h>

#include "peripheralbridge.h"
#include "sh2.h"

#define SHTP_CLIENT_BUFFER_LEN 100

#define SHTP_HEADER_LENGTH sizeof(shtp_packet_header_t)
#define SHTP_MAX_PACKET_SIZE 128 // bytes - packets can be up to 32k but we don't have that much RAM.
#define SHTP_MAX_MEMORY_SIZE 128 // bytes
#define SHTP_MAX_METADATA_SIZE 9 // words

#define SHTP_REPORT_STATUS_MASK 0x03
#define SHTP_FRS_STATUS_MASK 0x0f

typedef struct
{
    uint16_t x, y, z;
    uint32_t timestamp;
} axis3_t;

typedef struct
{
    uint16_t i, j, k, real;
    uint32_t timestamp;
} rotation_vector_t;

//typedef comm_host_t shtp_client_comm_host;

//typedef struct
//{
//comm_protocol
//    protocol;
//uint32_t
//    device;
//} shtp_client_header;

typedef struct
{
    uint16_t length;
    uint16_t offset;
    uint8_t data[SHTP_CLIENT_BUFFER_LEN];
} shtp_client_buffer;

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
    axis3_t
        axis3, 
        bias3;
    rotation_vector_t
        rotation_vector;
    uint16_t
        accuracy,
        temperature;
    uint32_t timestamp;
    SH2_SENSOR_REPORT_ID type;
} shtp_client_output;


typedef struct
{
    shtp_client_buffer buffer;
    shtp_client_product_id product;
    shtp_client_output output;
    void * data;
    uint8_t
        ID,
        sequence_number;
} shtp_client_t;

typedef struct
{
    comm_host_t comm_host;
    shtp_client_t shtp_client;
} shtp_client_comm_host_t;

static void SHTP_GenerateSH2Client( shtp_client_comm_host_t * p_host,
                                    uint8_t ID, uint8_t sequence_number,
                                    shtp_client_buffer * p_buffer,
                                    shtp_client_product_id * p_product,
                                    shtp_client_output * p_output,
                                    void * p_data)
{
    p_host->shtp_client.ID = ID;
    p_host->shtp_client.sequence_number = sequence_number;
    memcpy( (void *)&p_host->shtp_client.buffer, p_buffer, sizeof(shtp_client_buffer) );
    memcpy( (void *)&p_host->shtp_client.product, p_product, sizeof(shtp_client_product_id) );
    memcpy( (void *)&p_host->shtp_client.output, p_output, sizeof(shtp_client_output) );
    p_host->shtp_client.data = p_data;
}

static shtp_client_comm_host_t * p_active_client;

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

void SetSHTPActiveClient( shtp_client_t * client);
void SetSHTPRotationVector( uint32_t );

void SendSHTPPacket(void);
void SendSHTPExecutableCommand(SHTP_EXECUTABLE_COMMAND command);
void SendSHTPConfigurationReportCommandRequest(void);
void SendSHTPConfigurationFRSReadRequest(uint16_t record_id, uint16_t read_offset, uint16_t block_size);
void SendSHTPConfigurationFRSWriteDataRequest(void);
void SendSHTPConfigurationFRSWriteRequest(void);
void SendSHTPConfigurationProductIDRequest(void);
void SendSHTPFeatureCommand( uint8_t, uint32_t, uint32_t );

bool ReceiveSHTPHeader( shtp_packet_header_t * );
bool ReceiveSHTPPacket(void);
void ParseSHTPControlReport(void);
void ParseSHTPCommandResponse(void);
SHTP_EXECUTABLE_RESPONSE ParseSHTPExecutableResponse(void);
void ParseSHTPControlResponse(void);
void ParseSHTPInputReport(void);
void ParseSHTPWakeReport(void);
void ParseSHTPRotation(void);
 
void ParseSHTPConfigurationReportCommandResponse(sh2_command_response *);
bool ParseSHTPConfigurationFRSReadResponse(void);
bool ParseSHTPConfigurationFRSWriteResponse(void);
void ParseSHTPConfigurationProductIDResponse(void);

static comm_event_t GetSHTPHeaderReceiveEvent(shtp_packet_header_t * h) { return (comm_event_t)(generic_comm_event_t){ &p_active_client->comm_host, COMM_READ_REG, SHTP_HEADER_LENGTH, (uint8_t *)h }; }
static comm_event_t GetSHTPPacketReceiveEvent(uint8_t l, uint8_t * d) { return (comm_event_t)(generic_comm_event_t){ &p_active_client->comm_host, COMM_READ_REG, l, d }; }
static comm_event_t GetSHTPHeaderSendEvent(shtp_packet_header_t * h) { return (comm_event_t)(generic_comm_event_t){ &p_active_client->comm_host, COMM_WRITE_REG, SHTP_HEADER_LENGTH, (uint8_t *)h }; }
static comm_event_t GetSHTPPacketSendEvent(uint8_t l, uint8_t * d) { return (comm_event_t)(generic_comm_event_t){ &p_active_client->comm_host, COMM_WRITE_REG, l, d }; }

typedef struct
{
    void (*SetActiveClient)( shtp_client_t * );
    void (*SendPacket)(void);
    void (*SendExecutableCommand)(SHTP_EXECUTABLE_COMMAND command);
    void (*SendConfigurationReportCommandRequest)(void);
    void (*SendConfigurationFRSReadRequest)(uint16_t record_id, uint16_t read_offset, uint16_t block_size);
    void (*SendConfigurationFRSWriteDataRequest)(void);
    void (*SendConfigurationFRSWriteRequest)(void);
    void (*SendConfigurationProductIDRequest)(void);
    void (*SendFeatureCommand)( uint8_t, uint32_t, uint32_t );
    
    bool (*ReceiveHeader)( shtp_packet_header_t * );
    bool (*ReceivePacket)(void);
    
    void (*ParseControlReport)(void);
    void (*ParseCommandResponse)(void);
    SHTP_EXECUTABLE_RESPONSE (*ParseExecutableResponse)(void);
    void (*ParseControlResponse)(void);
    void (*ParseInputReport)(void);
    void (*ParseWakeReport)(void);
    void (*ParseRotation)(void);
    
    void (*ParseConfigurationReportCommandResponse)(sh2_command_response *);
    bool (*ParseConfigurationFRSReadResponse)(void);
    bool (*ParseConfigurationFRSWriteResponse)(void);
    void (*ParseConfigurationProductIDResponse)(void);
    
    comm_event_t (*GetHeaderReceiveEvent)(shtp_packet_header_t *);
    comm_event_t (*GetPacketReceiveEvent)(uint8_t, uint8_t *);
    comm_event_t (*GetHeaderSendEvent)(shtp_packet_header_t *);
    comm_event_t (*GetPacketSendEvent)(uint8_t, uint8_t *);
    
    void (*GenerateClient)( shtp_client_t *,
                            uint8_t, uint8_t,
                            shtp_client_buffer *,
                            shtp_client_product_id *,
                            shtp_client_output *,
                            void * );
} shtp_functions;

static const shtp_functions SHTPFunctions =
{
    .SetActiveClient = SetSHTPActiveClient,
    .SendPacket = SendSHTPPacket,
    .SendExecutableCommand = SendSHTPExecutableCommand,
    .SendConfigurationReportCommandRequest = SendSHTPConfigurationReportCommandRequest,
    .SendConfigurationFRSReadRequest = SendSHTPConfigurationFRSReadRequest,
    .SendConfigurationFRSWriteDataRequest = SendSHTPConfigurationFRSWriteDataRequest,
    .SendConfigurationFRSWriteRequest = SendSHTPConfigurationFRSWriteRequest,
    .SendConfigurationProductIDRequest = SendSHTPConfigurationProductIDRequest,
    .SendFeatureCommand = SendSHTPFeatureCommand,
    
    .ReceiveHeader = ReceiveSHTPHeader,
    .ReceivePacket = ReceiveSHTPPacket,
    
    .ParseControlReport = ParseSHTPControlReport,
    .ParseCommandResponse = ParseSHTPCommandResponse,
    .ParseExecutableResponse = ParseSHTPExecutableResponse,
    .ParseControlResponse = ParseSHTPControlResponse,
    .ParseInputReport = ParseSHTPInputReport,
    .ParseWakeReport = ParseSHTPWakeReport,
    .ParseRotation = ParseSHTPRotation,
    
    .ParseConfigurationReportCommandResponse = ParseSHTPConfigurationReportCommandResponse,
    .ParseConfigurationFRSReadResponse = ParseSHTPConfigurationFRSReadResponse,
    .ParseConfigurationFRSWriteResponse = ParseSHTPConfigurationFRSWriteResponse,
    .ParseConfigurationProductIDResponse = ParseSHTPConfigurationProductIDResponse,
    
    .GetHeaderReceiveEvent = GetSHTPHeaderReceiveEvent,
    .GetPacketReceiveEvent = GetSHTPPacketReceiveEvent,
    .GetHeaderSendEvent = GetSHTPHeaderSendEvent,
    .GetPacketSendEvent = GetSHTPPacketSendEvent,
    
    .GenerateClient = SHTP_GenerateSH2Client
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
void ParseFlushCompletedReport(sh2_sensor_flush * );
void ParseGetFeatureResponseReport( sh2_common_dynamic_feature_report * );
#endif /* shtp_h */
