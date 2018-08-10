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
#include "i2c_template.h"
#include "spi_template.h"

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
} SYSTEM_STATE;
typedef SYSTEM_STATE system_state_t;

typedef enum
{
    SYSTEM_ACTION_NONE = 0,
    SYSTEM_ACTION_CHANGE_STATE,
    SYSTEM_ACTION_CONNECT_TO_HOST,
    SYSTEM_ACTION_RECONNECT_TO_HOST,
    SYSTEM_ACTION_PERFORM_TAU,
    SYSTEM_ACTION_GO_TO_SLEEP,
    
    SYSTEM_ACTION_REPORT_DATA,
    SYSTEM_ACTION_REPORT_ERROR
} SYSTEM_ACTION;
typedef SYSTEM_ACTION system_action_t;

typedef enum
{
    SYSTEM_ACTIVITY_IDLE = 0,
    SYSTEM_ACTIVITY_STARTUP,
    SYSTEM_ACTIVITY_PROBE_HOST,
    SYSTEM_ACTIVITY_PROBE_RHO,
    SYSTEM_ACTIVITY_PROFILE_INIT,
    SYSTEM_ACTIVITY_TAU_INIT,
    SYSTEM_ACTIVITY_TAU_STANDARD,
    SYSTEM_ACTIVITY_TAU_AUGMENTED,
    SYSTEM_ACTIVITY_TAU_MINIMAL,
    SYSTEM_ACTIVITY_PROBE_BATTERY_MONITOR,
    SYSTEM_ACTIVITY_PROBE_TIP,
    SYSTEM_ACTIVITY_UPDATE_HAPTIC,
    SYSTEM_ACTIVITY_SLEEP,
    
    SYSTEM_ACTIVITY_WAIT_FOR_ACTION
} SYSTEM_ACTIVITY;
typedef SYSTEM_ACTIVITY system_activity_t;

typedef enum
{
    SYSTEM_SUBACTIVITY_SELF_CHECK = 0,
    
    SYSTEM_SUBACTIVITY_INIT_COMMUNICATION,
    SYSTEM_SUBACTIVITY_INIT_COMPONENTS,
    SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT,
    SYSTEM_SUBACTIVITY_INIT_KINETIC,
    SYSTEM_SUBACTIVITY_INIT_CONFIRM,
    
    SYSTEM_SUBACTIVITY_SEND_HOST_PROBE,
    SYSTEM_SUBACTIVITY_RECEIVE_HOST_PROBE,
    
    SYSTEM_SUBACTIVITY_SEND_RHO_PROBE,
    SYSTEM_SUBACTIVITY_RECEIVE_RHO_PROBE,
    
    SYSTEM_SUBACTIVITY_PROFILE_FETCH,
    SYSTEM_SUBACTIVITY_PROFILE_PERFORM,
    SYSTEM_SUBACTIVITY_PROFILE_UPDATE,
    SYSTEM_SUBACTIVITY_PROFILE_STORE,
    
    SYSTEM_SUBACTIVITY_TAU_SCHEDULE_RHO_RECEIVER,
    SYSTEM_SUBACTIVITY_TAU_SCHEDULE_PACKET_QUEUER,
    SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_RHO_RECEIVER,
    SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_PACKET_QUEUER,
    
    SYSTEM_SUBACTIVITY_TAU_RECEIVE_RHO_PACKET,
    SYSTEM_SUBACTIVITY_TAU_PERFORM_KINETIC,
    SYSTEM_SUBACTIVITY_TAU_GENERATE_PACKET,
    
    SYSTEM_SUBACTIVITY_TIP_SEND_PROBE,
    
    SYSTEM_SUBACTIVITY_BATTERY_MONITOR_SEND_PROBE,
    
    SYSTEM_SUBACTIVITY_HAPTIC_SEND_PACKET,
    
    SYSTEM_SUBACTIVITY_HANDLE_INTERRUPT,
} SYSTEM_SUBACTIVITY;
typedef SYSTEM_SUBACTIVITY system_subactivity_t;

typedef enum
{
    SYSTEM_ERROR_NONE = 0,
    SYSTEM_ERROR_STARTUP,
    SYSTEM_ERROR_HARDWARE_NOT_FOUND,
    SYSTEM_ERROR_RHO_CLIENT_NOT_FOUND,
    SYSTEM_ERROR_
} SYSTEM_ERROR;
typedef SYSTEM_ERROR system_error_t;

typedef enum
{
    SYSTEM_CONSUMPTION_NONE = 0,
    SYSTEM_CONSUMPTION_MICRO,
    SYSTEM_CONSUMPTION_LOW,
    SYSTEM_CONSUMPTION_NORMAL,
    SYSTEM_CONSUMPTION_HIGH,
    SYSTEM_CONSUMPTION_SURGE
} SYSTEM_CONSUMPTION;
typedef SYSTEM_CONSUMPTION system_consumption_t;

typedef struct
{
    system_state_t          state;
    system_action_t         action;
    system_activity_t       activity;
    system_subactivity_t    subactivity;
    system_error_t          error;
    system_consumption_t    consumption_level;
} system_master_t;

typedef struct
{
    system_activity_t activity;
    uint8_t length;
    system_subactivity_t subactivities[];
} system_activity_routine_t;

typedef enum
{
    SYSTEM_FAMILY_0 = 0, /* Always on */
    SYSTEM_FAMILY_A,
    SYSTEM_FAMILY_B,
    SYSTEM_FAMILY_C,
    SYSTEM_FAMILY_D
} SYSTEM_FAMILY;

typedef enum
{
    SYSTEM_COMM_NONE = 0,
    SYSTEM_COMM_I2C,
    SYSTEM_COMM_SPI,
    SYSTEM_COMM_UART
} SYSTEM_COMM;

typedef enum
{
    SYSTEM_COMM_CHANNEL_NONE = 0,
    SYSTEM_COMM_CHANNEL_PRIMARY,
    SYSTEM_COMM_CHANNEL_SECONDARY
} SYSTEM_COMM_CHANNEL;

typedef enum
{
    SYSTEM_COMM_READ_REG = 1,
    SYSTEM_COMM_WRITE_REG
} SYSTEM_COMM_TYPE;

typedef SYSTEM_COMM system_comm_channel;
typedef SYSTEM_COMM_TYPE system_comm_type;

#define SYSTEM_COMM_ADDR_NONE 0xff

typedef struct
{
    system_comm_type
type:8;
    uint8_t
    reg,
    length,
    addr;
    system_comm_channel
    channel;
} comm_event_t;

static void performCommEvent( comm_event_t event, uint8_t * data )
{
    switch( event.channel )
    {
        case SYSTEM_COMM_I2C:
            performI2CEvent(*(i2c_event_t *)&event, data );
            break;
        case SYSTEM_COMM_SPI:
            performSPIEvent(*(spi_event_t *)&event, data );
            break;
        case SYSTEM_COMM_UART:
            //performUARTEvent(*(uart_event_t *)&event, data );
            break;
        default:
            break;
    }
}

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
    SYSTEM_DRIVER_HAPTIC_PRIMARY = 0x50,
    SYSTEM_DRIVER_HAPTIC_SECONDARY = 0x51,
    SYSTEM_DRIVER_REGULATOR_1V5 = 0x61
} SYSTEM_DRIVER;

typedef enum
{
    SYSTEM_COMPONENT_SENSOR = 0x02,
    SYSTEM_COMPONENT_DRIVER = 0x03
} SYSTEM_COMPONENT;

typedef struct
{
uint8_t
    macro,
    micro;
} component_id;

typedef enum
{
    COMPONENT_STATE_OFF = 0x00,
    COMPONENT_STATE_ON = 0x01,
    COMPONENT_STATE_Z = 0x02,
    COMPONENT_STATE_INTERRUPT = 0x0a
} COMPONENT_STATE;

typedef uint8_t sys_family_t;
typedef struct
{
component_id
    ID;
uint8_t
    family,
    comm,
    chann,
    addr,
    port,
    pin,
    state,
    tied;
void *
    instance;
} sys_component_t;

typedef struct
{
system_activity_routine_t
    idle,
    startup,
    probe_host,
    probe_rho,
    profile_init,
    tau_init,
    tau_standard,
    tau_augmented,
    tau_minimal,
    probe_battery_monitor,
    probe_tip,
    update_haptic,
    sleep;
} sys_routines_t;

typedef struct
{
    
} sys_sensor_profile_t;
typedef struct
{
    
} sys_driver_profile_t;
typedef struct
{
    
} sys_behaviour_profile_t;

typedef enum
{
    SYSTEM_PROFILE_ENTRY_STATE_UNAVAILBLE = 0x00,
    SYSTEM_PROFILE_ENTRY_STATE_IDLE, // Uninitialized
    SYSTEM_PROFILE_ENTRY_STATE_DISABLED,
    SYSTEM_PROFILE_ENTRY_STATE_ENABLED,
    SYSTEM_PROFILE_ENTRY_STATE_ACTIVE,
} SYSTEM_PROFILE_ENTRY_STATE;
typedef SYSTEM_PROFILE_ENTRY_STATE sys_profile_entry_state;
typedef enum
{
    SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT,
    SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT,
    SYSTEM_PROFILE_ENTRY_DIRECTION_INOUT
} SYSTEM_PROFILE_ENTRY_DIRECTION;
typedef SYSTEM_PROFILE_ENTRY_DIRECTION sys_profile_entry_direction;

typedef enum
{
    SYSTEM_PROFILE_ENTRY_TYPE_NONE = 0,
    SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT,
    SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULE
} SYSTEM_PROFILE_ENTRY_TYPE;
typedef SYSTEM_PROFILE_ENTRY_TYPE sys_profile_entry_type;

typedef struct
{
sys_profile_entry_state
    state:3;
sys_profile_entry_direction
    direction:2;
sys_profile_entry_type
    type:3;
} sys_profile_header;

typedef struct
{
sys_profile_header
    header;
    union
    {
    uint32_t
        schedule,
        interrupt;
    } data;
system_activity_routine_t *
    routine;
} sys_profile_entry_t;

typedef struct
{
uint8_t
    num_entries;
sys_profile_entry_t *
    entries;
sys_routines_t *
    routines;
sys_component_t *
    components;
} sys_profile_t;

#endif /* systemtypes_h */
