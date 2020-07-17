// ********************** DO NOT EDIT - AUTO-GENERATED ********************** //
// C12_profile.h

// Created by Combine Profile Generator v0.1 on 7/16/2020
// Copyright © Marbl. All rights reserved.

#ifndef C12_profile_h
#define C12_profile_h

#define NONE 0

typedef enum
{
    COMPONENT_ID_NONE = 0,
    COMPONENT_ID_MOTION_SENSOR,
    COMPONENT_ID_BATTERY_MONITOR,
    COMPONENT_ID_BLE_RADIO,
    NUM_COMPONENT_ID
} COMPONENT_ID, APPLICATION_COMPONENT_ID, application_component_id_t;
#define NUM_APPLICATION_COMPONENT_ID NUM_COMPONENT_ID

typedef enum
{
    COMPONENT_FAMILY_NONE = 0,
    COMPONENT_FAMILY_0,
    COMPONENT_FAMILY_A,
    COMPONENT_FAMILY_B,
    NUM_COMPONENT_FAMILY
} COMPONENT_FAMILY, APPLICATION_COMPONENT_FAMILY, application_component_family_t;
#define NUM_APPLICATION_COMPONENT_FAMILY NUM_COMPONENT_FAMILY

typedef enum
{
    COMPONENT_PROTOCOL_NONE = 0,
    COMPONENT_PROTOCOL_I2C,
    COMPONENT_PROTOCOL_SPI,
    COMPONENT_PROTOCOL_UART,
    COMPONENT_PROTOCOL_BLE,
    COMPONENT_PROTOCOL_SUB,
    NUM_COMPONENT_PROTOCOL
} COMPONENT_PROTOCOL, APPLICATION_COMM_PROTOCOL, application_comm_protocol_t;
#define NUM_APPLICATION_COMM_PROTOCOL NUM_COMPONENT_PROTOCOL

typedef enum
{
    COMPONENT_ROUTE_NONE = 0,
    COMPONENT_ROUTE_PRIMARY,
    NUM_COMPONENT_ROUTE
} COMPONENT_ROUTE, APPLICATION_COMM_ROUTE, application_comm_route_t;
#define NUM_APPLICATION_COMM_ROUTE NUM_COMPONENT_ROUTE

typedef enum
{
    COMPONENT_ADDR_NONE = 0,
    NUM_COMPONENT_ADDR
} COMPONENT_ADDR;

typedef enum
{
    COMPONENT_PORT_0 = 0,
    COMPONENT_PORT_A,
    COMPONENT_PORT_B,
    COMPONENT_PORT_C,
    COMPONENT_PORT_D,
    COMPONENT_PORT_F,
    COMPONENT_PORT_NONE,
    NUM_COMPONENT_PORT
} COMPONENT_PORT, APPLICATION_PORT, application_port_t;
#define NUM_APPLICATION_PORT NUM_COMPONENT_PORT

typedef enum
{
    COMPONENT_PIN_INTERNAL = 0,
    NUM_COMPONENT_PIN
} COMPONENT_PIN;

typedef enum
{
    COMPONENT_STATE_OFF = 0,
    COMPONENT_STATE_ON,
    COMPONENT_STATE_INTERRUPT,
    COMPONENT_STATE_ENABLING,
    COMPONENT_STATE_DISABLING,
    NUM_COMPONENT_STATE
} COMPONENT_STATE, APPLICATION_COMPONENT_STATE, application_component_state_t;
#define NUM_APPLICATION_COMPONENT_STATE NUM_COMPONENT_STATE

#ifdef DEBUG
static const char * COMPONENT_ID_STRINGS[] =
{
    "COMPONENT_ID_NONE",
    "COMPONENT_ID_MOTION_SENSOR",
    "COMPONENT_ID_BATTERY_MONITOR",
    "COMPONENT_ID_BLE_RADIO"
};
#define APPLICATION_COMPONENT_ID_STRINGS COMPONENT_ID_STRINGS
#endif

#ifdef DEBUG
static const char * COMPONENT_FAMILY_STRINGS[] =
{
    "COMPONENT_FAMILY_NONE",
    "COMPONENT_FAMILY_0",
    "COMPONENT_FAMILY_A",
    "COMPONENT_FAMILY_B"
};
#define APPLICATION_COMPONENT_FAMILY_STRINGS COMPONENT_FAMILY_STRINGS
#endif

#ifdef DEBUG
static const char * COMPONENT_PROTOCOL_STRINGS[] =
{
    "COMPONENT_PROTOCOL_NONE",
    "COMPONENT_PROTOCOL_I2C",
    "COMPONENT_PROTOCOL_SPI",
    "COMPONENT_PROTOCOL_UART",
    "COMPONENT_PROTOCOL_BLE",
    "COMPONENT_PROTOCOL_SUB"
};
#define APPLICATION_COMM_PROTOCOL_STRINGS COMPONENT_PROTOCOL_STRINGS
#endif

#ifdef DEBUG
static const char * COMPONENT_ROUTE_STRINGS[] =
{
    "COMPONENT_ROUTE_NONE",
    "COMPONENT_ROUTE_PRIMARY"
};
#define APPLICATION_COMM_ROUTE_STRINGS COMPONENT_ROUTE_STRINGS
#endif

#ifdef DEBUG
static const char * COMPONENT_ADDR_STRINGS[] =
{
    "COMPONENT_ADDR_NONE"
};
#endif

#ifdef DEBUG
static const char * COMPONENT_PORT_STRINGS[] =
{
    "COMPONENT_PORT_0",
    "COMPONENT_PORT_A",
    "COMPONENT_PORT_B",
    "COMPONENT_PORT_C",
    "COMPONENT_PORT_D",
    "COMPONENT_PORT_F",
    "COMPONENT_PORT_NONE"
};
#define APPLICATION_PORT_STRINGS COMPONENT_PORT_STRINGS
#endif

#ifdef DEBUG
static const char * COMPONENT_PIN_STRINGS[] =
{
    "COMPONENT_PIN_INTERNAL"
};
#endif

#ifdef DEBUG
static const char * COMPONENT_STATE_STRINGS[] =
{
    "COMPONENT_STATE_OFF",
    "COMPONENT_STATE_ON",
    "COMPONENT_STATE_INTERRUPT",
    "COMPONENT_STATE_ENABLING",
    "COMPONENT_STATE_DISABLING"
};
#define APPLICATION_COMPONENT_STATE_STRINGS COMPONENT_STATE_STRINGS
#endif

typedef enum
{
    SUBACTIVITIY_ID_NONE = 0,
    SUBACTIVITIY_ID_INIT_COMMUNICATION,
    SUBACTIVITIY_ID_INIT_SYSIOCTL,
    SUBACTIVITIY_ID_ACTIVATE_BATTERY_MONITOR,
    SUBACTIVITIY_ID_DEACTIVATE_BATTERY_MONITOR,
    SUBACTIVITIY_ID_ACTIVATE_MOTION_SENSOR,
    SUBACTIVITIY_ID_DEACTIVATE_MOTION_SENSOR,
    SUBACTIVITIY_ID_TRANSMIT_BLE_PACKET,
    NUM_SUBACTIVITIY_ID
} SUBACTIVITIY_ID, APPLICATION_SUBACTIVITY_ID, application_subactivity_id_t;
#define NUM_APPLICATION_SUBACTIVITY_ID NUM_SUBACTIVITIY_ID

#ifdef DEBUG
static const char * SUBACTIVITIY_ID_STRINGS[] =
{
    "SUBACTIVITIY_ID_NONE",
    "SUBACTIVITIY_ID_INIT_COMMUNICATION",
    "SUBACTIVITIY_ID_INIT_SYSIOCTL",
    "SUBACTIVITIY_ID_ACTIVATE_BATTERY_MONITOR",
    "SUBACTIVITIY_ID_DEACTIVATE_BATTERY_MONITOR",
    "SUBACTIVITIY_ID_ACTIVATE_MOTION_SENSOR",
    "SUBACTIVITIY_ID_DEACTIVATE_MOTION_SENSOR",
    "SUBACTIVITIY_ID_TRANSMIT_BLE_PACKET"
};
#define APPLICATION_SUBACTIVITY_ID_STRINGS SUBACTIVITIY_ID_STRINGS
#endif

typedef enum
{
    QUEUE_ID_HW_INTERRUPTS = 0,
    QUEUE_ID_COMM_MESSAGES,
    QUEUE_ID_RUNTIME_MESSAGES,
    QUEUE_ID_APPLICATION_MESSAGES,
    NUM_QUEUE_ID
} QUEUE_ID, APPLICATION_QUEUE_ID, application_queue_id_t;
#define NUM_APPLICATION_QUEUE_ID NUM_QUEUE_ID

#ifdef DEBUG
static const char * QUEUE_ID_STRINGS[] =
{
    "QUEUE_ID_HW_INTERRUPTS",
    "QUEUE_ID_COMM_MESSAGES",
    "QUEUE_ID_RUNTIME_MESSAGES",
    "QUEUE_ID_APPLICATION_MESSAGES"
};
#define APPLICATION_QUEUE_ID_STRINGS QUEUE_ID_STRINGS
#endif

typedef enum
{
    TASK_ID_NONE = 0,
    TASK_ID_POLL_BATTERY_MONITOR,
    TASK_ID_POLL_MOTION_SENSOR,
    TASK_ID_RECEIVE_BLE_PACKET,
    NUM_TASK_ID
} TASK_ID, APPLICATION_TASK_ID, application_task_id_t;
#define NUM_APPLICATION_TASK_ID NUM_TASK_ID

typedef enum
{
    TASK_ACTION_NONE = 0,
    TASK_ACTION_INTERRUPT,
    TASK_ACTION_SCHEDULE,
    TASK_ACTION_QUEUE,
    NUM_TASK_ACTION
} TASK_ACTION, APPLICATION_TASK_ACTION, application_task_action_t;
#define NUM_APPLICATION_TASK_ACTION NUM_TASK_ACTION

typedef enum
{
    TASK_PRIORITY_EXECUTIVE = 0,
    TASK_PRIORITY_HIGH,
    TASK_PRIORITY_MEDIUM,
    TASK_PRIORITY_LOW,
    TASK_PRIORITY_PASSIVE,
    TASK_PRIORITY_QUARANTINE,
    NUM_TASK_PRIORITY
} TASK_PRIORITY, APPLICATION_TASK_PRIORITY, application_task_priority_t;
#define NUM_APPLICATION_TASK_PRIORITY NUM_TASK_PRIORITY

#ifdef DEBUG
static const char * TASK_ID_STRINGS[] =
{
    "TASK_ID_NONE",
    "TASK_ID_POLL_BATTERY_MONITOR",
    "TASK_ID_POLL_MOTION_SENSOR",
    "TASK_ID_RECEIVE_BLE_PACKET"
};
#define APPLICATION_TASK_ID_STRINGS TASK_ID_STRINGS
#endif

#ifdef DEBUG
static const char * TASK_ACTION_STRINGS[] =
{
    "TASK_ACTION_NONE",
    "TASK_ACTION_INTERRUPT",
    "TASK_ACTION_SCHEDULE",
    "TASK_ACTION_QUEUE"
};
#define APPLICATION_TASK_ACTION_STRINGS TASK_ACTION_STRINGS
#endif

#ifdef DEBUG
static const char * TASK_PRIORITY_STRINGS[] =
{
    "TASK_PRIORITY_EXECUTIVE",
    "TASK_PRIORITY_HIGH",
    "TASK_PRIORITY_MEDIUM",
    "TASK_PRIORITY_LOW",
    "TASK_PRIORITY_PASSIVE",
    "TASK_PRIORITY_QUARANTINE"
};
#define APPLICATION_TASK_PRIORITY_STRINGS TASK_PRIORITY_STRINGS
#endif

typedef enum
{
    TASK_SHELF_ID_NONE = 0,
    TASK_SHELF_ID_SENSOR_MOTION,
    TASK_SHELF_ID_SENSOR_BATTERY_MONITOR,
    TASK_SHELF_ID_DUAL_BLE_RADIO,
    TASK_SHELF_ID_WAIT_FOR_BLE_RADIO,
    NUM_TASK_SHELF_ID
} TASK_SHELF_ID, APPLICATION_TASK_SHELF_ENTRY_ID, application_task_shelf_entry_id_t;
#define NUM_APPLICATION_TASK_SHELF_ENTRY_ID NUM_TASK_SHELF_ID

#ifdef DEBUG
static const char * TASK_SHELF_ID_STRINGS[] =
{
    "TASK_SHELF_ID_NONE",
    "TASK_SHELF_ID_SENSOR_MOTION",
    "TASK_SHELF_ID_SENSOR_BATTERY_MONITOR",
    "TASK_SHELF_ID_DUAL_BLE_RADIO",
    "TASK_SHELF_ID_WAIT_FOR_BLE_RADIO"
};
#define APPLICATION_TASK_SHELF_ENTRY_ID_STRINGS TASK_SHELF_ID_STRINGS
#endif

typedef enum
{
    STATE_NAME_STARTUP = 0,
    STATE_NAME_IDLE,
    STATE_NAME_ACTIVE,
    STATE_NAME_ASLEEP,
    STATE_NAME_ERROR,
    STATE_NAME_RECOVERY,
    STATE_NAME_UNKNOWN,
    NUM_STATE_NAME
} STATE_NAME, APPLICATION_STATE, application_state_t;
#define NUM_APPLICATION_STATE NUM_STATE_NAME

typedef enum
{
    STATE_ACTIVITY_NONE = 0,
    STATE_ACTIVITY_STARTUP,
    STATE_ACTIVITY_ACTIVATE,
    STATE_ACTIVITY_WAIT,
    STATE_ACTIVITY_SLEEP,
    NUM_STATE_ACTIVITY
} STATE_ACTIVITY, APPLICATION_ACTIVITY, application_activity_t;
#define NUM_APPLICATION_ACTIVITY NUM_STATE_ACTIVITY

#ifdef DEBUG
static const char * STATE_NAME_STRINGS[] =
{
    "STATE_NAME_STARTUP",
    "STATE_NAME_IDLE",
    "STATE_NAME_ACTIVE",
    "STATE_NAME_ASLEEP",
    "STATE_NAME_ERROR",
    "STATE_NAME_RECOVERY",
    "STATE_NAME_UNKNOWN"
};
#define APPLICATION_STATE_STRINGS STATE_NAME_STRINGS
#endif

#ifdef DEBUG
static const char * STATE_ACTIVITY_STRINGS[] =
{
    "STATE_ACTIVITY_NONE",
    "STATE_ACTIVITY_STARTUP",
    "STATE_ACTIVITY_ACTIVATE",
    "STATE_ACTIVITY_WAIT",
    "STATE_ACTIVITY_SLEEP"
};
#define APPLICATION_ACTIVITY_STRINGS STATE_ACTIVITY_STRINGS
#endif


/* Families */
#define FAMILY_IDLE { COMPONENT_FAMILY_A }
#define FAMILY_ALL { COMPONENT_FAMILY_B }
#define FAMILY_ERROR { COMPONENT_FAMILY_NONE }

/* Channels */
#define CHANNEL_I2C { I2C1 }
#define CHANNEL_SPI { SPI1 }
#define CHANNEL_USART { USART1 }

/* Components */
#define STC310_NAME "STC310"
#define STC310_ID COMPONENT_ID_BATTERY_MONITOR
#define STC310_FAMILY COMPONENT_FAMILY_B
#define STC310_PROTOCOL COMPONENT_PROTOCOL_I2C
#define STC310_ROUTE COMPONENT_ROUTE_PRIMARY
#define STC310_ADDR 0x70
#define STC310_PORT COMPONENT_PORT_A
#define STC310_PIN 1
#define STC310_STATE COMPONENT_STATE_ON
#define STC310_COMPONENT { STC310_NAME, STC310_ID, STC310_FAMILY, STC310_PROTOCOL, STC310_ROUTE, STC310_ADDR, STC310_PORT, STC310_PIN, STC310_STATE }

#define BNO080_NAME "BNO080"
#define BNO080_ID COMPONENT_ID_MOTION_SENSOR
#define BNO080_FAMILY COMPONENT_FAMILY_A
#define BNO080_PROTOCOL COMPONENT_PROTOCOL_SPI
#define BNO080_ROUTE COMPONENT_ROUTE_PRIMARY
#define BNO080_ADDR 0x4B
#define BNO080_PORT COMPONENT_PORT_A
#define BNO080_PIN 2
#define BNO080_STATE COMPONENT_STATE_ON
#define BNO080_COMPONENT { BNO080_NAME, BNO080_ID, BNO080_FAMILY, BNO080_PROTOCOL, BNO080_ROUTE, BNO080_ADDR, BNO080_PORT, BNO080_PIN, BNO080_STATE }

#define BLE_NAME "BLE"
#define BLE_ID COMPONENT_ID_BLE_RADIO
#define BLE_FAMILY COMPONENT_FAMILY_0
#define BLE_PROTOCOL COMPONENT_PROTOCOL_BLE
#define BLE_ROUTE COMPONENT_ROUTE_PRIMARY
#define BLE_ADDR NONE
#define BLE_PORT COMPONENT_PORT_NONE
#define BLE_PIN INTERNAL
#define BLE_STATE COMPONENT_STATE_ON
#define BLE_COMPONENT { BLE_NAME, BLE_ID, BLE_FAMILY, BLE_PROTOCOL, BLE_ROUTE, BLE_ADDR, BLE_PORT, BLE_PIN, BLE_STATE }

/* Subactivities */
#define SUBACTIVITIY_INIT_COMMUNICATION { \
    .ID = SUBACTIVITIY_ID_INIT_COMMUNICATION, \
    .component_id = { COMPONENT_ID_BLE_RADIO }, \
    .num_component_id = 1, \
    .function = CommFunctions.Init, \
    .data = NONE \
}

#define SUBACTIVITIY_INIT_SYSIOCTL { \
    .ID = SUBACTIVITIY_ID_INIT_SYSIOCTL, \
    .component_id = NONE, \
    .num_component_id = NONE, \
    .function = SysIOCtlFunctions.Init, \
    .data = &System \
}

#define SUBACTIVITIY_ACTIVATE_BATTERY_MONITOR { \
    .ID = SUBACTIVITIY_ID_ACTIVATE_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1, \
    .function = BatteryMonitor.Set, \
    .data = ACTIVE \
}

#define SUBACTIVITIY_DEACTIVATE_BATTERY_MONITOR { \
    .ID = SUBACTIVITIY_ID_DEACTIVATE_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1, \
    .function = BatteryMonitor.Set, \
    .data = INACTIVE \
}

#define SUBACTIVITIY_ACTIVATE_MOTION_SENSOR { \
    .ID = SUBACTIVITIY_ID_ACTIVATE_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1, \
    .function = AppFunctions.MotionState, \
    .data = ACTIVE \
}

#define SUBACTIVITIY_DEACTIVATE_MOTION_SENSOR { \
    .ID = SUBACTIVITIY_ID_DEACTIVATE_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1, \
    .function = AppFunctions.MotionState, \
    .data = INACTIVE \
}

#define SUBACTIVITIY_TRANSMIT_BLE_PACKET { \
    .ID = SUBACTIVITIY_ID_TRANSMIT_BLE_PACKET, \
    .component_id = { COMPONENT_ID_BLE_RADIO }, \
    .num_component_id = 1, \
    .function = CommFunctions.Perform.Transmit, \
    .data = &App.buffers.sub_packet_out \
}

/* Queues */
#define QUEUE_HW_INTERRUPTS QUEUE( \
    QUEUE_ID_HW_INTERRUPTS, \
    DEFAULT_QUEUE_MAX_QTY, \
    DEFAULT_QUEUE_TIMEOUT_MS, \
    &System.error.interrupt \
)

#define QUEUE_COMM_MESSAGES QUEUE( \
    QUEUE_ID_COMM_MESSAGES, \
    DEFAULT_QUEUE_MAX_QTY, \
    DEFAULT_QUEUE_TIMEOUT_MS, \
    &System.error.messaging \
)

#define QUEUE_RUNTIME_MESSAGES QUEUE( \
    QUEUE_ID_RUNTIME_MESSAGES, \
    DEFAULT_QUEUE_MAX_QTY, \
    DEFAULT_QUEUE_TIMEOUT_MS, \
    &System.error.messaging \
)

#define QUEUE_APPLICATION_MESSAGES QUEUE( \
    QUEUE_ID_APPLICATION_MESSAGES, \
    DEFAULT_QUEUE_MAX_QTY, \
    DEFAULT_QUEUE_TIMEOUT_MS, \
    &System.error.messaging \
)

/* Tasks */
#define TASK_POLL_BATTERY_MONITOR { \
    .ID = TASK_ID_POLL_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1, \
    .data = 10, \
    .ACTION = TASK_ACTION_SCHEDULE, \
    .function = BatteryMonitor.GetBasic, \
    .object = &App.buffers.battery, \
    .PRIORITY = TASK_PRIORITY_LOW, \
    .error = &System.error.peripheral \
}

#define TASK_POLL_MOTION_SENSOR { \
    .ID = TASK_ID_POLL_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1, \
    .data = 1, \
    .ACTION = TASK_ACTION_INTERRUPT, \
    .function = IMUFunctions.Read, \
    .object = &App.objects.IMU.client, \
    .PRIORITY = TASK_PRIORITY_MEDIUM, \
    .error = &System.error.system \
}

#define TASK_RECEIVE_BLE_PACKET { \
    .ID = TASK_ID_RECEIVE_BLE_PACKET, \
    .component_id = { COMPONENT_ID_BLE_RADIO }, \
    .num_component_id = 1, \
    .data = NONE, \
    .ACTION = TASK_ACTION_INTERRUPT, \
    .function = CommFunctions.Perform.Receive, \
    .object = &App.buffers.sub_packet_in, \
    .PRIORITY = TASK_PRIORITY_HIGH, \
    .error = &System.error.peripheral \
}

/* Task Shelf */
#define TASK_SHELF_SENSOR_MOTION { \
    .ID = TASK_SHELF_ID_SENSOR_MOTION, \
    .interrupts = NONE, \
    .num_interrupts = NONE, \
    .scheduled = { TASK_ID_POLL_MOTION_SENSOR }, \
    .num_scheduled = 1 \
}

#define TASK_SHELF_SENSOR_BATTERY_MONITOR { \
    .ID = TASK_SHELF_ID_SENSOR_BATTERY_MONITOR, \
    .interrupts = NONE, \
    .num_interrupts = NONE, \
    .scheduled = { TASK_ID_POLL_BATTERY_MONITOR }, \
    .num_scheduled = 1 \
}

#define TASK_SHELF_WAIT_FOR_BLE_RADIO { \
    .ID = TASK_SHELF_ID_WAIT_FOR_BLE_RADIO, \
    .interrupts = { TASK_ID_RECEIVE_BLE_PACKET }, \
    .num_interrupts = 1, \
    .scheduled = NONE, \
    .num_scheduled = NONE \
}

/* States */
#define PROFILE_TEMPLATE \
{ \
    { /* COMPONENT */ \
        3, \
        { \
            STC310_COMPONENT, \
            BNO080_COMPONENT, \
            BLE_COMPONENT \
        } \
    },  \
    { /* SUBACTIVITIY */ \
        7, \
        { \
            SUBACTIVITIY_INIT_COMMUNICATION, \
            SUBACTIVITIY_INIT_SYSIOCTL, \
            SUBACTIVITIY_ACTIVATE_BATTERY_MONITOR, \
            SUBACTIVITIY_DEACTIVATE_BATTERY_MONITOR, \
            SUBACTIVITIY_ACTIVATE_MOTION_SENSOR, \
            SUBACTIVITIY_DEACTIVATE_MOTION_SENSOR, \
            SUBACTIVITIY_TRANSMIT_BLE_PACKET \
        } \
    },  \
    { /* QUEUE */ \
        4, \
        { \
            QUEUE_HW_INTERRUPTS, \
            QUEUE_COMM_MESSAGES, \
            QUEUE_RUNTIME_MESSAGES, \
            QUEUE_APPLICATION_MESSAGES \
        } \
    },  \
    { /* TASK */ \
        3, \
        { \
            TASK_POLL_BATTERY_MONITOR, \
            TASK_POLL_MOTION_SENSOR, \
            TASK_RECEIVE_BLE_PACKET \
        } \
    },  \
    { /* TASK_SHELF */ \
        3, \
        { \
            TASK_SHELF_SENSOR_MOTION, \
            TASK_SHELF_SENSOR_BATTERY_MONITOR, \
            TASK_SHELF_WAIT_FOR_BLE_RADIO \
        } \
    },  \
    { /* STATE */ \
        { \
            { \
                STATE_ACTIVITY_STARTUP, /* STATE_ACTIVITY */ \
                { /* SUBACTIVITIES */ \
                    SUBACTIVITIY_ID_INIT_SYSIOCTL, \
                    SUBACTIVITIY_ID_INIT_COMMUNICATION \
                }, \
                2, /* num_SUBACTIVITIES */ \
                STATE_NAME_ACTIVE, /* EXIT_STATE */ \
            }, \
            STATE_NAME_STARTUP, /* STATE_NAME */ \
            FAMILY_ALL, /* FAMILIES */ \
            { /* TASK_SHELF */ \
                TASK_SHELF_ID_NONE \
            }, \
            1, /* num_TASK_SHELF */ \
        }, \
        { \
            { \
                STATE_ACTIVITY_ACTIVATE, /* STATE_ACTIVITY */ \
                { /* SUBACTIVITIES */ \
                    SUBACTIVITIY_ID_ACTIVATE_BATTERY_MONITOR, \
                    SUBACTIVITIY_ID_ACTIVATE_MOTION_SENSOR \
                }, \
                2, /* num_SUBACTIVITIES */ \
                STATE_NAME_ASLEEP, /* EXIT_STATE */ \
            }, \
            STATE_NAME_ACTIVE, /* STATE_NAME */ \
            FAMILY_ALL, /* FAMILIES */ \
            { /* TASK_SHELF */ \
                TASK_SHELF_ID_DUAL_BLE_RADIO, \
                TASK_SHELF_ID_SENSOR_BATTERY_MONITOR, \
                TASK_SHELF_ID_SENSOR_MOTION \
            }, \
            3, /* num_TASK_SHELF */ \
        }, \
        { \
            { \
                STATE_ACTIVITY_SLEEP, /* STATE_ACTIVITY */ \
                { /* SUBACTIVITIES */ \
                    SUBACTIVITIY_ID_DEACTIVATE_BATTERY_MONITOR, \
                    SUBACTIVITIY_ID_DEACTIVATE_MOTION_SENSOR \
                }, \
                2, /* num_SUBACTIVITIES */ \
                STATE_NAME_IDLE, /* EXIT_STATE */ \
            }, \
            STATE_NAME_ASLEEP, /* STATE_NAME */ \
            FAMILY_IDLE, /* FAMILIES */ \
            { /* TASK_SHELF */ \
                TASK_SHELF_ID_WAIT_FOR_BLE_RADIO \
            }, \
            1, /* num_TASK_SHELF */ \
        }, \
        { \
            { \
                STATE_ACTIVITY_WAIT, /* STATE_ACTIVITY */ \
                { /* SUBACTIVITIES */ \
                }, \
                0, /* num_SUBACTIVITIES */ \
                STATE_NAME_ACTIVE, /* EXIT_STATE */ \
            }, \
            STATE_NAME_IDLE, /* STATE_NAME */ \
            FAMILY_IDLE, /* FAMILIES */ \
            { /* TASK_SHELF */ \
                TASK_SHELF_ID_DUAL_BLE_RADIO \
            }, \
            1, /* num_TASK_SHELF */ \
        } \
    } \
};

#endif /*C12_profile_h */
