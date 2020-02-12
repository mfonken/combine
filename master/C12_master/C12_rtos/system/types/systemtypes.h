//
//  systemtypes.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef system_types_h
#define system_types_h

#include "global_config.h"
#include "application_types.h"
#include "application_debug.h"

#if __OS__ == MICRIUM
#include "micrium_interface.h"
#else
/*#error */#warning "No OS defined."
#endif

#if __PAPI__ == EMLIB
#include "emlib_interface.h"
#else
#error "No peripheral API defined."
#endif

#include "globaltypes.h"

#define NO_REG 0xff

#define DEFAULT_TASK_STACK_LIMIT_FACTOR 10u

/* Port spoof */
#define PORT0 0
#define PORTA 0
#define PORTB 0
#define PORTC 0
#define PORTD 0
#define PORTF 0

#define INTERNAL 0xff

#define COMM_PORT_NONE 0xff
#define COMM_ADDR_NONE 0xff

typedef enum
{
    COMM_PROTOCOL_NONE = 0,
    COMM_PROTOCOL_I2C,
    COMM_PROTOCOL_SPI,
    COMM_PROTOCOL_UART,
    COMM_PROTOCOL_BLE,
    COMM_PROTOCOL_SUB
} COMM_PROTOCOL, comm_protocol;

typedef enum
{
    COMM_ROUTE_PRIMARY = 0,
    COMM_ROUTE_SECONDARY,
    COMM_ROUTE_NONE
} COMM_ROUTE;

typedef enum
{
    COMM_READ_REG = 1,
    COMM_WRITE_REG
} COMM_TYPE;

typedef enum
{
    TASK_PRIORITY_CLASS_EXECUTIVE = 2,
    TASK_PRIORITY_CLASS_HIGH,
    TASK_PRIORITY_CLASS_MEDIUM,
    TASK_PRIORITY_CLASS_LOW,
    TASK_PRIORITY_CLASS_PASSIVE,
    TASK_PRIORITY_CLASS_QUARANTINE
} TASK_PRIORITY_CLASS;

#define DEFAULT_TASK_OS_OPTIONS             0 //OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR

typedef OS_Q queue_t;

#define MAX_ROUTINES 45
#define MAX_SUBACTIVITIES_PER_ACTIVITY 20
#define MAX_TASKS 10
#define MAX_INTERRUPTS MAX_TASKS/2
#define MAX_SCHEDULED MAX_TASKS/2
#define MAX_TASK_SHELF_ENTRIES 20
#define MAX_STATE_PROFILE_ENTRIES 10

//#define APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS
#define NUM_SYSTEM_TASKS 1//NUM_APPLICATION_TASKS
#define NUM_SYSTEM_QUEUES 1//NUM_APPLICATION_QUEUES
#define NUM_SYSTEM_SUBACTIVITIES NUM_APPLICATION_SUBACTIVITIES
#define SYSTEM_ACTION_ID_NONE APPLICATION_ACTION_ID_NONE

#define APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS
#define SYSTEM_COMPONENT_NONE APPLICATION_COMPONENT_NONE

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

typedef enum
{
    SYSTEM_ACTIVITY_NONE = 0,
    SYSTEM_ACTIVITY_STARTUP,
//    SYSTEM_ACTIVITY_TAU_STANDARD_START,
    
    SYSTEM_ACTIVITY_IDLE,
    SYSTEM_ACTIVITY_SLEEP,
    
    NUM_SYSTEM_ACTIVITIES
} SYSTEM_ACTIVITY, system_activity_t;

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
    SYSTEM_FAMILY_NONE = 0,
    SYSTEM_FAMILY_0, /* Always on */
    SYSTEM_FAMILY_A,
    SYSTEM_FAMILY_B,
    SYSTEM_FAMILY_C,
    SYSTEM_FAMILY_D,
    NUM_SYSTEM_FAMILIES
} SYSTEM_FAMILY, system_family_t;

typedef enum
{
    SYSTEM_QUEUE_ID_HW_INTERRUPTS,
    SYSTEM_QUEUE_ID_COMM_INTERRUPTS,
    SYSTEM_QUEUE_ID_RUNTIME_MESSAGES,
    SYSTEM_QUEUE_ID_APPLICATION_MESSAGES,
    
    NUM_SYSTEM_QUEUE
} SYSTEM_QUEUE_ID, system_queue_id_t;

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


/************************************************************************************/

/************************************************************************************/
/***                               Types Start                                    ***/
/************************************************************************************/

//typedef enum
//{
//    TASK_TYPE_GENERIC   = 0x00,
//    TASK_TYPE_PROBE     = 0x01,
//    TASK_TYPE_SCHEDULE  = 0x02,
//    TASK_TYPE_INTERRUPT = 0x0a
//} TASK_TYPE;

typedef OS_SPECIFIC(OS_TASK_DATA_T) os_task_data_t;
typedef OS_SPECIFIC(OS_QUEUE_DATA_T) os_queue_data_t;
typedef OS_SPECIFIC(OS_TIMER_DATA_T) os_timer_data_t;

typedef bool generic_comm_return_t;

typedef PAPI_SPECIFIC(I2C_EVENT_T) i2c_event_t;
typedef PAPI_SPECIFIC(I2C_HOST_T) i2c_host_t;
typedef PAPI_SPECIFIC(I2C_TRANSFER_TYPE_T) i2c_transfer_type_t;
typedef PAPI_SPECIFIC(I2C_TRANSFER_RETURN_T) i2c_transfer_return_t;

typedef PAPI_SPECIFIC(SPI_EVENT_T) spi_event_t;
typedef PAPI_SPECIFIC(SPI_HOST_T) spi_host_t;
//typedef PAPI_SPECIFIC(SPI_TRANSFER_TYPE_T) spi_transfer_type_t;
typedef PAPI_SPECIFIC(SPI_TRANSFER_RETURN_T) spi_transfer_return_t;

typedef os_task_data_t os_task_list_t[NUM_SYSTEM_TASKS];
typedef os_queue_data_t os_queue_list_t[NUM_SYSTEM_QUEUES];

typedef application_subactivity_t system_subactivity_t;
typedef application_task_id_t system_task_id_t;
typedef application_task_shelf_entry_id_t system_task_shelf_entry_id_t;
//typedef application_objects_t system_objects_t;
//typedef application_buffers_t system_buffers_t;

typedef I2C_TypeDef I2C_Channel;
typedef SPI_TypeDef SPI_Channel;

typedef struct
{
uint8_t
    protocol:8,
    address:8;
void *
    device;
void *
    buffer;
} generic_comm_host_t;

typedef union
{
    i2c_host_t i2c_host;
    spi_host_t spi_host;
    generic_comm_host_t generic_comm_host;
} comm_host_t;

typedef struct
{
    uint8_t ID;
    void (*function)(void *);
    void * data;
} system_subactivity_map_entry_t;
typedef system_subactivity_map_entry_t system_subactivity_map_t[NUM_SYSTEM_SUBACTIVITIES];

typedef struct
{
    system_activity_t activity;
    system_subactivity_t subactivities[MAX_SUBACTIVITIES_PER_ACTIVITY];
    uint8_t length;
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
system_task_id_t
    ID;
//system_profile_header
//    header;
    union
    {
        frequency_t
        schedule;
        INTERRUPT_ACTION
        action;
        uint8_t
        info[4];
    } data;
component_id_t
    component_id;
system_subactivity_t
    handler_id;
os_task_data_t
    *os_task_data;
} system_profile_entry_t;

typedef struct
{
uint8_t
    ID,
    num_interrupts;
system_profile_entry_t
    interrupts[MAX_INTERRUPTS];
uint8_t
    num_scheduled;
system_profile_entry_t
    scheduled[MAX_SCHEDULED];
} system_task_shelf_entry_t;

typedef struct
{
uint8_t
    num_tasks;
system_task_shelf_entry_t tasks[MAX_TASK_SHELF_ENTRIES];
} system_task_shelf_t;

typedef struct
{
system_activity_routine_t
    routine;
system_state_t
    state_id;
uint8_t
    families[NUM_SYSTEM_FAMILIES];
system_task_shelf_entry_id_t
    entries[MAX_STATE_PROFILE_ENTRIES];
uint8_t
    num_entries;
} system_state_profile_t;
typedef system_state_profile_t system_state_profile_list_t[NUM_SYSTEM_STATES];

#define MAX_COMPONENTS 20
typedef struct
{
uint8_t
    num_entries;
component_t
    entries[MAX_COMPONENTS];
} component_list_t;

typedef struct
{
component_list_t
    component_list;
system_task_shelf_t
    shelf;
system_state_profile_list_t
    state_profiles;
} system_profile_t;

typedef struct
{
    void (*Send)(system_task_id_t);
    void (*Receive)(system_task_id_t);
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
    void (*Schedule)(system_task_id_t, uint32_t );
    void (*Deschedule)(system_task_id_t);
} system_scheduler_functions;

typedef struct
{
    void (*Send)(system_task_id_t);
    void (*Receive)(system_task_id_t, hw_event_message_t);
    void (*Perform)(system_task_id_t);
} system_interrupter_functions;

typedef OS_ERR RTOS_ERR;
typedef struct
{
system_error_t
    type;
RTOS_ERR
    system,
    peripheral,
    interrupt,
    messaging,
    runtime,
    recovery;
} system_error_buffer_t;

//typedef struct
//{
//    bool
//    [MAX_INTERRUPTS],
//    scheduled[MAX_SCHEDULED];
//} system_registered_t;

typedef struct
{
    bool                    profile_entries[NUM_APPLICATION_TASKS];
    system_task_id_t        component_tasks[MAX_COMPONENTS];
    comm_host_t             comm_hosts[MAX_COMPONENTS];
} system_registration_log_t;

typedef struct
{
    system_state_t          state, prev_state, exit_state;
    os_task_list_t         *os_tasks;
    os_queue_list_t        *queue_list;
    system_activity_t       activity;
    system_subactivity_t    subactivity;
    system_error_buffer_t   error;
    system_consumption_t    consumption_level;
    system_subactivity_map_t *subactivity_map;
    system_profile_t       *profile;
    system_registration_log_t registration;
} system_master_t;
/************************************************************************************/

/************************************************************************************/
/***                             Functions Start                                  ***/
/************************************************************************************/

static void NullFunction(void) {}
/************************************************************************************/

#endif /* systemtypes_h */
