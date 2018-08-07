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

void SetSHTPActiveClient( shtp_client_t * client) { active_client = client; };

static void CopyAxis3ToByteArray( axis3_t * data, uint8_t * array )
{
    memcpy(array, (uint8_t *)data, sizeof(axis3_t));
}
static void CopyByteArrayToAxis3( uint8_t * array, axis3_t * data )
{
    memcpy((uint8_t *)data, array, sizeof(axis3_t));
    data->timestamp = timestamp();
}
static void CopyByteArrayToRotationVector( uint8_t * array, rotation_vector_t * data )
{
    memcpy((uint8_t *)data, array, sizeof(rotation_vector_t));
    data->timestamp = timestamp();
}

void ParseSHTPCommandResponse(void) {}
void ParseSHTPControlResponse(void) {}
void ParseSHTPWakeReport(void) {}
void ParseSHTPRotation(void) {}

void SendSHTPPacket(void)
{
    performCommEvent(SHTPFunctions.GetHeaderSendEvent(), (uint8_t*)&local.header);
    performCommEvent(SHTPFunctions.GetPacketSendEvent(local.header.length), (uint8_t*)&local.data);
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

bool ReceiveSHTPHeader( shtp_packet_header_t * header )
{
    performCommEvent(SHTPFunctions.GetHeaderReceiveEvent(), (uint8_t *)header);
    header->length &= ~(1<<15);
    return true;
}

bool ReceiveSHTPPacket(void)
{
    if(!ReceiveSHTPHeader( &local.header )) return false;
    performCommEvent(SHTPFunctions.GetPacketReceiveEvent(local.header.length), (uint8_t*)&local.data);
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
    void * report = (sh2_report_header_t *)local.data;
    sh2_report_header_t * packet_header = (sh2_report_header_t *)report;
    switch(packet_header->report_ID)
    {
        case SH2_CONFIGURATION_REPORT_COMMAND_RESPONSE:
            ParseSHTPConfigurationReportCommandResponse(report);
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
//    shtp_client_buffer * buffer = &active_client->buffer;
    active_client->output.type = packet_header->report_ID;
    active_client->sequence_number = packet_header->sequence_number;
    switch(packet_header->report_ID)
    {
        case SH2_SENSOR_REPORT_ACCELEROMETER:
            ParseAccelerometerReport(report);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_CALIBRATED:
            ParseGyroscopeCalibratedReport(report);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_CALIBRATED:
            ParseMagneticFieldCalibratedReport(report);
            break;
        case SH2_SENSOR_REPORT_LINEAR_ACCELERATION:
            ParseLinearAccelerationReport(report);
            break;
        case SH2_SENSOR_REPORT_ROTATION_VECTOR:
            ParseRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_GRAVITY:
            ParseGravityReport(report);
            break;
        case SH2_SENSOR_REPORT_GYROSCOPE_UNCALIBRATED:
            ParseGyroscopeUncalibratedReport(report);
            break;
        case SH2_SENSOR_REPORT_GAME_ROTATION_VECTOR:
            ParseGameRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_GEOMAGNETIC_ROTATION_VECTOR:
            ParseGeomagneticRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_PRESSURE:
            ParsePressureReport(report);
            break;
        case SH2_SENSOR_REPORT_AMBIENT_LIGHT:
            ParseAmbientLightReport(report);
            break;
        case SH2_SENSOR_REPORT_HUMIDITY:
            ParseHumidityReport(report);
            break;
        case SH2_SENSOR_REPORT_PROXIMITY:
            ParseProximityReport(report);
            break;
        case SH2_SENSOR_REPORT_TEMPERATURE:
            ParseTemperatureReport(report);
            break;
        case SH2_SENSOR_REPORT_MAGNETIC_FIELD_UNCALIBRATED:
            ParseMagneticFieldUncalibratedReport(report);
            break;
        case SH2_SENSOR_REPORT_TAP_DETECTOR:
            ParseTapDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_STEP_COUNTER:
            ParseStepCounterReport(report);
            break;
        case SH2_SENSOR_REPORT_SIGNIFICANT_MOTION:
            ParseSignificantMotionReport(report);
            break;
        case SH2_SENSOR_REPORT_STABILITY_CLASSIFIER:
            ParseStabilityClassifierReport(report);
            break;
        case SH2_SENSOR_REPORT_RAW_ACCELEROMETER:
            ParseRawAccelerometerReport(report);
            break;
        case SH2_SENSOR_REPORT_RAW_GYROSCOPE:
            ParseRawGyroscopeReport(report);
            break;
        case SH2_SENSOR_REPORT_RAW_MAGNETOMTER:
            ParseRawMagnetometerReport(report);
            break;
        case SH2_SENSOR_REPORT_STEP_DETECTOR:
            ParseStepDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_SHAKE_DETECTOR:
            ParseShakeDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_FLIP_DETECTOR:
            ParseFlipDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_PICKUP_DETECTOR:
            ParsePickupDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_STABILITY_DETECTOR:
            ParseStabilityDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_PERSONAL_ACTIVITY_CLASSIFIER:
            ParsePersonalActivityClassifierReport(report);
            break;
        case SH2_SENSOR_REPORT_SLEEP_DETECTOR:
            ParseSleepDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_TILT_DETECTOR:
            ParseTiltDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_POCKET_DETECTOR:
            ParsePocketDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_CIRCLE_DETECTOR:
            ParseCircleDetectorReport(report);
            break;
        case SH2_SENSOR_REPORT_HEART_RATE_MONITOR:
            ParseHeartRateMonitorReport(report);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_ROTATION_VECTOR:
            ParseStabilizedRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_STABILIZED_GAME_ROTATION_VECTOR:
            ParseStabilizedGameRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_GYRO_INTEGRATED_ROTATION_VECTOR:
            ParseGyroIntegratedRotationVectorReport(report);
            break;
        case SH2_SENSOR_REPORT_FLUSH_COMPLETED:
            ParseFlushCompletedReport(report);
            break;
        case SH2_SENSOR_REPORT_GET_FEATURE_RESPONSE:
            ParseGetFeatureResponseReport(report);
            break;
        case SH2_SENSOR_REPORT_SAR:
        default:
            break;
    }
    
    TriggerTick();
}

inline void ParseSHTPConfigurationReportCommandResponse(sh2_command_response * response)
{
//    SHTPClientBufferFunctions.Push( (uint8_t *)response, sizeof(sh2_command_response) );
}
inline void ParseAccelerometerReport(sh2_accelerometer_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseRotationVectorReport(sh2_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
    active_client->output.accuracy = report->accuracy;
}
inline void ParseGravityReport(sh2_gravity_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->x, &active_client->output.rotation_vector);
}
inline void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.bias3 );
}
inline void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
}
inline void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
    active_client->output.accuracy = report->accuracy;
}
inline void ParsePressureReport(sh2_pressure_input_report * report )
{
    
}
inline void ParseAmbientLightReport(sh2_ambient_light_input_report * report )
{
    
}
inline void ParseHumidityReport(sh2_humidity_input_report * report )
{
    
}
inline void ParseProximityReport(sh2_proximity_input_report * report )
{
    
}
inline void ParseTemperatureReport(sh2_temperature_input_report * report )
{
    
}
inline void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseTapDetectorReport(sh2_tap_detector_input_report * report )
{
    
}
inline void ParseStepCounterReport(sh2_step_counter_input_report * report )
{
    
}
inline void ParseSignificantMotionReport(sh2_significant_motion_input_report * report )
{
    
}
inline void ParseStabilityClassifierReport(sh2_stability_classifier_input_report * report )
{
    
}
inline void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report * report )
{
    
}
inline void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
    active_client->output.temperature = report->temperature;
}
inline void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report * report )
{
    CopyByteArrayToAxis3( (uint8_t *)&report->x, &active_client->output.axis3 );
}
inline void ParseStepDetectorReport(sh2_step_detector_input_report * report )
{
    
}
inline void ParseShakeDetectorReport(sh2_shake_detector_input_report * report )
{
    
}
inline void ParseFlipDetectorReport(sh2_flip_detector_input_report * report )
{
    
}
inline void ParsePickupDetectorReport(sh2_pickup_detector_input_report * report )
{
    
}
inline void ParseStabilityDetectorReport(sh2_stability_detector_input_report * report )
{
    
}
inline void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report * report )
{
    
}
inline void ParseSleepDetectorReport(sh2_sleep_detector_input_report * report )
{
    
}
inline void ParseTiltDetectorReport(sh2_tilt_detector_input_report * report )
{
 
}
inline void ParsePocketDetectorReport(sh2_pocket_detector_input_report * report )
{
    
}
inline void ParseCircleDetectorReport(sh2_circle_detector_input_report * report )
{
    
}
inline void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report * report )
{
    
}
inline void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
    active_client->output.accuracy = report->accuracy;
}
inline void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
}
inline void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report * report )
{
    CopyByteArrayToRotationVector((uint8_t *)&report->i, &active_client->output.rotation_vector);
    CopyByteArrayToAxis3((uint8_t *)&report->x_angular_velocity, &active_client->output.axis3);
}
inline void ParseFlushCompletedReport(sh2_sensor_flush * report )
{
//    SHTPClientBufferFunctions.Push( &report->sensor_id, 1 );
}
inline void ParseGetFeatureResponseReport( sh2_common_dynamic_feature_report * report )
{
    
}
