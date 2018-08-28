//
//  systemtypes.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef system_types_h
#define system_types_h

#include <stdint.h>
#include "taumanager.h"
#include "communicationmanager.h"
#include "motionsensor.h"
#include "rho_client.h"
#include "batterymonitor.h"
#include "touchcontroller.h"

#define MAX_ROUTINES 45
#define MAX_SUBACTIVITIES_PER_ACTIVITY 20
#define MAX_TASKS 10
#define MAX_INTERRUPTS MAX_TASKS/2
#define MAX_SCHEDULED MAX_TASKS/2
#define MAX_TASK_SHELF_ENTRIES 20
#define MAX_STATE_PROFILE_ENTRIES 10

#define COMPONENT_ID(A,B) ( ( A << 8 ) & 0xf0 | ( B & 0x0f ) )

/************************************************************************************/
/***                               Enums Start                                    ***/
/************************************************************************************/
typedef enum
{
    SYSTEM_STATE_STARTUP = 0,
    SYSTEM_STATE_IDLE,
    SYSTEM_STATE_WAITING,
    SYSTEM_STATE_ACTIVE,
    SYSTEM_STATE_ASLEEP,
    SYSTEM_STATE_ERROR,
    SYSTEM_STATE_RECOVERY,
    SYSTEM_STATE_UNKNOWN,
    NUM_SYSTEM_STATES
} SYSTEM_STATE, system_state_t;

//typedef enum
//{
//    SYSTEM_ACTION_NONE = 0,
//    SYSTEM_ACTION_CHANGE_STATE,
//    SYSTEM_ACTION_CONNECT_TO_HOST,
//    SYSTEM_ACTION_RECONNECT_TO_HOST,
//    SYSTEM_ACTION_TRIGGER_TAU,
//    SYSTEM_ACTION_GO_TO_SLEEP,
//
//    SYSTEM_ACTION_REPORT_DATA,
//    SYSTEM_ACTION_REPORT_ERROR
//} SYSTEM_ACTION;

typedef enum
{
    SYSTEM_ACTIVITY_NONE = 0,
    SYSTEM_ACTIVITY_STARTUP,
    SYSTEM_ACTIVITY_TAU_STANDARD_START,

//    SYSTEM_ACTIVITY_PROBE_HOST,
//    SYSTEM_ACTIVITY_TRANSMIT_HOST_PACKET,
//    SYSTEM_ACTIVITY_RECEIVE_HOST_PACKET,
//    SYSTEM_ACTIVITY_TRANSMIT_SUB_PACKET,
//    SYSTEM_ACTIVITY_RECEIVE_SUB_PACKET,
//    SYSTEM_ACTIVITY_PROBE_RHO,
//    SYSTEM_ACTIVITY_RECEIVE_RHO_PACKET,
//    SYSTEM_ACTIVITY_PROFILE_INIT,
//    SYSTEM_ACTIVITY_TAU_INIT,
//    SYSTEM_ACTIVITY_TAU_STANDARD,
//    SYSTEM_ACTIVITY_TAU_AUGMENTED,
//    SYSTEM_ACTIVITY_TAU_MINIMAL,
//    SYSTEM_ACTIVITY_PROBE_TIP,
//    SYSTEM_ACTIVITY_UPDATE_HAPTIC,
    
    SYSTEM_ACTIVITY_IDLE,
    SYSTEM_ACTIVITY_SLEEP,
    
    NUM_SYSTEM_ACTIVITIES
} SYSTEM_ACTIVITY, system_activity_t;

typedef enum
{
    /* General */
    SYSTEM_SUBACTIVITY_NONE = 0,
    SYSTEM_SUBACTIVITY_SELF_CHECK,
    SYSTEM_SUBACTIVITY_WAIT_FOR_WAKE,
    
    /* Initialization */
    SYSTEM_SUBACTIVITY_INIT_COMMUNICATION,
    SYSTEM_SUBACTIVITY_INIT_COMPONENTS,
    SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT,
    SYSTEM_SUBACTIVITY_INIT_RHO_CLIENT,
    SYSTEM_SUBACTIVITY_INIT_CONFIRM,
    SYSTEM_SUBACTIVITY_BATTERY_MONITOR_ACTIVE,
    
    /* Profile */
//    SYSTEM_SUBACTIVITY_PROFILE_FETCH,
//    SYSTEM_SUBACTIVITY_PROFILE_PERFORM,
//    SYSTEM_SUBACTIVITY_PROFILE_UPDATE,
//    SYSTEM_SUBACTIVITY_PROFILE_STORE,
    
    /* Probes */
    SYSTEM_SUBACTIVITY_SEND_HOST_PROBE,
    SYSTEM_SUBACTIVITY_RECEIVE_HOST_PROBE,
    
    /* Handles */
    SYSTEM_SUBACTIVITY_HANDLE_MOTION_EVENT,
    SYSTEM_SUBACTIVITY_HANDLE_RHO_EVENT,
    SYSTEM_SUBACTIVITY_HANDLE_TOUCH_EVENT,
    
    SYSTEM_SUBACTIVITY_POLL_BATTERY_MONITOR,
    SYSTEM_SUBACTIVITY_POLL_TIP,
    
    SYSTEM_SUBACTIVITY_TRIGGER_HAPTIC,
    
    SYSTEM_SUBACTIVITY_TRANSMIT_HOST_PACKET,
    SYSTEM_SUBACTIVITY_RECEIVE_HOST_PACKET,
    SYSTEM_SUBACTIVITY_TRANSMIT_SUB_RADIO_PACKET,
    SYSTEM_SUBACTIVITY_RECEIVE_SUB_RADIO_PACKET,
    
    /* Tau */
    SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_START,
    SYSTEM_SUBACTIVITY_TAU_STANDARD_MOTION_START,
    SYSTEM_SUBACTIVITY_TAU_STANDARD_START,
    
    /* Sleep/Idle */
    SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_STOP,
    SYSTEM_SUBACTIVITY_TAU_STANDARD_MOTION_STOP,
    SYSTEM_SUBACTIVITY_TAU_STOP,
    SYSTEM_SUBACTIVITY_BATTERY_MONITOR_SLEEP,
    
    NUM_SYSTEM_SUBACTIVITIES
} SYSTEM_SUBACTIVITY, system_subactivity_t;
typedef enum
{
    SYSTEM_ERROR_NONE = 0,
    SYSTEM_ERROR_STARTUP,
    SYSTEM_ERROR_HARDWARE_NOT_FOUND,
    SYSTEM_ERROR_HOST_NOT_FOUND
} SYSTEM_ERROR, system_error_t;
typedef enum
{
    SYSTEM_CONSUMPTION_NONE = 0,
    SYSTEM_CONSUMPTION_MICRO,
    SYSTEM_CONSUMPTION_LOW,
    SYSTEM_CONSUMPTION_NORMAL,
    SYSTEM_CONSUMPTION_HIGH,
    SYSTEM_CONSUMPTION_SURGE
} SYSTEM_CONSUMPTION, system_consumption_t;
typedef enum
{
    SYSTEM_FAMILY_0 = 1, /* Always on */
    SYSTEM_FAMILY_A,
    SYSTEM_FAMILY_B,
    SYSTEM_FAMILY_C,
    SYSTEM_FAMILY_D,
    NUM_SYSTEM_FAMILIES
} SYSTEM_FAMILY, system_family_t;
typedef enum
{
    SYSTEM_SENSOR_MOTION_PRIMARY = 0x50,
    SYSTEM_SENSOR_MOTION_SECONDARY = 0x51,
    SYSTEM_SENSOR_RHO_MODULE_PRIMARY = 0x60,
    SYSTEM_SENSOR_RHO_MODULE_SECONDARY = 0x61,
    SYSTEM_SENSOR_TOUCH_PRIMARY = 0x70,
    SYSTEM_SENSOR_TOUCH_SECONDARY = 0x71,
    SYSTEM_SENSOR_TIP_PRIMARY = 0x80,
    SYSTEM_SENSOR_TIP_SECONDARY = 0x81,
    SYSTEM_SENSOR_TIP_ALTERNATE = 0x82,
    SYSTEM_SENSOR_BATTERY_MONITOR_PRIMARY = 0x90
} SYSTEM_SENSOR;
typedef enum
{
    SYSTEM_DRIVER_BLE_RADIO = 0x30,
    SYSTEM_DRIVER_SUB_RADIO = 0x40,
    SYSTEM_DRIVER_HAPTIC_PRIMARY = 0x50,
    SYSTEM_DRIVER_HAPTIC_SECONDARY = 0x51,
    SYSTEM_DRIVER_REGULATOR_1V5 = 0x61
} SYSTEM_DRIVER;
typedef enum
{
    SYSTEM_COMPONENT_TYPE_NONE = 0x00,
    SYSTEM_COMPONENT_TYPE_SENSOR = 0x02,
    SYSTEM_COMPONENT_TYPE_DRIVER = 0x03
} SYSTEM_COMPONENT_TYPE;

typedef enum
{
    SYSTEM_COMPONENT_NONE = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_NONE, 0),
    SYSTEM_COMPONENT_MOTION_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_MOTION_PRIMARY),
    SYSTEM_COMPONENT_MOTION_SECONDARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_MOTION_SECONDARY),
    SYSTEM_COMPONENT_RHO_MODULE_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_RHO_MODULE_PRIMARY),
    SYSTEM_COMPONENT_RHO_MODULE_SECONDARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_RHO_MODULE_SECONDARY),
    SYSTEM_COMPONENT_TOUCH_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_TOUCH_PRIMARY),
    SYSTEM_COMPONENT_TOUCH_SECONDARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_TOUCH_SECONDARY),
    SYSTEM_COMPONENT_TIP_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_TIP_PRIMARY),
    SYSTEM_COMPONENT_TIP_SECONDARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_TIP_SECONDARY),
    SYSTEM_COMPONENT_TIP_ALTERNATE = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_TIP_ALTERNATE),
    SYSTEM_COMPONENT_BATTERY_MONITOR_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_SENSOR, SYSTEM_SENSOR_BATTERY_MONITOR_PRIMARY),
    
    SYSTEM_COMPONENT_BLE_RADIO = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_DRIVER, SYSTEM_DRIVER_BLE_RADIO),
    SYSTEM_COMPONENT_SUB_RADIO = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_DRIVER, SYSTEM_DRIVER_SUB_RADIO),
    SYSTEM_COMPONENT_HAPTIC_PRIMARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_DRIVER, SYSTEM_DRIVER_HAPTIC_PRIMARY),
    SYSTEM_COMPONENT_HAPTIC_SECONDARY = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_DRIVER, SYSTEM_DRIVER_HAPTIC_SECONDARY),
    SYSTEM_COMPONENT_REGULATOR_1V5 = COMPONENT_ID(SYSTEM_COMPONENT_TYPE_DRIVER, SYSTEM_DRIVER_REGULATOR_1V5)
} SYSTEM_COMPONENT;

typedef enum
{
    COMPONENT_STATE_OFF = 0x00,
    COMPONENT_STATE_ON = 0x01,
    COMPONENT_STATE_Z = 0x02,
    COMPONENT_STATE_INTERRUPT = 0x0a
} COMPONENT_STATE;
typedef enum
{
    SYSTEM_PROFILE_ENTRY_STATE_NONE = 0,
    SYSTEM_PROFILE_ENTRY_STATE_IDLE, // Uninitialized
    SYSTEM_PROFILE_ENTRY_STATE_DISABLED,
    SYSTEM_PROFILE_ENTRY_STATE_ENABLED,
    SYSTEM_PROFILE_ENTRY_STATE_ACTIVE,
} SYSTEM_PROFILE_ENTRY_STATE, system_profile_entry_state;
typedef enum
{
    SYSTEM_PROFILE_ENTRY_DIRECTION_NONE = 0,
    SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT,
    SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT,
    SYSTEM_PROFILE_ENTRY_DIRECTION_INOUT
} SYSTEM_PROFILE_ENTRY_DIRECTION, system_profile_entry_direction;
typedef enum
{
    SYSTEM_PROFILE_ENTRY_TYPE_NONE = 0,
    SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT,
    SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULED
} SYSTEM_PROFILE_ENTRY_TYPE, system_profile_entry_type;
typedef enum
{
    SYSTEM_TASK_SHELF_ENTRY_ID_NULL_TASKS = 0,
    SYSTEM_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_SENSOR_TOUCH_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_SENSOR_TIP_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_SENSOR_BATTERY_MONITOR_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_SENSOR_RHO_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_DRIVER_HAPTIC_PRIMARY_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_COMMUNICATION_HOST_RADIO_TASKS,
    SYSTEM_TASK_SHELF_ENTRY_ID_COMMUNICATION_SUB_RADIO_TASKS
} SYSTEM_TASK_SHELF_ENTRY_ID, system_task_shelf_entry_id_t;
typedef enum
{
    SYSTEM_PROBE_ID_NONE = 0,
    SYSTEM_PROBE_ID_HOST,
    SYSTEM_PROBE_ID_RHO,
    SYSTEM_PROBE_ID_TIP,
    SYSTEM_PROBE_ID_BATTERY_MONITOR,
    NUM_SYSTEM_PROBES
} SYSTEM_PROBE_ID, system_probe_id_t;
typedef enum
{
    SYSTEM_SCHEDULER_ID_NONE = 0,
    SYSTEM_SCHEDULER_ID_TAU_PERFORM,
    SYSTEM_SCHEDULER_ID_TAU_PACKET_QUEUE,
    
    SYSTEM_SCHEDULER_ID_TIP_POLL,
    SYSTEM_SCHEDULER_ID_BATTERY_MONITOR_POLL,
    
    NUM_SYSTEM_SCHEDULES
} SYSTEM_SCHEDULER_ID, system_scheduler_id_t;
typedef enum
{
    SYSTEM_INTERRUPTER_ID_NONE = 0,
    SYSTEM_INTERRUPTER_ID_TAU_PACKET_TRANSMIT,
    SYSTEM_INTERRUPTER_ID_TAU_PACKET_RECEIVE,
    SYSTEM_INTERRUPTER_ID_SUB_RADIO_PACKET_TRANSMIT,
    SYSTEM_INTERRUPTER_ID_HAPTIC_TRIGGER,
    SYSTEM_SCHEDULER_ID_MOTION_INTERRUPT,
    SYSTEM_SCHEDULER_ID_RHO_INTERRUPT,
    SYSTEM_SCHEDULER_ID_TOUCH_INTERRUPT,
    
    NUM_SYSTEM_INTERRUPTS
} SYSTEM_INTERRUPTER_ID, system_interrupter_id_t;

/************************************************************************************/

/************************************************************************************/
/***                               Types Start                                    ***/
/************************************************************************************/
typedef struct
{
    uint8_t ID;
    union
    {
        void (*blank)(void);
        void (*byte)(uint8_t);
        void (*word)(uint32_t);
        void (*pointer)(void *);
    } function;
    union
    {
        uint8_t byte;
        uint32_t word;
        void * pointer;
    } data;
} system_subactivity_map_entry_t;
typedef struct
{
    system_subactivity_map_entry_t entries[NUM_SYSTEM_SUBACTIVITIES];
} system_subactivity_map_t;

typedef struct
{
    system_activity_t activity;
    uint8_t length;
    system_subactivity_t subactivities[MAX_SUBACTIVITIES_PER_ACTIVITY];
    system_state_t exit_state;
} system_activity_routine_t;
typedef system_activity_routine_t system_routine_map_t[MAX_ROUTINES];

typedef struct
{
    system_profile_entry_state
state:3;
    system_profile_entry_direction
direction:2;
    system_profile_entry_type
type:3;
} system_profile_header;

typedef struct
{
    union
    {
        system_probe_id_t probe;
        system_scheduler_id_t scheduler;
        system_interrupter_id_t interrupter;
    } ID;
system_profile_header
    header;
    union
    {
        uint32_t
        schedule;
        uint8_t
        info[4];
    } data;
component_id
    component;
system_subactivity_t
    handler_id;
} system_profile_entry_t;

typedef struct
{
uint8_t
    task_id,
    num_interrupts;
system_profile_entry_t
    interrupts[MAX_INTERRUPTS];
uint8_t
    num_scheduled;
system_profile_entry_t
    scheduled[MAX_SCHEDULED];
} system_task_shelf_entry_t;
typedef system_task_shelf_entry_t system_task_shelf_t[MAX_TASK_SHELF_ENTRIES];

typedef struct
{
system_activity_routine_t
    routine;
uint8_t
    families[NUM_SYSTEM_FAMILIES];
uint8_t
    num_entries;
system_task_shelf_entry_id_t
    entries[MAX_STATE_PROFILE_ENTRIES];
} system_state_profile_t;
typedef system_state_profile_t system_state_profile_list_t[NUM_SYSTEM_STATES];

#define MAX_COMPONENTS 20
typedef struct
{
system_task_shelf_t
    shelf;
system_state_profile_list_t
    state_profiles;
component_t
    components[MAX_COMPONENTS];
} system_profile_t;

typedef struct
{
    void (*Send)(system_probe_id_t);
    void (*Receive)(system_probe_id_t);
} system_probe_functions;

typedef struct
{
    void (*Init)( system_profile_t * );
    void (*Fetch)(void);
    void (*Perform)(void);
    void (*Update)(void);
    void (*Store)(void);
} system_profile_functions;

typedef struct
{
    void (*Schedule)(system_scheduler_id_t, uint32_t );
    void (*Deschedule)(system_scheduler_id_t);
} system_scheduler_functions;

typedef struct
{
    void (*Send)(system_interrupter_id_t);
    void (*Receive)(system_interrupter_id_t);
    void (*Perform)(system_interrupter_id_t);
} system_interrupter_functions;

typedef struct
{
orientation_data_t
    orientation;
rho_data_t
    rho;
touch_data_t
    touch;
comm_packet_t
    packet_in,
    packet_out,
    sub_packet_in,
    sub_packet_out;
battery_monitor_basic_t
    battery;
uint8_t
    battery_monitor_mode;
tau_settings_t
    tau_settings;
} system_buffers_t;

typedef struct
{
imu_t
    IMU;
rho_t
    Rho;
tau_t
    Kinetic;
} system_objects_t;

typedef struct
{
    system_profile_t       *profile;
    system_state_t          state;
    system_activity_t       activity;
    system_subactivity_t    subactivity;
    system_error_t          error;
    system_consumption_t    consumption_level;
    system_subactivity_map_t subactivity_map;
    system_buffers_t        buffers;
    system_objects_t        objects;
} system_master_t;
/************************************************************************************/

/************************************************************************************/
/***                             Functions Start                                  ***/
/************************************************************************************/


static void NullFunction(void) {}
/************************************************************************************/

#endif /* systemtypes_h */
