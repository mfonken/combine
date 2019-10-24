//
//  sercom.cpp
//  combine_core
//
//  Created by Matthew Fonken on 12/7/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "sercom.h"

int Init_SERCOM_Default( void )
{
    SERCOM_Channel chan = {-1, DEFAULT_SERCOM_PORT, DEFAULT_SERCOM_PORT_ALT, B115200, CS8, 0};
    return Init_SERCOM(&chan);
}
    
int Init_SERCOM( SERCOM_Channel * chan )
{
    chan->initialized = false;
    chan->filestream = open(chan->port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    if (chan->filestream == -1)
    {
#ifdef UTILITY_VERBOSE
        printf("Trying alternate port at %s\n", chan->port_alt);
#endif
        chan->filestream = open(chan->port_alt, O_RDWR | O_NOCTTY | O_NDELAY);
        if (chan->filestream == -1)
        {
#ifdef UTILITY_VERBOSE
            printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
#endif
            return chan->filestream;
        }
        else
        {
#ifdef UTILITY_VERBOSE
            printf("Initializing SERCOM at %s: success on alternate.\n", chan->port);
#endif
        }
    }
#ifdef UTILITY_VERBOSE
    else
    {
        printf("Initializing SERCOM at %s(%d): success on main.\n", chan->port, chan->filestream);
    }
#endif
    
    tcgetattr(chan->filestream, &chan->options);
    chan->options.c_cflag = chan->baud | chan->bits | CLOCAL | CREAD;
    chan->options.c_iflag = IGNPAR;
    chan->options.c_oflag = 0;
    chan->options.c_lflag = 0;
    tcflush(chan->filestream, TCIFLUSH);
    tcsetattr(chan->filestream, TCSANOW, &chan->options);
    chan->initialized = true;
    return chan->filestream;
}

void Write_SERCOM_Byte( int filestream, char b )
{
    int byte_written = -1;
    while(byte_written <= 0) byte_written = (int)write(filestream, &b, 1);
}

void Write_SERCOM_Bytes( int filestream, const char * a, int l )
{
    int byte_written = -1;
    while(byte_written <= 0) byte_written = (int)write(filestream, a, (size_t)l);
}

int Read_SERCOM_Bytes( int filestream, char * a, int l )
{
    return (int)read(filestream, a, (size_t)l);
}

int Read_SERCOM_Byte( int filestream, char * b )
{
    return (int)read(filestream, b, 1);
}

int Test_SERCOM( int filestream )
{
    char t;
    return (int)read(filestream, &t, 1);
}

void tokenifyPacket( char * a, int n, double * d)
{
    char *token = strtok(a, ",");
    int i;
    for( i = 0; i < n && token != NULL; i++ )
    {
        d[i] = atof(token);
        token = strtok(NULL, ",");
    }
    if(token == NULL)
        d[0] = 0xffff;
}
