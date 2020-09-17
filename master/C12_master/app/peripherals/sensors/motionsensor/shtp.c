//
//  shtp.c
//  sensors
//
//  Created by Matthew Fonken on 7/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "shtp.h"

static void TriggerWake(void)
{}
static void TriggerTick(void)
{}

void SetSHTPActiveHost( shtp_client_comm_host_t * p_host) { p_active_host = p_host; };

static void CopyAxis3ToByteArray( axis3_t * p_data, uint8_t * p_array )
{
    memcpy(p_array, (uint8_t *)p_data, sizeof(axis3_t));
}
static void CopyByteArrayToAxis3( uint8_t * p_array, axis3_t * p_data )
{
    memcpy((uint8_t *)p_data, p_array, sizeof(axis3_t));
    p_data->timestamp = TIMESTAMP();
}
static void CopyByteArrayToRotationVector( uint8_t * p_array, rotation_vector_t * p_data )
{
    memcpy((uint8_t *)p_data, p_array, sizeof(rotation_vector_t));
    p_data->timestamp = TIMESTAMP();
}

void ParseSHTPCommandResponse(void) {}
void ParseSHTPControlResponse(void) {}
void ParseSHTPWakeReport(void) {}
void ParseSHTPRotation(void) {}

void SendSHTPPacket(void)
{
    PerformCommEvent( SHTPFunctions.GetHeaderSendEvent(&local.header) );
    PerformCommEvent(SHTPFunctions.GetPacketSendEvent(local.header.length, (uint8_t*)&local.data));
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
    
    sh2_command_request * p_request = (sh2_command_request *)local.data;
    p_request->report_id = SH2_CONFIGURATION_REPORT_COMMAND_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSReadRequest(uint16_t record_id, uint16_t read_offset, uint16_t block_size)
{
    local.header.length = sizeof(sh2_frs_read_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_read_request * p_request = (sh2_frs_read_request *)local.data;
    p_request->report_id = SH2_CONFIGURATION_REPORT_FRS_READ_REQUEST;
    p_request->reserved = 0;
    p_request->read_offset = read_offset;
    p_request->block_size = block_size;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSWriteDataRequest(void)
{
    local.header.length = 1;
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_write_data_request * p_request = (sh2_frs_write_data_request *)local.data;
    p_request->report_id = SH2_CONFIGURATION_REPORT_FRS_WRITE_DATA_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationFRSWriteRequest(void)
{
    local.header.length = 1;
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_frs_write_request * p_request = (sh2_frs_write_request *)local.data;
    p_request->report_id = SH2_CONFIGURATION_REPORT_FRS_WRITE_REQUEST;
    
    SendSHTPPacket();
}
void SendSHTPConfigurationProductIDRequest(void)
{
    local.header.length = sizeof(sh2_product_id_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    sh2_product_id_request * p_request = (sh2_product_id_request*)local.data;
    p_request->report_id = SH2_CONFIGURATION_REPORT_PRODUCT_ID_REQUEST;
    p_request->reserved = 0;
    
    SendSHTPPacket();
}

void SendSHTPFeatureCommand( uint8_t report_id, uint32_t interval_ms, uint32_t specific )
{
    sh2_common_dynamic_feature_report * p_report = (sh2_common_dynamic_feature_report *)&local.data;
    p_report->report_id = SH2_SENSOR_REPORT_SET_FEATURE_COMMAND;
    p_report->feature_report_id = report_id;
    p_report->report_interval = interval_ms * 1000;
    p_report->sensor_specific_configuration_word = specific;

    local.header.length = sizeof(sh2_common_dynamic_feature_report);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    SendSHTPPacket();
}

void SendSHTPFeatureRequest( uint8_t feature_report_id )
{
    sh2_get_feature_request * p_report = (sh2_get_feature_request *)&local.data;
    p_report->report_id = SH2_SENSOR_REPORT_GET_FEATURE_REQUEST;
    p_report->feature_report_id = feature_report_id;
    
    local.header.length = sizeof(sh2_get_feature_request);
    local.header.channel = SHTP_CONTROL_CHANNEL;
    
    SendSHTPPacket();
}

void SendSHTPForceSensorFlush( uint8_t sensor_id )
{
    sh2_sensor_flush * p_report = (sh2_sensor_flush *)&local.data;
    p_report->report_id = SH2_SENSOR_REPORT_FORCE_SENSOR_FLUSH;
    p_report->sensor_id = sensor_id;
    
    local.header.length = sizeof(sh2_sensor_flush);
    local.header.channel = SHTP_CONTROL_CHANNEL;
}

void GetSHTPSensorMetaDataRecord( SH2_METADATA_RECORD metadata_record_id, uint16_t read_offset, uint16_t block_size )
{
    SendSHTPConfigurationFRSReadRequest(metadata_record_id, read_offset, block_size );
}

bool ReceiveSHTPHeader( shtp_packet_header_t * p_header )
{
    PerformCommEvent(SHTPFunctions.GetHeaderReceiveEvent(p_header));
    p_header->length &= ~(1<<15);
    return true;
}

bool ReceiveSHTPPacket(void)
{
    if(!ReceiveSHTPHeader( &local.header )) return false;
    PerformCommEvent(SHTPFunctions.GetPacketReceiveEvent(local.header.length, local.data));
    switch( local.data[0] )
    {
        case SH2_BATCHING_BASE_TIMESTAMP_REFERENCE:
            local.timestamp = *(sh2_base_timestamp_reference_record*)&local.data;
            switch( local.header.channel )
            {
//                case SHTP_COMMAND_CHANNEL:
//                    break;
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
//                case SHTP_ROTATION_CHANNEL:
//                    break;
                default:
                    break;
            }
            break;
    }
    return true;
}

void ParseSHTPControlReport(void)
{
    void * p_report = (sh2_report_header_t *)local.data;
    sh2_report_header_t * p_packet_header = (sh2_report_header_t *)p_report;
    switch(p_packet_header->report_ID)
    {
        case SH2_CONFIGURATION_REPORT_COMMAND_RESPONSE:
            ParseSHTPConfigurationReportCommandResponse(p_report);
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
    sh2_frs_read_response * p_response = (sh2_frs_read_response *)local.data;
    
    if(p_response->data0 > 0)
        local.metadata[meta_data_spot++] = p_response->data0;
    if(p_response->data1 > 1)
        local.metadata[meta_data_spot++] = p_response->data1;
    return IsFRSStatusGood(p_response->status);
}
bool ParseSHTPConfigurationFRSWriteResponse(void)
{
    sh2_frs_write_response * p_response = (sh2_frs_write_response *)local.data;
    return IsFRSStatusGood(p_response->status);
}
void ParseSHTPConfigurationProductIDResponse(void)
{
    sh2_product_id_response * response = (sh2_product_id_response *)local.data;
    p_active_host->shtp_client.product.reset_cause = response->reset_cause;
    p_active_host->shtp_client.product.sw_version_major = response->sw_version_major;
    p_active_host->shtp_client.product.sw_version_minor = response->sw_version_minor;
    p_active_host->shtp_client.product.sw_part_number = response->sw_part_number;
    p_active_host->shtp_client.product.sw_build_number = response->sw_build_number;
    p_active_host->shtp_client.product.sw_version_patch = response->sw_version_patch;
}

void ParseSHTPInputReport(void)
{
    void * p_report = (sh2_report_header_t *)(local.data+sizeof(sh2_base_timestamp_reference_record));
    sh2_report_header_t * p_packet_header = (sh2_report_header_t *)p_report;
//    shtp_client_buffer * p_buffer = &p_active_host->shtp_client.buffer;
    p_active_host->shtp_client.output.type = p_packet_header->report_ID;
    p_active_host->shtp_client.sequence_number = p_packet_header->sequence_number;
    switch(p_packet_header->report_ID)
    {
        case SH2_SENSOR_REPORT_ACCELEROMETER:
            ParseAccelerometerReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_CALIBRATED:
            ParseGyroscopeCalibratedReport(p_report);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_CALIBRATED:
            ParseMagneticFieldCalibratedReport(p_report);
            break;
        case SH2_SENSOR_REPORT_LINEAR_ACCELERATION:
            ParseLinearAccelerationReport(p_report);
            break;
        case SH2_SENSOR_REPORT_ROTATION_VECTOR:
            ParseRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GRAVITY:
            ParseGravityReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_UNCALIBRATED:
            ParseGyroscopeUncalibratedReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GAME_ROTATION_VECTOR:
            ParseGameRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GEOMAGNETIC_ROTATION_VECTOR:
            ParseGeomagneticRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_PRESSURE:
            ParsePressureReport(p_report);
            break;
        case SH2_SENSOR_REPORT_AMBIENT_LIGHT:
            ParseAmbientLightReport(p_report);
            break;
        case SH2_SENSOR_REPORT_HUMIDITY:
            ParseHumidityReport(p_report);
            break;
        case SH2_SENSOR_REPORT_PROXIMITY:
            ParseProximityReport(p_report);
            break;
        case SH2_SENSOR_REPORT_TEMPERATURE:
            ParseTemperatureReport(p_report);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_UNCALIBRATED:
            ParseMagneticFieldUncalibratedReport(p_report);
            break;
        case SH2_SENSOR_REPORT_TAP_DETECTOR:
            ParseTapDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STEP_COUNTER:
            ParseStepCounterReport(p_report);
            break;
        case SH2_SENSOR_REPORT_SIGNIFICANT_MOTION:
            ParseSignificantMotionReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STABILITY_CLASSIFIER:
            ParseStabilityClassifierReport(p_report);
            break;
        case SH2_SENSOR_REPORT_RAW_ACCELEROMETER:
            ParseRawAccelerometerReport(p_report);
            break;
        case SH2_SENSOR_REPORT_RAW_GYROSCOPE:
            ParseRawGyroscopeReport(p_report);
            break;
        case SH2_SENSOR_REPORT_RAW_MAGNETOMTER:
            ParseRawMagnetometerReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STEP_DETECTOR:
            ParseStepDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_SHAKE_DETECTOR:
            ParseShakeDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_FLIP_DETECTOR:
            ParseFlipDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_PICKUP_DETECTOR:
            ParsePickupDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STABILITY_DETECTOR:
            ParseStabilityDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_PERSONAL_ACTIVITY_CLASSIFIER:
            ParsePersonalActivityClassifierReport(p_report);
            break;
        case SH2_SENSOR_REPORT_SLEEP_DETECTOR:
            ParseSleepDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_TILT_DETECTOR:
            ParseTiltDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_POCKET_DETECTOR:
            ParsePocketDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_CIRCLE_DETECTOR:
            ParseCircleDetectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_HEART_RATE_MONITOR:
            ParseHeartRateMonitorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_ROTATION_VECTOR:
            ParseStabilizedRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_GAME_ROTATION_VECTOR:
            ParseStabilizedGameRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GYRO_INTEGRATED_ROTATION_VECTOR:
            ParseGyroIntegratedRotationVectorReport(p_report);
            break;
        case SH2_SENSOR_REPORT_FLUSH_COMPLETED:
            ParseFlushCompletedReport(p_report);
            break;
        case SH2_SENSOR_REPORT_GET_FEATURE_RESPONSE:
            ParseGetFeatureResponseReport(p_report);
            break;
        case SH2_SENSOR_REPORT_SAR:
        default:
            break;
    }
    
    TriggerTick();
}

inline void ParseSHTPConfigurationReportCommandResponse(sh2_command_response * p_response)
{
//    SHTPClientBufferFunctions.Push( (uint8_t *)p_response, sizeof(sh2_command_response) );
}
inline void ParseAccelerometerReport(sh2_accelerometer_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseRotationVectorReport(sh2_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
    p_active_host->shtp_client.output.accuracy = p_report->accuracy;
}
inline void ParseGravityReport(sh2_gravity_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->x, &p_active_host->shtp_client.output.rotation_vector);
}
inline void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.bias3 );
}
inline void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
}
inline void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
    p_active_host->shtp_client.output.accuracy = p_report->accuracy;
}
inline void ParsePressureReport(sh2_pressure_input_report * p_report )
{
    
}
inline void ParseAmbientLightReport(sh2_ambient_light_input_report * p_report )
{
    
}
inline void ParseHumidityReport(sh2_humidity_input_report * p_report )
{
    
}
inline void ParseProximityReport(sh2_proximity_input_report * p_report )
{
    
}
inline void ParseTemperatureReport(sh2_temperature_input_report * p_report )
{
    
}
inline void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseTapDetectorReport(sh2_tap_detector_input_report * p_report )
{
    
}
inline void ParseStepCounterReport(sh2_step_counter_input_report * p_report )
{
    
}
inline void ParseSignificantMotionReport(sh2_significant_motion_input_report * p_report )
{
    
}
inline void ParseStabilityClassifierReport(sh2_stability_classifier_input_report * p_report )
{
    
}
inline void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report * p_report )
{
    
}
inline void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
    p_active_host->shtp_client.output.temperature = p_report->temperature;
}
inline void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report * p_report )
{
    CopyByteArrayToAxis3( (uint8_t *)&p_report->x, &p_active_host->shtp_client.output.axis3 );
}
inline void ParseStepDetectorReport(sh2_step_detector_input_report * p_report )
{
    
}
inline void ParseShakeDetectorReport(sh2_shake_detector_input_report * p_report )
{
    
}
inline void ParseFlipDetectorReport(sh2_flip_detector_input_report * p_report )
{
    
}
inline void ParsePickupDetectorReport(sh2_pickup_detector_input_report * p_report )
{
    
}
inline void ParseStabilityDetectorReport(sh2_stability_detector_input_report * p_report )
{
    
}
inline void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report * p_report )
{
    
}
inline void ParseSleepDetectorReport(sh2_sleep_detector_input_report * p_report )
{
    
}
inline void ParseTiltDetectorReport(sh2_tilt_detector_input_report * p_report )
{
 
}
inline void ParsePocketDetectorReport(sh2_pocket_detector_input_report * p_report )
{
    
}
inline void ParseCircleDetectorReport(sh2_circle_detector_input_report * p_report )
{
    
}
inline void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report * p_report )
{
    
}
inline void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
    p_active_host->shtp_client.output.accuracy = p_report->accuracy;
}
inline void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
}
inline void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report * p_report )
{
    CopyByteArrayToRotationVector((uint8_t *)&p_report->i, &p_active_host->shtp_client.output.rotation_vector);
    CopyByteArrayToAxis3((uint8_t *)&p_report->x_angular_velocity, &p_active_host->shtp_client.output.axis3);
}
inline void ParseFlushCompletedReport(sh2_sensor_flush * p_report )
{
//    SHTPClientBufferFunctions.Push( &p_report->sensor_id, 1 );
}
inline void ParseGetFeatureResponseReport( sh2_common_dynamic_feature_report * p_report )
{
    
}
