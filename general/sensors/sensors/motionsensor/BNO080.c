//
//  BNO080.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "BNO080.h"

void GetSHTPHeader( shtp_packet_header_t * header )
{
    performI2CEvent(BNO080Functions.GetSHTPHeaderEvent(), i2c_data);
    header = (shtp_packet_header_t*)&i2c_data;
    header->length &= ~(1<<15);
}

void GetSHTPPacket( shtp_t * packet )
{
    GetSHTPHeader( &packet->header );
    performI2CEvent(BNO080Functions.GetSHTPPacketEvent(packet->header.length), (uint8_t*)&packet->data);
    if( packet->data[0] == SH2_BATCHING_BASE_TIMESTAMP_REFERENCE )
    {
        packet->timestamp = *(sh2_base_timestamp_reference_record*)&packet->data;
        switch(packet->header.channel)
        {
            case SHTP_COMMAND:
                break;
            case SHTP_EXECUTABLE:
                break;
            case SHTP_CONTROL:
                break;
            case SHTP_INPUT_REPORT:
                
                break;
            case SHTP_WAKE_REPORT:
                break;
            case SHTP_ROTATION:
                break;
            default:
                break;
        }
    }
    else
    {
        
    }
}

void ParseSHTPInputReport( shtp_t * packet )
{
    void * report = (sh2_report_header_t *)(&packet->data+sizeof(sh2_base_timestamp_reference_record));
    sh2_report_header_t * packet_header = (sh2_report_header_t *)report;
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
        case SH2_SENSOR_REPORT_SAR:
//            ParseSARReport(report);
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
            
//        case SH2_SENSOR_REPORT_FLUSH_COMPLETED:
//            ParseFlushCompletedReport(report);
//            break;
//        case SH2_SENSOR_REPORT_FORCE_SENSOR_FLUSH:
//            ParseForceSensorFlushReport(report);
//            break;
//
//        case SH2_SENSOR_REPORT_GET_FEATURE_RESPONSE:
//            ParseGetFeatureResponseReport(report);
//            break;
//        case SH2_SENSOR_REPORT_GET_FEATURE_COMMAND:
//            ParseGetFeatureCommandReport(report);
//            break;
//        case SH2_SENSOR_REPORT_GET_FEATURE_REQUEST:
//            ParseGetFeatureRequestReport(report);
//            break;
        default:
            break;
    }
}
void ParseAccelerometerReport(sh2_accelerometer_input_report * report )
{
    report->x;
    
}
void ParseGyroscopeCalibratedReport(sh2_gyroscope_calibrated_input_report * report )
{
    
}
void ParseMagneticFieldCalibratedReport(sh2_magnetic_field_calibrated_input_report * report )
{
    
}
void ParseLinearAccelerationReport(sh2_linear_acceleration_input_report * report )
{
    
}
void ParseRotationVectorReport(sh2_rotation_vector_input_report * report )
{
    
}
void ParseGravityReport(sh2_gravity_input_report * report )
{
    
}
void ParseGyroscopeUncalibratedReport(sh2_gyroscope_uncalibrated_input_report * report )
{
    
}
void ParseGameRotationVectorReport(sh2_game_rotation_vector_input_report * report )
{
    
}
void ParseGeomagneticRotationVectorReport(sh2_geomagnetic_rotation_vector_input_report * report )
{
    
}
void ParsePressureReport(sh2_pressure_input_report * report )
{
    
}
void ParseAmbientLightReport(sh2_ambient_light_input_report * report )
{
    
}
void ParseHumidityReport(sh2_humidity_input_report * report )
{
    
}
void ParseProximityReport(sh2_proximity_input_report * report )
{
    
}
void ParseTemperatureReport(sh2_temperature_input_report * report )
{
    
}
void ParseMagneticFieldUncalibratedReport(sh2_magnetic_field_uncalibrated_input_report * report )
{
    
}
void ParseTapDetectorReport(sh2_tap_detector_input_report * report )
{
    
}
void ParseStepCounterReport(sh2_step_counter_input_report * report )
{
    
}
void ParseSignificantMotionReport(sh2_significant_motion_input_report * report )
{
    
}
void ParseStabilityClassifierReport(sh2_stability_classifier_input_report * report )
{
    
}
void ParseRawAccelerometerReport(sh2_raw_accelerometer_input_report * report )
{
    
}
void ParseRawGyroscopeReport(sh2_raw_gyroscope_input_report * report )
{
    
}
void ParseRawMagnetometerReport(sh2_raw_magnetometer_input_report * report )
{
    
}
//void ParseSARReport(sh2_sar_input_report * report )
//{
//
//}
void ParseStepDetectorReport(sh2_step_detector_input_report * report )
{
    
}
void ParseShakeDetectorReport(sh2_shake_detector_input_report * report )
{
    
}
void ParseFlipDetectorReport(sh2_flip_detector_input_report * report )
{
    
}
void ParsePickupDetectorReport(sh2_pickup_detector_input_report * report )
{
    
}
void ParseStabilityDetectorReport(sh2_stability_detector_input_report * report )
{
    
}
void ParsePersonalActivityClassifierReport(sh2_personal_activity_classifier_input_report * report )
{
    
}
void ParseSleepDetectorReport(sh2_sleep_detector_input_report * report )
{
    
}
void ParseTiltDetectorReport(sh2_tilt_detector_input_report * report )
{
    
}
void ParsePocketDetectorReport(sh2_pocket_detector_input_report * report )
{
    
}
void ParseCircleDetectorReport(sh2_circle_detector_input_report * report )
{
    
}
void ParseHeartRateMonitorReport(sh2_heart_rate_monitor_input_report * report )
{
    
}
void ParseStabilizedRotationVectorReport(sh2_arvr_stabilized_rotation_vector_input_report * report )
{
    
}
void ParseStabilizedGameRotationVectorReport(sh2_arvr_stabilized_game_rotation_vector_input_report * report )
{
    
}
void ParseGyroIntegratedRotationVectorReport(sh2_gyro_integrated_rotation_vector_input_report * report )
{
    
}

