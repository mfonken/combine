//
//  bluetooth_serial.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef sercom_wrapper_hpp
#define sercom_wrapper_hpp

#include <stdio.h>
#include <string>

#include "sercom.h"

#define DEFAULT_HANDSHAKE_DELAY 1000000
#define DEFAULT_HANDSHAKE_ATTEMPTS  3
#define DEFAULT_HANDSHAKE_ID        "ab\r\n"
#define NO_HANDSHAKE                NULL

#define BUFFER_SIZE 256

typedef enum
{
    HANDSHAKE_FAILED = -1,
    HANDSHAKE_SUCCEEDED,
    HANDSHAKE_SKIPPED,
    INITIALIZATION_FAILED
} SERCOM_STATUS;

typedef enum
{
    USB = 0,
    BLUETOOTH
} SERCOM_TYPE;

class SERCOM
{
    SERCOM_Channel channel;
    
public:
    SERCOM( SERCOM_TYPE );
    SERCOM( SERCOM_TYPE, const char * );
    SERCOM_STATUS initUSB(const char *);
    SERCOM_STATUS initBluetooth(const char *);
    SERCOM_STATUS init(char *, char *, const char *);
    SERCOM_STATUS handshake( const char * );
    SERCOM_STATUS handshake( const char *, int, int );
    void write(std::string);
    std::string read(int);
};

#endif /* bluetooth_serial_hpp */
