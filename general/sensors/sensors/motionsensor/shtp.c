//
//  shtp.c
//  sensors
//
//  Created by Matthew Fonken on 7/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "shtp.h"

static void TriggerWake(void)
{
    
}
static void SetActiveSHTPClient( shtp_client_t * client) { active_client = client; };

static void CopyAxis3ToByteArray( axis3_t * data, uint8_t * array )
{
    for( uint8_t i = 0; i < sizeof(axis3_t); i++ ) array[i] = ((uint8_t *)data)[i];
}

void PushBytesToSHTPClientBufferNext( uint8_t * data, uint16_t len, shtp_client_buffer * buffer )
{
    memcpy( SHTPClientBufferFunctions.GetNext(buffer), data, len );
    buffer->offset += len;
    buffer->length += len;
}

void PushAxis3ToSHTPClientBufferNext( axis3_t * data, shtp_client_buffer * buffer)
{
    CopyAxis3ToByteArray( data, SHTPClientBufferFunctions.GetNext(buffer) );
    buffer->offset += sizeof(axis3_t);
    buffer->length += sizeof(axis3_t);
}

uint8_t * GetSHTPClientBufferNext( shtp_client_buffer * buffer )
{
    return (((uint8_t *)&buffer->data) + buffer->offset);
}

void SetSHTPRotationVector( uint32_t interval )
{
    SendSHTPFeatureCommand( SH2_SENSOR_REPORT_ROTATION_VECTOR, interval, 0 );
}

void SendSHTPPacket(void)
{
    performI2CEvent(SHTPFunctions.GetHeaderSendEvent(), (uint8_t*)&local.header);
    performI2CEvent(SHTPFunctions.GetPacketSendEvent(local.header.length), (uint8_t*)&local.data);
}

void SendSHTPExecutableCommand(SHTP_EXECUTABLE_COMMAND command)
{
    local.header.length = 1;
    local.header.channel = SHTP_EXECUTABLE_CHANNEL;
    local.data[0] = command;
    
    SendSHTPPacket();
}

void SendSHTPConfigurationReportCommandRequest(void)
{
    local.header.length = 1;
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_command_request * request = (sh2_command_request *)local.data;
    request->report_id = SH2_CONFIGURATION_REPORT_COMMAND_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSReadRequest(uint16_t record_id, uint16_t read_offset, uint16_t block_size)
{
    local.header.length = sizeof(sh2_frs_read_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_read_request * request = (sh2_frs_read_request *)local.data;
    request->report_id = SH2_CONFIGURATION_REPORT_FRS_READ_REQUEST;
    request->reserved = 0;
    request->read_offset = read_offset;
    request->block_size = block_size;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSWriteDataRequest(void)
{
    local.header.length = 1;
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_write_data_request * request = (sh2_frs_write_data_request *)local.data;
    request->report_id = SH2_CONFIGURATION_REPORT_FRS_WRITE_DATA_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSWriteRequest(void)
{
    local.header.length = 1;
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_write_request * request = (sh2_frs_write_request *)local.data;
    request->report_id = SH2_CONFIGURATION_REPORT_FRS_WRITE_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationProductIDRequest(void)
{
    local.header.length = sizeof(sh2_product_id_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_product_id_request * request = (sh2_product_id_request*)local.data;
    request->report_id = SH2_CONFIGURATION_REPORT_PRODUCT_ID_REQUEST;
    request->reserved = 0;
    
    SendSHTPPacket();
}

void SendSHTPFeatureCommand( uint8_t report_id, uint32_t interval_ms, uint32_t specific )
{
    sh2_common_dynamic_feature_report * report = (sh2_common_dynamic_feature_report *)&local.data;
    report->report_id = SH2_SENSOR_REPORT_SET_FEATURE_COMMAND;
    report->feature_report_id = report_id;
    report->report_interval = interval_ms * 1000;
    report->sensor_specific_configuration_word = specific;

    local.header.length = sizeof(sh2_common_dynamic_feature_report);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    SendSHTPPacket();
}

void SendSHTPFeatureRequest( uint8_t feature_report_id )
{
    sh2_get_feature_request * report = (sh2_get_feature_request *)&local.data;
    report->report_id = SH2_SENSOR_REPORT_GET_FEATURE_REQUEST;
    report->feature_report_id = feature_report_id;
    
    local.header.length = sizeof(sh2_get_feature_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    SendSHTPPacket();
}

void SendSHTPForceSensorFlush( uint8_t sensor_id )
{
    sh2_sensor_flush * report = (sh2_sensor_flush *)&local.data;
    report->report_id = SH2_SENSOR_REPORT_FORCE_SENSOR_FLUSH;
    report->sensor_id = sensor_id;
    
    local.header.length = sizeof(sh2_sensor_flush);
    local.header.channel = SHTP_CONTROL_CHANNEL;
}

void GetSHTPSensorMetaDataRecord( SH2_METADATA_RECORD metadata_record_id, uint16_t read_offset, uint16_t block_size )
{
    SendSHTPConfigurationFRSReadRequest(metadata_record_id, read_offset, block_size );
}

void ReceiveSHTPHeader( shtp_packet_header_t * header )
{
    performI2CEvent(SHTPFunctions.GetHeaderReceiveEvent(), (uint8_t *)header);
    header->length &= ~(1<<15);
}

void ReceiveSHTPPacket(void)
{
    ReceiveSHTPHeader( &local.header );
    performI2CEvent(SHTPFunctions.GetPacketReceiveEvent(local.header.length), (uint8_t*)&local.data);
    switch( local.data[0] )
    {
        case SH2_BATCHING_BASE_TIMESTAMP_REFERENCE:
            local.timestamp = *(sh2_base_timestamp_reference_record*)&local.data;
            switch( local.header.channel )
            {
                case SHTP_COMMAND_CHANNEL:
                    break;
                case SHTP_EXECUTABLE_CHANNEL:
                    ParseSHTPExecutableResponse();
                    break;
                case SHTP_CONTROL_CHANNEL:
                    ParseSHTPControlReport();
                    break;
                case SHTP_WAKE_REPORT_CHANNEL:
                    TriggerWake();
                case SHTP_INPUT_REPORT_CHANNEL:
                    ParseSHTPInputReport();
                    break;
                case SHTP_ROTATION_CHANNEL:
                    break;
                default:
                    break;
            }
            break;
    }
}

void ParseSHTPControlReport(void)
{
    void * report = (sh2_report_header_t *)local.data;
    sh2_report_header_t * packet_header = (sh2_report_header_t *)report;
    shtp_client_buffer * buffer = &active_client->buffer;
    switch(packet_header->report_ID)
    {
        case SH2_CONFIGURATION_REPORT_COMMAND_RESPONSE:
            ParseSHTPConfigurationReportCommandResponse(report, buffer);
            break;
        case SH2_CONFIGURATION_REPORT_FRS_READ_RESPONSE:
            ParseSHTPConfigurationFRSReadResponse();
            break;
        case SH2_CONFIGURATION_REPORT_FRS_WRITE_RESPONSE:
            ParseSHTPConfigurationFRSWriteResponse();
            break;
        case SH2_CONFIGURATION_REPORT_PRODUCT_ID_RESPONSE:
            ParseSHTPConfigurationProductIDResponse();
            break;
        default:
            break;
    }
}

SHTP_EXECUTABLE_RESPONSE ParseSHTPExecutableResponse(void)
{
    return (SHTP_EXECUTABLE_RESPONSE)local.data[0];
}

static bool IsFRSStatusGood( uint8_t status )
{
    switch( status & SHTP_FRS_STATUS_MASK )
    {
        case SH2_FRS_STATUS_READ_RECORD_COMPLETED:
        case SH2_FRS_STATUS_READ_BLOCK_COMPLETED:
        case SH2_FRS_STATUS_READ_COMPLETED:
            return true;
        default:
            return false;
    }
}

bool ParseSHTPConfigurationFRSReadResponse(void)
{
    uint8_t meta_data_spot = 0;
    sh2_frs_read_response * response = (sh2_frs_read_response *)local.data;
    
    if(response->data0 > 0)
        local.metadata[meta_data_spot++] = response->data0;
    if(response->data1 > 1)
        local.metadata[meta_data_spot++] = response->data1;
    return IsFRSStatusGood(response->status);
}
bool ParseSHTPConfigurationFRSWriteResponse(void)
{
    sh2_frs_write_response * response = (sh2_frs_write_response *)local.data;
    return IsFRSStatusGood(response->status);
}
void ParseSHTPConfigurationProductIDResponse(void)
{
    sh2_product_id_response * response = (sh2_product_id_response *)local.data;
    active_client->product.reset_cause = response->reset_cause;
    active_client->product.sw_version_major = response->sw_version_major;
    active_client->product.sw_version_minor = response->sw_version_minor;
    active_client->product.sw_part_number = response->sw_part_number;
    active_client->product.sw_build_number = response->sw_build_number;
    active_client->product.sw_version_patch = response->sw_version_patch;
}

void ParseSHTPInputReport(void)
{
    void * report = (sh2_report_header_t *)(local.data+sizeof(sh2_base_timestamp_reference_record));
    sh2_report_header_t * packet_header = (sh2_report_header_t *)report;
    shtp_client_buffer * buffer = &active_client->buffer;
    switch(packet_header->report_ID)
    {
        case SH2_SENSOR_REPORT_ACCELEROMETER:
            ParseAccelerometerReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_CALIBRATED:
            ParseGyroscopeCalibratedReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_CALIBRATED:
            ParseMagneticFieldCalibratedReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_LINEAR_ACCELERATION:
            ParseLinearAccelerationReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_ROTATION_VECTOR:
            ParseRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GRAVITY:
            ParseGravityReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_UNCALIBRATED:
            ParseGyroscopeUncalibratedReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GAME_ROTATION_VECTOR:
            ParseGameRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GEOMAGNETIC_ROTATION_VECTOR:
            ParseGeomagneticRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_PRESSURE:
            ParsePressureReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_AMBIENT_LIGHT:
            ParseAmbientLightReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_HUMIDITY:
            ParseHumidityReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_PROXIMITY:
            ParseProximityReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_TEMPERATURE:
            ParseTemperatureReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_UNCALIBRATED:
            ParseMagneticFieldUncalibratedReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_TAP_DETECTOR:
            ParseTapDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STEP_COUNTER:
            ParseStepCounterReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_SIGNIFICANT_MOTION:
            ParseSignificantMotionReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STABILITY_CLASSIFIER:
            ParseStabilityClassifierReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_RAW_ACCELEROMETER:
            ParseRawAccelerometerReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_RAW_GYROSCOPE:
            ParseRawGyroscopeReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_RAW_MAGNETOMTER:
            ParseRawMagnetometerReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STEP_DETECTOR:
            ParseStepDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_SHAKE_DETECTOR:
            ParseShakeDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_FLIP_DETECTOR:
            ParseFlipDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_PICKUP_DETECTOR:
            ParsePickupDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STABILITY_DETECTOR:
            ParseStabilityDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_PERSONAL_ACTIVITY_CLASSIFIER:
            ParsePersonalActivityClassifierReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_SLEEP_DETECTOR:
            ParseSleepDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_TILT_DETECTOR:
            ParseTiltDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_POCKET_DETECTOR:
            ParsePocketDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_CIRCLE_DETECTOR:
            ParseCircleDetectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_HEART_RATE_MONITOR:
            ParseHeartRateMonitorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_ROTATION_VECTOR:
            ParseStabilizedRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_GAME_ROTATION_VECTOR:
            ParseStabilizedGameRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GYRO_INTEGRATED_ROTATION_VECTOR:
            ParseGyroIntegratedRotationVectorReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_FLUSH_COMPLETED:
            ParseFlushCompletedReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_GET_FEATURE_RESPONSE:
            ParseGetFeatureResponseReport(report, buffer);
            break;
        case SH2_SENSOR_REPORT_SAR:
        default:
            break;
    }
}

void ParseSHTPConfigurationReportCommandResponse(sh2_command_response * response, shtp_client_buffer * buffer)
{
    SHTPClientBufferFunctions.Push( (uint8_t *)response, sizeof(sh2_command_response), buffer );
}

inline void ParseAccelerometerReport(sh2_accelerometer_input_report * report, shtp_client_buffer * buffer )
{
    SHTPClientBufferFunctions.PushAxis3( (axis3_t *)&report->x, buffer );
}
inline void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report * report, shtp_client_buffer * buffer )
{
    SHTPClientBufferFunctions.PushAxis3( (axis3_t *)&report->x, buffer );
}
void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseRotationVectorReport(sh2_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseGravityReport(sh2_gravity_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParsePressureReport(sh2_pressure_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseAmbientLightReport(sh2_ambient_light_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseHumidityReport(sh2_humidity_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseProximityReport(sh2_proximity_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseTemperatureReport(sh2_temperature_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseTapDetectorReport(sh2_tap_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStepCounterReport(sh2_step_counter_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseSignificantMotionReport(sh2_significant_motion_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStabilityClassifierReport(sh2_stability_classifier_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStepDetectorReport(sh2_step_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseShakeDetectorReport(sh2_shake_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseFlipDetectorReport(sh2_flip_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParsePickupDetectorReport(sh2_pickup_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStabilityDetectorReport(sh2_stability_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseSleepDetectorReport(sh2_sleep_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseTiltDetectorReport(sh2_tilt_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParsePocketDetectorReport(sh2_pocket_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseCircleDetectorReport(sh2_circle_detector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report * report, shtp_client_buffer * buffer )
{
    
}
void ParseFlushCompletedReport(sh2_sensor_flush * report, shtp_client_buffer * buffer )
{
    
}
void ParseGetFeatureResponseReport( sh2_common_dynamic_feature_report * report, shtp_client_buffer * buffer )
{
    
}
