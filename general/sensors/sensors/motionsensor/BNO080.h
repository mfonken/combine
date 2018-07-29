//
//  BNO080.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef BNO080_h
#define BNO080_h

#include "i2c_template.h"
#incldue "sh-2.h"

#define SHTP_HEADER_LENGTH 4
#define SHTP_MAX_PACKET_SIZE 128 //Packets can be up to 32k but we don't have that much RAM.
#define BNO080_MAX_METADATA_SIZE 9

typedef enum
{
    SHTP_EXECUTABLE_COMMAND_RESET = 1,
    SHTP_EXECUTABLE_COMMAND_ON,
    SHTP_EXECUTABLE_COMMAND_SLEEP
} shtp_executable_commands;

typedef enum
{
    SHTP_EXECUTABLE_RESPONSE_RESET_COMPLETE = 1
} shtp_executable_responses;

typedef enum
{
    SHTP_COMMAND = 0,
    SHTP_EXECUTABLE,
    SHTP_CONTROL,
    SHTP_INPUT_REPORT,
    SHTP_WAKE_REPORT,
    SHTP_ROTATION
} shtp_channel;

typedef struct
{
    uint16_t        length:15;
    shtp_channel    channel:8;
    uint8_t         seqnum:8;
} shtp_packet_t;

typedef struct
{
    shtp_packet_t
        packet;
    uint8_t
        data[SHTP_MAX_PACKET_SIZE];
    uint32_t
        metadata[BNO080_MAX_METADATA_SIZE];
} bno080_t;

//static i2c_event_t bno080GetReadAllI2CEvent(void) { return (i2c_event_t){ I2C_READ_REG_EVENT, CPT_ADDR, NO_REG, CPT_PACKET_LEN }; }
//
//typedef struct
//{
//    i2c_event_t (*imuGetReadAllEvent)(void);
//} bno080_functions;
//
//static bno080_functions CPTFunctions =
//{
//    .getReadEvent = cptGetReadEvent
//};

#endif /* BNO080_h */
