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
#error "No OS defined."
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
    COMM_READ_REG = 1,
    COMM_WRITE_REG
} COMM_TYPE;

#define DEFAULT_TASK_OS_OPTIONS 0 //OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR

typedef OS_Q queue_t;

#define MAX_SUBACTIVITIES_PER_ACTIVITY 20
#define MAX_TASKS 10
#define MAX_INTERRUPTS (MAX_TASKS/2)
#define MAX_SCHEDULED (MAX_TASKS/2)
#define MAX_TASK_SHELF_ENTRIES 20
#define MAX_STATE_PROFILE_ENTRIES 10

#define NUM_SYSTEM_STATE NUM_APPLICATION_STATE
#define NUM_SYSTEM_COMPONENT_FAMILY NUM_APPLICATION_COMPONENT_FAMILY
#define NUM_SYSTEM_QUEUE_ID NUM_APPLICATION_QUEUE_ID
#define NUM_SYSTEM_SUBACTIVITY_ID NUM_APPLICATION_SUBACTIVITY_ID
#define NUM_SYSTEM_TASK_ID NUM_APPLICATION_TASK_ID

#define SYSTEM_COMPONENT_FAMILY APPLICATION_COMPONENT_FAMILY
#define SYSTEM_COMPONENT_FAMILY_0 APPLICATION_COMPONENT_FAMILY_0

#define SYSTEM_ACTION_ID_NONE APPLICATION_ACTION_ID_NONE
#define SYSTEM_TASK_SHELF_ENTRY_ID_NONE APPLICATION_TASK_SHELF_ENTRY_ID_NONE

#define APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS
#define SYSTEM_COMPONENT_NONE APPLICATION_COMPONENT_NONE

#define SYSTEM_TASK_SHELF_ENTRY_ID_STRINGS APPLICATION_TASK_SHELF_ENTRY_ID_STRINGS
#define SYSTEM_COMPONENT_FAMILY_STRINGS APPLICATION_COMPONENT_FAMILY_STRINGS
#define SYSTEM_TASK_ID_STRINGS APPLICATION_TASK_ID_STRINGS
#define SYSTEM_TIMER_ID_STRINGS APPLICATION_TASK_ID_STRINGS
#define SYSTEM_SUBACTIVITY_ID_STRINGS APPLICATION_SUBACTIVITY_ID_STRINGS
#define SYSTEM_STATE_STRINGS APPLICATION_STATE_STRINGS
#define SYSTEM_ACTIVITY_STRINGS APPLICATION_ACTIVITY_STRINGS
#define SYSTEM_QUEUE_ID_STRINGS APPLICATION_QUEUE_ID_STRINGS


/************************************************************************************/
/***                               Enums Start                                    ***/
/************************************************************************************/

typedef application_comm_protocol_t system_comm_protocol_t;
typedef application_state_t system_state_t;
typedef application_activity_t system_activity_t;
typedef application_task_shelf_entry_id_t system_task_shelf_entry_id_t;

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

typedef application_component_family_t system_family_t;

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

typedef OS_SPECIFIC(OS_TASK_DATA_T) os_task_data_t;
typedef OS_SPECIFIC(OS_QUEUE_DATA_T) os_queue_data_t;
typedef OS_SPECIFIC(OS_TIMER_DATA_T) os_timer_data_t;

typedef PAPI_SPECIFIC(I2C_EVENT_T) i2c_event_t;
typedef PAPI_SPECIFIC(I2C_HOST_T) i2c_host_t;
typedef PAPI_SPECIFIC(I2C_TRANSFER_TYPE_T) i2c_transfer_type_t;
typedef PAPI_SPECIFIC(I2C_TRANSFER_RETURN_T) i2c_transfer_return_t;

typedef PAPI_SPECIFIC(SPI_EVENT_T) spi_event_t;
typedef PAPI_SPECIFIC(SPI_HOST_T) spi_host_t;
//typedef PAPI_SPECIFIC(SPI_TRANSFER_TYPE_T) spi_transfer_type_t;
typedef PAPI_SPECIFIC(SPI_TRANSFER_RETURN_T) spi_transfer_return_t;

typedef bool generic_comm_return_t;
//typedef os_task_data_t os_task_list_t[NUM_SYSTEM_TASK_ID];
//typedef os_queue_data_t os_queue_list_t[NUM_SYSTEM_QUEUE_ID];

typedef application_subactivity_id_t system_subactivity_id_t;
typedef application_task_id_t system_task_id_t;
typedef application_task_shelf_entry_id_t system_task_shelf_entry_id_t;
typedef application_task_priority_t system_task_priority_t;
typedef application_task_action_t system_task_action_t;
//typedef application_objects_t system_objects_t;
//typedef application_buffers_t system_buffers_t;

typedef I2C_TypeDef I2C_Channel;
typedef SPI_TypeDef SPI_Channel;

typedef struct
{
system_comm_protocol_t
    protocol:8;
uint8_t
    address:8;
void *
    device;
void *
    buffer;
} generic_comm_host_t;

#define NULL_HOST (comm_host_t)(generic_comm_host_t){ 0, 0, NULL, NULL }

typedef union
{
    i2c_host_t i2c_host;
    spi_host_t spi_host;
    generic_comm_host_t generic_comm_host;
} comm_host_t;

typedef struct
{
    uint8_t ID;
    component_id_t component_id[MAX_COMPONENTS];
    uint8_t num_component_id;
    void (*function)(void *);
    void * data;
} system_subactivity_t;

typedef struct
{
    uint8_t num_entries;
    system_subactivity_t entry[NUM_SYSTEM_SUBACTIVITY_ID];
} system_subactivity_map_t;

typedef struct
{
    system_activity_t activity;
    system_subactivity_id_t subactivities[MAX_SUBACTIVITIES_PER_ACTIVITY];
    uint8_t num_subactivities;
    system_state_t exit_state;
} system_activity_routine_t;

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
component_id_t
    component_id[MAX_COMPONENTS];
uint8_t
    num_component_id;
//system_profile_header
//    header;
    union
    {
        frequency_t
        schedule;
        uint8_t
        info[4];
    } data;
    system_task_action_t ACTION;
    void          *function; /* Pointer to task */
    void          *object; /* Argument */
    system_task_priority_t PRIORITY; /* Priority */
    OS_ERR        *error;
//system_subactivity_id_t
//    handler_id;
os_task_data_t
    os_task_data;
} system_task_t;

typedef struct
{
system_task_shelf_entry_id_t
    ID;
uint8_t
    num_interrupts;
system_task_id_t
    interrupts[MAX_INTERRUPTS];
uint8_t
    num_scheduled;
system_task_id_t
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
    families[NUM_SYSTEM_COMPONENT_FAMILY];
system_task_shelf_entry_id_t
    entries[MAX_STATE_PROFILE_ENTRIES];
uint8_t
    num_entries;
} system_state_profile_t;
typedef system_state_profile_t system_state_profile_list_t[NUM_SYSTEM_STATE];

typedef struct
{
uint8_t
    num_entries;
system_task_t
    entries[NUM_SYSTEM_TASK_ID];
} task_list_t;

typedef struct
{
uint8_t
    num_entries;
system_subactivity_t
    entries[NUM_SYSTEM_TASK_ID];
} subactivity_list_t;

typedef struct
{
uint8_t
    num_entries;
component_t
    entries[MAX_COMPONENTS];
} component_list_t;

typedef struct
{
uint8_t
    num_entries;
os_queue_data_t
    entries[NUM_SYSTEM_TASK_ID];
} queue_list_t;

typedef struct
{
    component_list_t            component_list;
    system_subactivity_map_t    subactivity_map;
    queue_list_t                queue_list;
    task_list_t                 task_list;
    system_task_shelf_t         shelf;
    system_state_profile_list_t state_profiles;
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

typedef struct
{
    bool                    tasks[NUM_SYSTEM_TASK_ID];
    bool                    component_tasks[MAX_COMPONENTS][NUM_SYSTEM_TASK_ID];
    comm_host_t             comm_hosts[MAX_COMPONENTS];
} system_registration_log_t;

typedef struct
{
    system_state_t          state, prev_state, exit_state;
    system_activity_t       activity;
    system_subactivity_id_t subactivity;
    system_error_buffer_t   error;
    system_consumption_t    consumption_level;
    system_registration_log_t registration;
    system_profile_t       *profile;
} system_master_t;
/************************************************************************************/

/************************************************************************************/
/***                             Functions Start                                  ***/
/************************************************************************************/

static void NullFunction(void) {}
/************************************************************************************/

#endif /* systemtypes_h */
