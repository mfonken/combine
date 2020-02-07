// ********************** DO NOT EDIT - AUTO-GENERATED ********************** //
// C12_profile.h

// Created by Combine Profile Generator v0.1 on 2/6/2020
// Copyright © Marbl. All rights reserved.

#ifndef C12_profile_h
#define C12_profile_h

#define NONE 0

#define Families_IDLE { SYSTEM_FAMILY_0 }
#define Families_ALL { SYSTEM_FAMILY_0, SYSTEM_FAMILY_A, SYSTEM_FAMILY_B, SYSTEM_FAMILY_C, SYSTEM_FAMILY_D }
#define Families_ERROR { SYSTEM_FAMILY_0, SYSTEM_FAMILY_B, SYSTEM_FAMILY_C }

#define EFR32_BLE_ID APPLICATION_COMPONENT_BLE_RADIO_PRIMARY
#define EFR32_BLE_FAMILY SYSTEM_FAMILY_A
#define EFR32_BLE_COMM COMM_BLE
#define EFR32_BLE_ROUTE COMM_ROUTE_PRIMARY
#define EFR32_BLE_ADDR COMM_ADDR_NONE
#define EFR32_BLE_PORT COMM_PORT_NONE
#define EFR32_BLE_PIN INTERNAL
#define EFR32_BLE_STATE COMPONENT_STATE_OFF
#define EFR32_BLE_Components { EFR32_BLE_ID, EFR32_BLE_FAMILY, EFR32_BLE_COMM, EFR32_BLE_ROUTE, EFR32_BLE_ADDR, EFR32_BLE_PORT, EFR32_BLE_PIN, EFR32_BLE_STATE }

#define EFR32_SUB_ID APPLICATION_COMPONENT_SUB_RADIO_PRIMARY
#define EFR32_SUB_FAMILY SYSTEM_FAMILY_B
#define EFR32_SUB_COMM COMM_SUB
#define EFR32_SUB_ROUTE COMM_ROUTE_PRIMARY
#define EFR32_SUB_ADDR COMM_ADDR_NONE
#define EFR32_SUB_PORT COMM_PORT_NONE
#define EFR32_SUB_PIN INTERNAL
#define EFR32_SUB_STATE COMPONENT_STATE_OFF
#define EFR32_SUB_Components { EFR32_SUB_ID, EFR32_SUB_FAMILY, EFR32_SUB_COMM, EFR32_SUB_ROUTE, EFR32_SUB_ADDR, EFR32_SUB_PORT, EFR32_SUB_PIN, EFR32_SUB_STATE }

#define BNO080_ID APPLICATION_COMPONENT_MOTION_PRIMARY
#define BNO080_FAMILY SYSTEM_FAMILY_B
#define BNO080_COMM COMM_SPI
#define BNO080_ROUTE COMM_ROUTE_PRIMARY
#define BNO080_ADDR COMM_ADDR_NONE
#define BNO080_PORT PORTC
#define BNO080_PIN 9
#define BNO080_STATE COMPONENT_STATE_OFF
#define BNO080_Components { BNO080_ID, BNO080_FAMILY, BNO080_COMM, BNO080_ROUTE, BNO080_ADDR, BNO080_PORT, BNO080_PIN, BNO080_STATE }

#define RHOMOD_ID APPLICATION_COMPONENT_RHO_MODULE_PRIMARY
#define RHOMOD_FAMILY SYSTEM_FAMILY_0
#define RHOMOD_COMM COMM_I2C
#define RHOMOD_ROUTE COMM_ROUTE_PRIMARY
#define RHOMOD_ADDR 0xee
#define RHOMOD_PORT PORTA
#define RHOMOD_PIN 2
#define RHOMOD_STATE COMPONENT_STATE_INTERRUPT
#define RHOMOD_Components { RHOMOD_ID, RHOMOD_FAMILY, RHOMOD_COMM, RHOMOD_ROUTE, RHOMOD_ADDR, RHOMOD_PORT, RHOMOD_PIN, RHOMOD_STATE }

#define STC310_ID APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY
#define STC310_FAMILY SYSTEM_FAMILY_C
#define STC310_COMM COMM_I2C
#define STC310_ROUTE COMM_ROUTE_PRIMARY
#define STC310_ADDR 0x00
#define STC310_PORT PORT0
#define STC310_PIN 0
#define STC310_STATE COMPONENT_STATE_INTERRUPT
#define STC310_Components { STC310_ID, STC310_FAMILY, STC310_COMM, STC310_ROUTE, STC310_ADDR, STC310_PORT, STC310_PIN, STC310_STATE }

#define XC9265_ID APPLICATION_COMPONENT_REGULATOR_1V5
#define XC9265_FAMILY SYSTEM_FAMILY_A
#define XC9265_COMM COMM_NONE
#define XC9265_ROUTE COMM_ROUTE_NONE
#define XC9265_ADDR COMM_ADDR_NONE
#define XC9265_PORT COMM_PORT_NONE
#define XC9265_PIN INTERNAL
#define XC9265_STATE COMPONENT_STATE_OFF
#define XC9265_Components { XC9265_ID, XC9265_FAMILY, XC9265_COMM, XC9265_ROUTE, XC9265_ADDR, XC9265_PORT, XC9265_PIN, XC9265_STATE }

#define APPLICATION_SCHEDULER_MOTION_INTERRUPT { \
    .ID = APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT, \
    .data = INTERRUPT_ACTION_IMMEDIATE, \
    .component_id = APPLICATION_COMPONENT_MOTION_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT \
}

#define APPLICATION_SCHEDULER_TOUCH_INTERRUPT { \
    .ID = APPLICATION_SCHEDULER_ID_TOUCH_INTERRUPT, \
    .data = 0, \
    .component_id = APPLICATION_COMPONENT_TOUCH_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_HANDLE_TOUCH_EVENT \
}

#define APPLICATION_SCHEDULER_BATTERY_MONITOR_POLL { \
    .ID = APPLICATION_SCHEDULER_ID_BATTERY_MONITOR_POLL, \
    .data = 1, \
    .component_id = APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_POLL_BATTERY_MONITOR \
}

#define APPLICATION_SCHEDULER_RHO_INTERRUPT { \
    .ID = APPLICATION_SCHEDULER_ID_RHO_INTERRUPT, \
    .data = INTERRUPT_ACTION_QUEUE, \
    .component_id = APPLICATION_COMPONENT_RHO_MODULE_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_HANDLE_RHO_EVENT \
}

#define APPLICATION_INTERRUPTER_TAU_PACKET_TRANSMIT { \
    .ID = APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT, \
    .data = 0, \
    .component_id = APPLICATION_COMPONENT_BLE_RADIO_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_TRANSMIT_HOST_PACKET \
}

#define APPLICATION_INTERRUPTER_TAU_PACKET_RECEIVE { \
    .ID = APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE, \
    .data = INTERRUPT_ACTION_QUEUE, \
    .component_id = APPLICATION_COMPONENT_SUB_RADIO_PRIMARY, \
    .handler_id = APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET \
}

#define APPLICATION_TASK_SHELF_ENTRY_NULL_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS, \
    .interrupts = NONE, \
    .num_interrupts = NONE, \
    .scheduled = NONE, \
    .num_scheduled = NONE \
}

#define APPLICATION_TASK_SHELF_ENTRY_GLOBAL_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS, \
    .interrupts = { APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT }, \
    .num_interrupts = 1, \
    .scheduled = NONE, \
    .num_scheduled = NONE \
}

#define APPLICATION_TASK_SHELF_ENTRY_SENSOR_MOTION_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS, \
    .interrupts = { APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT }, \
    .num_interrupts = 1, \
    .scheduled = NONE, \
    .num_scheduled = NONE \
}

#define APPLICATION_TASK_SHELF_ENTRY_SENSOR_BATTERY_MONITOR_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_BATTERY_MONITOR_TASKS, \
    .interrupts = NONE, \
    .num_interrupts = NONE, \
    .scheduled = { APPLICATION_PROBE_ID_BATTERY_MONITOR }, \
    .num_scheduled = 1 \
}

#define APPLICATION_TASK_SHELF_ENTRY_SENSOR_RHO_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_RHO_TASKS, \
    .interrupts = NONE, \
    .num_interrupts = NONE, \
    .scheduled = { APPLICATION_SCHEDULER_ID_RHO_INTERRUPT }, \
    .num_scheduled = 1 \
}

#define APPLICATION_TASK_SHELF_ENTRY_COMMUNICATION_HOST_RADIO_TASKS { \
    .ID = APPLICATION_TASK_SHELF_ENTRY_ID_COMMUNICATION_HOST_RADIO_TASKS, \
    .interrupts = { APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE }, \
    .num_interrupts = 1, \
    .scheduled = { APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT }, \
    .num_scheduled = 1 \
}

#define PROFILE_TEMPLATE \
{ \
    { /* Components */ \
        6, \
        { \
            EFR32_BLE_Components, \
            EFR32_SUB_Components, \
            BNO080_Components, \
            RHOMOD_Components, \
            STC310_Components, \
            XC9265_Components \
        } \
    },  \
    { /* Tasks */ \
        6, \
        { \
            APPLICATION_TASK_SHELF_ENTRY_NULL_TASKS, \
            APPLICATION_TASK_SHELF_ENTRY_GLOBAL_TASKS, \
            APPLICATION_TASK_SHELF_ENTRY_SENSOR_MOTION_TASKS, \
            APPLICATION_TASK_SHELF_ENTRY_SENSOR_BATTERY_MONITOR_TASKS, \
            APPLICATION_TASK_SHELF_ENTRY_SENSOR_RHO_TASKS, \
            APPLICATION_TASK_SHELF_ENTRY_COMMUNICATION_HOST_RADIO_TASKS \
        } \
    },  \
    { /* States */ \
        { \
            { \
                SYSTEM_ACTIVITY_STARTUP, \
                { /* SUBACTIVITIES */ \
                    APPLICATION_SUBACTIVITY_INIT_CONFIRM, \
                    APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT, \
                    APPLICATION_SUBACTIVITY_INIT_COMPONENTS, \
                    APPLICATION_SUBACTIVITY_INIT_COMMUNICATION, \
                    APPLICATION_SUBACTIVITY_SELF_CHECK \
                }, \
                5, \
                SYSTEM_STATE_IDLE, \
            }, \
            SYSTEM_STATE_STARTUP, /* STATE */ \
            Families_ALL, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_IDLE, \
            }, \
            SYSTEM_STATE_IDLE, /* STATE */ \
            Families_IDLE, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_IDLE, \
            }, \
            SYSTEM_STATE_WAITING, /* STATE */ \
            Families_IDLE, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_ACTIVE, \
            }, \
            SYSTEM_STATE_ACTIVE, /* STATE */ \
            Families_ALL, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS, \
                APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_TOUCH_TASKS \
            }, \
            2, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_IDLE, \
            }, \
            SYSTEM_STATE_ASLEEP, /* STATE */ \
            Families_IDLE, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_RECOVERY, \
            }, \
            SYSTEM_STATE_ERROR, /* STATE */ \
            Families_ERROR, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_RECOVERY, \
            }, \
            SYSTEM_STATE_RECOVERY, /* STATE */ \
            Families_ERROR, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        }, \
        { \
            { \
                SYSTEM_ACTIVITY_NONE, \
                { /* SUBACTIVITIES */ \
                }, \
                0, \
                SYSTEM_STATE_ERROR, \
            }, \
            SYSTEM_STATE_UNKNOWN, /* STATE */ \
            Families_ERROR, /* FAMILIES */ \
            { /* TASKS */ \
                APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS \
            }, \
            1, /* num_TASKS */ \
        } \
    } \
};

#endif /*C12_profile_h */
