//
//  sercom.cpp
//  combine_core
//
//  Created by Matthew Fonken on 12/7/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "sercom.h"

void Init_SERCOM( int * filestream, const char * port, const char * port_alt)
{
    int fd;
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    if (fd == -1)
    {
        printf("Trying alternate port at %s\n", port_alt);
        fd = open(port_alt, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1)
            printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        else
            printf("Initializing SERCOM at %s: success on alternate.\n", port);
    }
    else printf("Initializing SERCOM at %s(%d): success on main.\n", port, fd);
    struct termios options;
    tcgetattr(fd, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    *filestream = fd;
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
