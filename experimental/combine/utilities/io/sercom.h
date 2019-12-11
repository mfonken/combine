//
//  sercom.hpp
//  combine_core
//
//  Created by Matthew Fonken on 12/7/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef sercom_h
#define sercom_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
    
#define UTILITY_VERBOSE
//#define PACKET_DEBUG

#define DEFAULT_SERCOM_PORT     "/dev/tty.usbmodem141401"
#define DEFAULT_SERCOM_PORT_ALT "/dev/tty.usbmodem143401"
#define DEFAULT_SERCOM_PORT_LEN sizeof(DEFAULT_SERCOM_PORT)

typedef struct
{
    int filestream;
    const char *port;
    const char *port_alt;
    int baud;
    int bits;
    struct termios options;
    bool initialized;
} SERCOM_Channel;
    
int Init_SERCOM_Default( void );
int Init_SERCOM( SERCOM_Channel * sc );
void Write_SERCOM_Byte( int filestream, char b );
void Write_SERCOM_Bytes( int filestream, const char * a, int l );
int Read_SERCOM_Bytes( int filestream, char * a, int l );
int Read_SERCOM_Byte( int filestream, char * b );
int Test_SERCOM( int filestream );
void tokenifyPacket( char * a, int n, double * d);
char** str_split(char* a_str, const char a_delim);
    
#ifdef __cplusplus
}
#endif
        
#endif /* sercom_hpp */
