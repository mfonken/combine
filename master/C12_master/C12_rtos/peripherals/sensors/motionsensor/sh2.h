/* File: sh2.h
 * Author: Matthew Fonken - mfonken@marbl.co
 * Source: https://www.hillcrestlabs.com/download/5a57cc8a566d07c5e0000008
 * Description:
 The SH-2 is Hillcrest’s turnkey sensor hub software solution. The SH-2 connects to various motion and environmental sensors, collects data from them, processes that data and provides the results to a host processor.
 */

#ifndef sh2_h
#define sh2_h

/* Records start */
typedef enum
{
    SH2_CONFIGURATION_RECORD_STATIC_CALIBRATION_AGM                          = 0x7979,
    SH2_CONFIGURATION_RECORD_NOMINAL_CALIBRATION_AGM                         = 0x4D4D,
    SH2_CONFIGURATION_RECORD_STATIC_CALIBRATION_SRA                          = 0x8A8A,
    SH2_CONFIGURATION_RECORD_NOMINAL_CALIBRATION_SRA                         = 0x4E4E,
    SH2_CONFIGURATION_RECORD_DYNAMIC_CALIBRATION                             = 0x1F1F,
    SH2_CONFIGURATION_RECORD_MOTIONENGINE_POWER_MANAGEMENT                   = 0xD3E2,
    SH2_CONFIGURATION_RECORD_SYSTEM_ORIENTATION                              = 0x2D3E,
    SH2_CONFIGURATION_RECORD_PRIMARY_ACCELEROMETER_ORIENTATION               = 0x2D41,
    SH2_CONFIGURATION_RECORD_GYROSCOPE_ORIENTATION                           = 0x2D46,
    SH2_CONFIGURATION_RECORD_MAGNETOMETER_ORIENTATION                        = 0x2D4C,
    SH2_CONFIGURATION_RECORD_STABILIZATION_ROTATION_VECTOR                   = 0x3E2D,
    SH2_CONFIGURATION_RECORD_STABILIZATION_GAME_ROTATION_VECTOR              = 0x3E2E,
    SH2_CONFIGURATION_RECORD_SIGNIFICANT_MOTION_DETECTOR_CONFIGURATION       = 0xC274,
    SH2_CONFIGURATION_RECORD_SHAKE_DETECTOR_CONFIGURATION                    = 0x7D7D,
    SH2_CONFIGURATION_RECORD_MAxIMUM_FUSION_PERIOD                           = 0xD7D7,
    SH2_CONFIGURATION_RECORD_SERIAL_NUMBER                                   = 0x4B4B,
    SH2_CONFIGURATION_RECORD_ENVIRONMENTAL_SENSOR_PRESSURE_CALIBRATION       = 0x39AF,
    SH2_CONFIGURATION_RECORD_ENVIRONMENTAL_SENSOR_TEMPERATURE_CALIBRATION    = 0x4D20,
    SH2_CONFIGURATION_RECORD_ENVIRONMENTAL_SENSOR_HUMIDITY_CALIBRATION       = 0x1AC9,
    SH2_CONFIGURATION_RECORD_ENVIRONMENTAL_SENSOR_AMBIENT_LIGHT_CALIBRATION  = 0x39B1,
    SH2_CONFIGURATION_RECORD_ENVIRONMENTAL_SENSOR_PROXIMITY_CALIBRATION      = 0x4DA2,
    SH2_CONFIGURATION_RECORD_ALS_CALIBRATION                                 = 0xD401,
    SH2_CONFIGURATION_RECORD_PROXIMITY_SENSOR_CALIBRATION                    = 0xD402,
    SH2_CONFIGURATION_RECORD_STABILITY_DETECTOR_CONFIGURATION                = 0xED85,
    SH2_CONFIGURATION_RECORD_USER_RECORD                                     = 0x74B4,
    SH2_CONFIGURATION_RECORD_MOTIONENGINE_TIME_SOURCE_SELECTION              = 0xD403,
    SH2_CONFIGURATION_RECORD_GYRO_INTEGRATED_ROTATION_VECTOR_CONFIGURATION   = 0xA1A2
} SH2_CONFIGURATION_RECORD;

typedef enum
{
    SH2_METADATA_RECORD_RAW_ACCELEROMETER                                   = 0xE301,
    SH2_METADATA_RECORD_ACCELEROMETER                                       = 0xE302,
    SH2_METADATA_RECORD_LINEAR_ACCELERATION                                 = 0xE303,
    SH2_METADATA_RECORD_GRAVITY                                             = 0xE304,
    SH2_METADATA_RECORD_RAW_GYROSCOPE                                       = 0xE305,
    SH2_METADATA_RECORD_GYROSCOPE_CALIBRATED                                = 0xE306,
    SH2_METADATA_RECORD_GYROSCOPE_UNCALIBRATED                              = 0xE307,
    SH2_METADATA_RECORD_RAW_MAGNETOMETER                                    = 0xE308,
    SH2_METADATA_RECORD_MAGNETIC_FIELD_CALIBRATED                           = 0xE309,
    SH2_METADATA_RECORD_MAGNETIC_FIELD_UNCALIBRATED                         = 0xE30A,
    SH2_METADATA_RECORD_ROTATION_VECTOR                                     = 0xE30B,
    SH2_METADATA_RECORD_GAME_ROTATION_VECTOR                                = 0xE30C,
    SH2_METADATA_RECORD_GEOMAGNETIC_ROTATION_VECTOR                         = 0xE30D,
    SH2_METADATA_RECORD_PRESSURE                                            = 0xE30E,
    SH2_METADATA_RECORD_AMBIENT_LIGHT                                       = 0xE30F,
    SH2_METADATA_RECORD_HUMIDITY                                            = 0xE310,
    SH2_METADATA_RECORD_PROXIMITY                                           = 0xE311,
    SH2_METADATA_RECORD_TEMPERATURE                                         = 0xE312,
    SH2_METADATA_RECORD_TAP_DETECTOR                                        = 0xE313,
    SH2_METADATA_RECORD_STEP_DETECTOR                                       = 0xE314,
    SH2_METADATA_RECORD_STEP_COUNTER                                        = 0xE315,
    SH2_METADATA_RECORD_SIGNIFICANT_MOTION                                  = 0xE316,
    SH2_METADATA_RECORD_STABILITY_CLASSIFIER                                = 0xE317,
    SH2_METADATA_RECORD_SHAKE_DETECTOR                                      = 0xE318,
    SH2_METADATA_RECORD_FLIP_DETECTOR                                       = 0xE319,
    SH2_METADATA_RECORD_PICKUP_DETECTOR                                     = 0xE31A,
    SH2_METADATA_RECORD_STABILITY_DETECTOR                                  = 0xE31B,
    SH2_METADATA_RECORD_PERSONAL_ACTIVITY_CLASSIFIER                        = 0xE31C,
    SH2_METADATA_RECORD_SLEEP_DETECTOR                                      = 0xE31D,
    SH2_METADATA_RECORD_TILT_DETECTOR                                       = 0xE31E,
    SH2_METADATA_RECORD_POCKET_DETECTOR                                     = 0xE31F,
    SH2_METADATA_RECORD_CIRCLE_DETECTOR                                     = 0xE320,
    SH2_METADATA_RECORD_HEART_RATE_MONITOR                                  = 0xE321,
    SH2_METADATA_RECORD_ARVR_STABILIZED_ROTATION_VECTOR                     = 0xE322,
    SH2_METADATA_RECORD_ARVR_STABILIZED_GAME_ROTATION_VECTOR                = 0xE323,
    SH2_METADATA_RECORD_GYRO_INTEGRATED_ROTATION_VECTOR                     = 0xE324,
} SH2_METADATA_RECORD;
/* Records end */

/* Configuration reports start */
typedef enum
{
    SH2_CONFIGURATION_REPORT_COMMAND_RESPONSE               = 0xF1,
    SH2_CONFIGURATION_REPORT_COMMAND_REQUEST                = 0xF2,
    SH2_CONFIGURATION_REPORT_FRS_READ_RESPONSE              = 0xF3,
    SH2_CONFIGURATION_REPORT_FRS_READ_REQUEST               = 0xF4,
    SH2_CONFIGURATION_REPORT_FRS_WRITE_RESPONSE             = 0xF5,
    SH2_CONFIGURATION_REPORT_FRS_WRITE_DATA_REQUEST         = 0xF6,
    SH2_CONFIGURATION_REPORT_FRS_WRITE_REQUEST              = 0xF7,
    SH2_CONFIGURATION_REPORT_PRODUCT_ID_RESPONSE            = 0xF8,
    SH2_CONFIGURATION_REPORT_PRODUCT_ID_REQUEST             = 0xF9
} SH2_CONFIGURATION_REPORT_ID;

typedef struct /* 0xF1 */
{
    uint8_t
    report_id,
    sequence_number,
    command,
    command_sequence_number,
    response_sequence_number,
    R[11];
} sh2_command_response;
typedef struct /* 0xF2 */
{
    uint8_t
    report_id,
    sequence_number,
    command,
    P[9];
} sh2_command_request;
typedef struct /* 0xF3 */
{
    uint8_t
    report_id,
    data_length:4,
    status:4;
    uint16_t
    word_offset;
    uint32_t
    data0,
    data1;
    uint16_t
    frs_type,
    reserved;
} sh2_frs_read_response;
typedef struct /* 0xF4 */
{
    uint8_t
    report_id,
    reserved;
    uint16_t
    read_offset,
    frs_type,
    block_size;
} sh2_frs_read_request;
typedef struct /* 0xF5 */
{
    uint8_t
    report_id,
    status;
    uint16_t
    word_offset;
} sh2_frs_write_response;
typedef struct /* 0xF6 */
{
    uint8_t
    report_id,
    reserved;
    uint16_t
    offset;
    uint32_t
    data0,
    data1;
} sh2_frs_write_data_request;
typedef struct /* 0xF7 */
{
    uint8_t
    report_id,
    reserved;
    uint16_t
    length,
    frs_type;
} sh2_frs_write_request;

typedef enum
{
    SH2_FRS_STATUS_NO_ERROR = 0,
    SH2_FRS_STATUS_UNRECOGNIZED_FRS_TYPE,
    SH2_FRS_STATUS_BUSY,
    SH2_FRS_STATUS_READ_RECORD_COMPLETED,
    SH2_FRS_STATUS_OFFSET_OUT_OF_RANGE,
    SH2_FRS_STATUS_RECORD_EMPTY,
    SH2_FRS_STATUS_READ_BLOCK_COMPLETED,    // if block size requested
    SH2_FRS_STATUS_READ_COMPLETED,          // if block size requested
    SH2_FRS_STATUS_DEVICE_ERROR
} SH2_FRS_STATUS;

typedef enum
{
    SH2_RESET_CAUSE_NOT_APPLICABLE                          = 0x00,
    SH2_RESET_CAUSE_POWER_ON_RESET                          = 0x01,
    SH2_RESET_CAUSE_INTERNAL_SYSTEM_RESET                   = 0x02,
    SH2_RESET_CAUSE_WATCHDOG_TIMEOUT                        = 0x03,
    SH2_RESET_CAUSE_EXTERNAL_RESET                          = 0x04,
    SH2_RESET_CAUSE_OTHER                                   = 0x05
} SH2_RESET_CAUSE;
typedef struct /* 0xF8 */
{
    uint8_t
    report_id,
    reset_cause,
    sw_version_major,
    sw_version_minor;
    uint32_t
    sw_part_number,
    sw_build_number;
    uint16_t
    sw_version_patch,
    reserved;
} sh2_product_id_response;
typedef struct /* 0xF9 */
{
    uint8_t
    report_id,
    reserved;
} sh2_product_id_request;
/* Configuration reports end */

/* Commands start */
typedef enum
{
    SH2_COMMAND_REPORT_ERRORS                               = 0x01,
    SH2_COMMAND_COUNTER                                     = 0x02,
    SH2_COMMAND_TARE                                        = 0x03,
    SH2_COMMAND_INITIALIZATION                              = 0x04,
    SH2_COMMAND_SAVE_DCD                                    = 0x06,
    SH2_COMMAND_ME_CALIBRATION                              = 0x07,
    SH2_COMMAND_CONFIGURE_PERIODIC_DCD_SAVE                 = 0x09,
    SH2_COMMAND_GET_OSCILLATOR_TYPE                         = 0x0A,
    SH2_COMMAND_CLEAR_DCD_AND_RESET                         = 0x0B
} SH2_COMMAND_ID;

typedef enum
{
    SH2_COUNTER_COMMAND_GET_COUNTS                          = 0x00,
    SH2_COUNTER_COMMAND_CLEAR_COUNTS                        = 0x01
} SH2_COUNTER_COMMAND_ID;
typedef enum
{
    SH2_COUNTER_COMMAND_TARE_NOW                            = 0x00,
    SH2_COUNTER_COMMAND_PERSIST_TARE                        = 0x01,
    SH2_COUNTER_COMMAND_SET_ORIENTATION                     = 0x02
} SH2_TARE_COMMAND_ID;
/* Commands end */

typedef enum
{
    SH2_SENSOR_STATUS_UNRELIABLE = 0x00,
    SH2_SENSOR_STATUS_ACCURACTY_LOW = 0x01,
    SH2_SENSOR_STATUS_ACCURACTY_MEDIUM = 0x02,
    SH2_SENSOR_STATUS_ACCURACTY_HIGH = 0x03,
} SH2_SENSOR_STATUS;

typedef struct
{
 uint16_t
    accuracy:2,
    delay:14;                       // 6 most-significant bits of report delay. See blow.
} sh2_sensor_status;

/* Generic report types start */
typedef struct
{
    uint8_t
    report_id,
    sequence_number;
    uint32_t
    report_interval,
    batch_interval;
    struct
    {
        uint8_t
    change_sensitivity_type:1,      // 0 – absolute, 1 – relative
    change_sensitivity_enable:1,    // 0 – disabled, 1 – enabled
    wake_up_enable:1,               // 0 – disabled, 1 – enabled
    always_on_enable:1,             // 0 – disabled, 1 - enabled(run sensor while hub is in “sleep” mode)
    reserved:4;
    } feature_flags;
    uint16_t
    change_sensitivity_absolute,
    change_sensitivity_relative;
    uint32_t
    sensor_specific_configuration_word;
    sh2_sensor_status status;
} sh2_common_fields;

typedef struct
{
    uint8_t
    report_id,
    feature_report_id,
    feature_flags;
    uint16_t
    change_sensitivity;
    uint32_t
    report_interval,
    batch_interval,
    sensor_specific_configuration_word;
} sh2_common_dynamic_feature_report;

typedef struct
{
uint8_t
    report_ID,
    sequence_number,
    status,
    delay;
} sh2_report_header_t;

typedef struct
{
    sh2_report_header_t header;
    uint16_t
    x,
    y,
    z;
} sh2_generic_triple_axis_report;
/* Generic report types end */

/* Input report types start */
typedef enum
{
    SH2_SENSOR_REPORT_ACCELEROMETER                         = 0x01,
    SH2_SENSOR_REPORT_GYROSCOPE_CALIBRATED                  = 0x02,
    SH2_SENSOR_REPORT_MAGNETIC_FIELD_CALIBRATED             = 0x03,
    SH2_SENSOR_REPORT_LINEAR_ACCELERATION                   = 0x04,
    SH2_SENSOR_REPORT_ROTATION_VECTOR                       = 0x05,
    SH2_SENSOR_REPORT_GRAVITY                               = 0x06,
    SH2_SENSOR_REPORT_GYROSCOPE_UNCALIBRATED                = 0x07,
    SH2_SENSOR_REPORT_GAME_ROTATION_VECTOR                  = 0x08,
    SH2_SENSOR_REPORT_GEOMAGNETIC_ROTATION_VECTOR           = 0x09,
    SH2_SENSOR_REPORT_PRESSURE                              = 0x0A,
    SH2_SENSOR_REPORT_AMBIENT_LIGHT                         = 0x0B,
    SH2_SENSOR_REPORT_HUMIDITY                              = 0x0C,
    SH2_SENSOR_REPORT_PROXIMITY                             = 0x0D,
    SH2_SENSOR_REPORT_TEMPERATURE                           = 0x0E,
    SH2_SENSOR_REPORT_MAGNETIC_FIELD_UNCALIBRATED           = 0x0F,
    SH2_SENSOR_REPORT_TAP_DETECTOR                          = 0x10,
    SH2_SENSOR_REPORT_STEP_COUNTER                          = 0x11,
    SH2_SENSOR_REPORT_SIGNIFICANT_MOTION                    = 0x12,
    SH2_SENSOR_REPORT_STABILITY_CLASSIFIER                  = 0x13,
    SH2_SENSOR_REPORT_RAW_ACCELEROMETER                     = 0x14,
    SH2_SENSOR_REPORT_RAW_GYROSCOPE                         = 0x15,
    SH2_SENSOR_REPORT_RAW_MAGNETOMTER                       = 0x16,
    SH2_SENSOR_REPORT_SAR                                   = 0x17,
    SH2_SENSOR_REPORT_STEP_DETECTOR                         = 0x18,
    SH2_SENSOR_REPORT_SHAKE_DETECTOR                        = 0x19,
    SH2_SENSOR_REPORT_FLIP_DETECTOR                         = 0x1A,
    SH2_SENSOR_REPORT_PICKUP_DETECTOR                       = 0x1B,
    SH2_SENSOR_REPORT_STABILITY_DETECTOR                    = 0x1C,
    SH2_SENSOR_REPORT_PERSONAL_ACTIVITY_CLASSIFIER          = 0x1E,
    SH2_SENSOR_REPORT_SLEEP_DETECTOR                        = 0x1F,
    SH2_SENSOR_REPORT_TILT_DETECTOR                         = 0x20,
    SH2_SENSOR_REPORT_POCKET_DETECTOR                       = 0x21,
    SH2_SENSOR_REPORT_CIRCLE_DETECTOR                       = 0x22,
    SH2_SENSOR_REPORT_HEART_RATE_MONITOR                    = 0x23,
    
    SH2_SENSOR_REPORT_STABILIZED_ROTATION_VECTOR            = 0x28,
    SH2_SENSOR_REPORT_STABILIZED_GAME_ROTATION_VECTOR       = 0x29,
    SH2_SENSOR_REPORT_GYRO_INTEGRATED_ROTATION_VECTOR       = 0x2A,
    
    SH2_SENSOR_REPORT_FLUSH_COMPLETED                       = 0xEF,
    SH2_SENSOR_REPORT_FORCE_SENSOR_FLUSH                    = 0xF0,
    
    SH2_SENSOR_REPORT_GET_FEATURE_RESPONSE                  = 0xFC,
    SH2_SENSOR_REPORT_SET_FEATURE_COMMAND                   = 0xFD,
    SH2_SENSOR_REPORT_GET_FEATURE_REQUEST                   = 0xFE,
} SH2_SENSOR_REPORT_ID;

typedef struct
{
uint8_t
    report_id,
    feature_report_id;
} sh2_get_feature_request;

typedef struct
{
uint8_t
    report_id,
    sensor_id;
} sh2_sensor_flush;

typedef sh2_generic_triple_axis_report
sh2_accelerometer_input_report,             /* 0x01 */
sh2_linear_acceleration_input_report,       /* 0x04 */
sh2_gravity_input_report,                   /* 0x06 */
sh2_gyroscope_calibrated_input_report,      /* 0x02 */
sh2_magnetic_field_calibrated_input_report;  /* 0x03 */

typedef struct                                  /* 0x14 */
{
    sh2_report_header_t header;
    uint16_t
    x,
    y,
    z,
    reserved;
    uint32_t
    timestamp;
} sh2_raw_accelerometer_input_report,
sh2_raw_magnetometer_input_report;          /* 0x16 */

typedef struct                                  /* 0x15 */
{
    sh2_report_header_t header;
    uint16_t
    x,
    y,
    z,
    temperature;
    uint32_t
    timestamp;
} sh2_raw_gyroscope_input_report;

typedef struct                                  /* 0x07 */
{
    sh2_report_header_t header;
    uint16_t
    x,
    y,
    z,
    x_bias,
    y_bias,
    z_bias;
} sh2_gyroscope_uncalibrated_input_report,
    sh2_magnetic_field_uncalibrated_input_report;
typedef struct
{
    sh2_report_header_t header;
    uint16_t
    i,
    j,
    k,
    real,
    accuracy;
} sh2_rotation_vector_input_report,             /* 0x05 */
sh2_geomagnetic_rotation_vector_input_report,    /* 0x09 */
sh2_arvr_stabilized_rotation_vector_input_report; /* 0x28 */

typedef struct
{
    sh2_report_header_t header;
    uint16_t
    i,
    j,
    k,
    real;
} sh2_game_rotation_vector_input_report,        /* 0x08 */
sh2_arvr_stabilized_game_rotation_vector_input_report; /* 0x29 */

typedef struct
{
    sh2_report_header_t header;
    uint16_t
    i,
    j,
    k,
    real,
    x_angular_velocity,
    y_angular_velocity,
    z_angular_velocity;
} sh2_gyro_integrated_rotation_vector_input_report; /* 0x2A */

typedef struct
{
    sh2_report_header_t header;
    union
    {
        uint32_t
        atmospheric_pressure:32,                /* 0x0A */
        ambient_light:32,                       /* 0x0B */
        humidity:16,                            /* 0x0C */
        distance:16,                            /* 0x0D */
        temperature:16,                         /* 0x0E */
        taps_detected:8,                        /* 0x10 */
        detect_latency:8,                       /* 0x18 */
        motion:16;                              /* 0x12 */
        struct                                  /* 0x13 */
        {
            uint8_t
            stability_classification,
            reserved;
        } stability_classifier_data;
        uint32_t
        shake:16,                               /* 0x19 */
        flip:16,                                /* 0x1A */
        pickup:16,                              /* 0x1B */
        stability:16;                           /* 0x1C */
        struct                                  /* 0x1F */
        {
            uint8_t
            sleep_state,
            reserved;
        } sleep_detector_data;
        uint32_t
        tilt:16,                                /* 0x20 */
        pocket:16,                              /* 0x21 */
        circle:16,                              /* 0x22 */
        heart_rate:16;                          /* 0x23 */
    } sensor_data;
} sh2_pressure_input_report,
sh2_ambient_light_input_report,
sh2_humidity_input_report,
sh2_proximity_input_report,
sh2_temperature_input_report,
sh2_tap_detector_input_report,
sh2_step_detector_input_report,
sh2_significant_motion_input_report,
sh2_stability_classifier_input_report,
sh2_shake_detector_input_report,
sh2_flip_detector_input_report,
sh2_pickup_detector_input_report,
sh2_stability_detector_input_report,
sh2_sleep_detector_input_report,
sh2_tilt_detector_input_report,
sh2_pocket_detector_input_report,
sh2_circle_detector_input_report,
sh2_heart_rate_monitor_input_report;

typedef struct
{
    sh2_report_header_t header;
    uint32_t
    detect_latency;
    uint16_t
    steps,
    reserved;
} sh2_step_counter_input_report;                /* 0x11 */

typedef enum
{
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_UNKNOWN      = 0x00,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_IN_VEHICLE   = 0x01,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_ON_BICYCLE   = 0x02,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_ON_FOOT      = 0x03,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_STILL        = 0x04,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_TILTING      = 0x05,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_WALKING      = 0x06,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_RUNNING      = 0x07,
    SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE_ON_STAIRS    = 0x08
} SH2_PERSONAL_ACTIVITY_CLASSIFICATION_STATE;
typedef struct
{
    sh2_report_header_t header;
    uint8_t
    page_number_and_eos,
    most_likely_state,
    classification[10];
} sh2_personal_activity_classifier_input_report; /* 0x1E */
/* Input report types end */

/* Batching start */
typedef enum
{
    SH2_BATCHING_TIMESTAMP_REBASE                           = 0xFA,
    SH2_BATCHING_BASE_TIMESTAMP_REFERENCE                   = 0xFB
} SH2_BATCHING_ID;

typedef struct
{
    uint8_t
    report_id;
    union
    {
        uint32_t
        base_delta,     // relative to transport-defined reference point. Signed. Units are 100 microsecond ticks.
        rebase_delta;   // relative to Base Timestamp Reference. Signed. Units are 100 microsecond ticks.
    } batching_data;
    
} sh2_base_timestamp_reference_record,          /* 0xFB */
sh2_timestamp_rebase_record;                /* 0xFA */
/* Batching end */



#endif /* sh2_h */

