// ********************** DO NOT EDIT - AUTO-GENERATED ********************** //
// C12_profile.h

// Created by Combine Profile Generator v0.1 on 2/17/2020
// Copyright Â© Marbl. All rights reserved.

#ifndef C12_profile_h
#define C12_profile_h

#define NONE 0

typedef enum
{
    COMPONENT_ID_NONE = 0,
    COMPONENT_ID_MOTION_SENSOR,
    COMPONENT_ID_BATTERY_MONITOR,
    COMPONENT_ID_BLE_RADIO_PRIMARY,
    NUM_COMPONENT_ID
} COMPONENT_ID;

typedef enum
{
    COMPONENT_FAMILY_NONE = 0,
    COMPONENT_FAMILY_A,
    COMPONENT_FAMILY_B,
    NUM_COMPONENT_FAMILY
} COMPONENT_FAMILY;

typedef enum
{
    COMPONENT_PROTOCOL_NONE = 0,
    COMPONENT_PROTOCOL_I2C,
    COMPONENT_PROTOCOL_SPI,
    COMPONENT_PROTOCOL_UART,
    COMPONENT_PROTOCOL_BLE,
    NUM_COMPONENT_PROTOCOL
} COMPONENT_PROTOCOL;

typedef enum
{
    COMPONENT_ROUTE_NONE = 0,
    COMPONENT_ROUTE_PRIMARY,
    NUM_COMPONENT_ROUTE
} COMPONENT_ROUTE;

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
} COMPONENT_PORT;

typedef enum
{
    COMPONENT_PIN_INTERNAL = 0,
    NUM_COMPONENT_PIN
} COMPONENT_PIN;

typedef enum
{
    COMPONENT_STATE_OFF = 0,
    COMPONENT_STATE_ON,
    COMPONENT_STATE_Z,
    COMPONENT_STATE_INTERRUPT,
    NUM_COMPONENT_STATE
} COMPONENT_STATE;

typedef enum
{
    SUBACTIVITY_ID_INIT_COMMUNICATION = 0,
    SUBACTIVITY_ID_INIT_COMPONENTS,
    SUBACTIVITY_ID_POLL_BATTERY_MONITOR,
    SUBACTIVITY_ID_ACTIVATE_BATTERY_MONITOR,
    SUBACTIVITY_ID_DEACTIVATE_BATTERY_MONITOR,
    SUBACTIVITY_ID_POLL_MOTION_SENSOR,
    SUBACTIVITY_ID_ACTIVATE_MOTION_SENSOR,
    SUBACTIVITY_ID_DEACTIVATE_MOTION_SENSOR,
    SUBACTIVITY_ID_TRANSMIT_BLE_PACKET,
    SUBACTIVITY_ID_RECEIVE_BLE_PACKET,
    NUM_SUBACTIVITY_ID
} SUBACTIVITY_ID;

typedef enum
{
    TASK_ID_NONE = 0,
    TASK_ID_SENSOR_MOTION,
    TASK_ID_SENSOR_BATTERY_MONITOR,
    TASK_ID_DUAL_BLE_RADIO,
    TASK_ID_WAIT_FOR_BLE_RADIO,
    NUM_TASK_ID
} TASK_ID;

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
} STATE_NAME;

typedef enum
{
    STATE_ACTIVITY_NONE = 0,
    STATE_ACTIVITY_STARTUP,
    STATE_ACTIVITY_ACTIVATE,
    STATE_ACTIVITY_WAIT,
    STATE_ACTIVITY_SLEEP,
    NUM_STATE_ACTIVITY
} STATE_ACTIVITY;


/* Families */
#define FAMILY_IDLE { COMPONENT_FAMILY_A }
#define FAMILY_ALL { COMPONENT_FAMILY_B }
#define FAMILY_ERROR { COMPONENT_FAMILY_NONE }

/* Channels */
#define CHANNEL_I2C { I2C1 }
#define CHANNEL_SPI { SPI1 }
#define CHANNEL_USART { USART1 }

/* Components */
#define STC310_ID COMPONENT_ID_BATTERY_MONITOR
#define STC310_FAMILY COMPONENT_FAMILY_B
#define STC310_PROTOCOL COMPONENT_PROTOCOL_I2C
#define STC310_ROUTE COMPONENT_ROUTE_PRIMARY
#define STC310_ADDR COMPONENT_ADDR_0x70
#define STC310_PORT COMPONENT_PORT_A
#define STC310_PIN COMPONENT_PIN_INTERNAL
#define STC310_STATE COMPONENT_STATE_ON
#define STC310_COMPONENT { STC310_ID, STC310_FAMILY, STC310_PROTOCOL, STC310_ROUTE, STC310_ADDR, STC310_PORT, STC310_PIN, STC310_STATE }

#define BNO080_ID COMPONENT_ID_MOTION_SENSOR
#define BNO080_FAMILY COMPONENT_FAMILY_A
#define BNO080_PROTOCOL COMPONENT_PROTOCOL_SPI
#define BNO080_ROUTE COMPONENT_ROUTE_PRIMARY
#define BNO080_ADDR COMPONENT_ADDR_0x4B
#define BNO080_PORT COMPONENT_PORT_A
#define BNO080_PIN COMPONENT_PIN_INTERNAL
#define BNO080_STATE COMPONENT_STATE_ON
#define BNO080_COMPONENT { BNO080_ID, BNO080_FAMILY, BNO080_PROTOCOL, BNO080_ROUTE, BNO080_ADDR, BNO080_PORT, BNO080_PIN, BNO080_STATE }

/* Subactivities */
#define SUBACTIVITY_INIT_COMMUNICATION { \
    .ID = SUBACTIVITY_ID_INIT_COMMUNICATION, \
    .component_id = { COMPONENT_ID_BLE_RADIO_PRIMARY }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_INIT_COMPONENTS { \
    .ID = SUBACTIVITY_ID_INIT_COMPONENTS, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR, COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 2 \
}

#define SUBACTIVITY_ACTIVATE_BATTERY_MONITOR { \
    .ID = SUBACTIVITY_ID_ACTIVATE_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_DEACTIVATE_BATTERY_MONITOR { \
    .ID = SUBACTIVITY_ID_DEACTIVATE_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_POLL_MOTION_SENSOR { \
    .ID = SUBACTIVITY_ID_POLL_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_ACTIVATE_MOTION_SENSOR { \
    .ID = SUBACTIVITY_ID_ACTIVATE_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_DEACTIVATE_MOTION_SENSOR { \
    .ID = SUBACTIVITY_ID_DEACTIVATE_MOTION_SENSOR, \
    .component_id = { COMPONENT_ID_MOTION_SENSOR }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_TRANSMIT_BLE_PACKET { \
    .ID = SUBACTIVITY_ID_TRANSMIT_BLE_PACKET, \
    .component_id = { COMPONENT_ID_BLE_RADIO_PRIMARY }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_RECEIVE_BLE_PACKET { \
    .ID = SUBACTIVITY_ID_RECEIVE_BLE_PACKET, \
    .component_id = { COMPONENT_ID_BLE_RADIO_PRIMARY }, \
    .num_component_id = 1 \
}

#define SUBACTIVITY_POLL_BATTERY_MONITOR { \
    .ID = SUBACTIVITY_ID_POLL_BATTERY_MONITOR, \
    .component_id = { COMPONENT_ID_BATTERY_MONITOR }, \
    .num_component_id = 1 \
}

/* Task */
#define TASK_SENSOR_MOTION { \
    .ID = TASK_ID_SENSOR_MOTION, \
    .interrupts = { SUBACTIVITY_ID_DEACTIVATE_MOTION_SENSOR, SUBACTIVITY_ID_ACTIVATE_MOTION_SENSOR }, \
    .num_interrupts = 2, \
    .scheduled = { SUBACTIVITY_ID_POLL_MOTION_SENSOR }, \
    .num_scheduled = 1 \
}

#define TASK_SENSOR_BATTERY_MONITOR { \
    .ID = TASK_ID_SENSOR_BATTERY_MONITOR, \
    .interrupts = { SUBACTIVITY_ID_DEACTIVATE_BATTERY_MONITOR, SUBACTIVITY_ID_ACTIVATE_BATTERY_MONITOR }, \
    .num_interrupts = 2, \
    .scheduled = { SUBACTIVITY_ID_POLL_BATTERY_MONITOR }, \
    .num_scheduled = 1 \
}

#define TASK_DUAL_BLE_RADIO { \
    .ID = TASK_ID_DUAL_BLE_RADIO, \
    .interrupts = { SUBACTIVITY_ID_RECEIVE_BLE_PACKET }, \
    .num_interrupts = 1, \
    .scheduled = { SUBACTIVITY_ID_TRANSMIT_BLE_PACKET }, \
    .num_scheduled = 1 \
}

#define TASK_WAIT_FOR_BLE_RADIO { \
    .ID = TASK_ID_WAIT_FOR_BLE_RADIO, \
    .interrupts = { SUBACTIVITY_ID_TRANSMIT_BLE_PACKET }, \
    .num_interrupts = 1, \
    .scheduled = NONE, \
    .num_scheduled = NONE \
}

/* States */
#define PROFILE_TEMPLATE \
{ \
    { /* COMPONENT */ \
        2, \
        { \
            STC310_COMPONENT, \
            BNO080_COMPONENT \
        } \
    },  \
    { /* TASK */ \
        4, \
        { \
            TASK_SENSOR_MOTION, \
            TASK_SENSOR_BATTERY_MONITOR, \
            TASK_DUAL_BLE_RADIO, \
            TASK_WAIT_FOR_BLE_RADIO \
        } \
    },  \
    { /* STATE */ \
        { \
            { \
                ACTIVITY_STARTUP, /* ACTIVITY */\
                { /* SUBACTIVITIES */ \
                    SUBACTIVITY_ID_INIT_COMPONENTS, \
                    SUBACTIVITY_ID_INIT_COMMUNICATION \
                }, \
                2, /* num_SUBACTIVITIES */\
                STATE_ACTIVE, /* EXIT_STATE */\
            }, \
            STATE_STARTUP, /* NAME */ \
            FAMILY_ALL, /* FAMILIES */ \
            { /* TASKS */ \
                TASKS_NONE \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                ACTIVITY_ACTIVATE, /* ACTIVITY */\
                { /* SUBACTIVITIES */ \
                    SUBACTIVITY_ID_ACTIVATE_BATTERY_MONITOR, \
                    SUBACTIVITY_ID_ACTIVATE_MOTION_SENSOR \
                }, \
                2, /* num_SUBACTIVITIES */\
                STATE_ASLEEP, /* EXIT_STATE */\
            }, \
            STATE_ACTIVE, /* NAME */ \
            FAMILY_ALL, /* FAMILIES */ \
            { /* TASKS */ \
                TASKS_DUAL_BLE_RADIO, \
                TASKS_SENSOR_BATTERY_MONITOR, \
                TASKS_SENSOR_MOTION \
            }, \
            3, /* num_TASKS */ \
        }, \
        { \
            { \
                ACTIVITY_SLEEP, /* ACTIVITY */\
                { /* SUBACTIVITIES */ \
                    SUBACTIVITY_ID_DEACTIVATE_BATTERY_MONITOR, \
                    SUBACTIVITY_ID_DEACTIVATE_MOTION_SENSOR \
                }, \
                2, /* num_SUBACTIVITIES */\
                STATE_IDLE, /* EXIT_STATE */\
            }, \
            STATE_ASLEEP, /* NAME */ \
            FAMILY_IDLE, /* FAMILIES */ \
            { /* TASKS */ \
                TASKS_WAIT_FOR_BLE_RADIO \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                ACTIVITY_WAIT, /* ACTIVITY */\
                { /* SUBACTIVITIES */ \
                }, \
                0, /* num_SUBACTIVITIES */\
                STATE_ACTIVE, /* EXIT_STATE */\
            }, \
            STATE_IDLE, /* NAME */ \
            FAMILY_IDLE, /* FAMILIES */ \
            { /* TASKS */ \
                TASKS_DUAL_BLE_RADIO \
            }, \
            1, /* num_TASKS */ \
        } \
    } \
};

#endif /*C12_profile_h */
