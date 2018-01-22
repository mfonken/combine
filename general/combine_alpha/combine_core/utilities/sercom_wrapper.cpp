//
//  bluetooth_serial.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sercom_wrapper.hpp"

SERCOM::SERCOM( SERCOM_TYPE type )
{
    SERCOM(type, NO_HANDSHAKE);
}

SERCOM::SERCOM( SERCOM_TYPE type, const char * handshake_id )
{
    SERCOM_STATUS ret;
    switch(type)
    {
        default:
        case USB:
            ret = initUSB(handshake_id);
            break;
        case BLUETOOTH:
            ret = initBluetooth(handshake_id);
            break;
    }
    switch(ret)
    {
        case HANDSHAKE_SUCCEEDED:
            printf("Handshake success.\n");
            break;
        case HANDSHAKE_FAILED:
            printf("Handshake failed.\n");
            break;
        case HANDSHAKE_SKIPPED:
            printf("Handshake skipped.\n");
            break;
        case INITIALIZATION_FAILED:
            printf("Initialization failed.\n");
            break;
    }
}

SERCOM_STATUS SERCOM::initUSB(const char * handshake_id)
{
    Init_SERCOM_Default();
    return handshake(handshake_id);
}

SERCOM_STATUS SERCOM::initBluetooth(const char * handshake_id)
{
    channel.filestream = -1;
    channel.port = "/dev/tty.Bluetooth-Incoming-Port";
    channel.port_alt = "/dev/cu.Bluetooth-Incoming-Port";
    Init_SERCOM( &channel );
    return handshake(handshake_id);
}

SERCOM_STATUS SERCOM::init( char * port, char * port_alt,const  char * handshake_id )
{
    channel.filestream = -1;
    channel.port = port;
    channel.port_alt = port_alt;
    Init_SERCOM( &channel );
    return handshake(handshake_id);
}

SERCOM_STATUS SERCOM::handshake(const  char * id )
{
    return handshake(id, DEFAULT_HANDSHAKE_DELAY, DEFAULT_HANDSHAKE_ATTEMPTS);
}
SERCOM_STATUS SERCOM::handshake(const  char * id, int delay, int attempts )
{
    if( !channel.initialized ) return INITIALIZATION_FAILED;
    if( id != NULL ) return HANDSHAKE_SKIPPED;
    int counter = 0;
    while( counter < attempts )
    {
        if( Test_SERCOM(channel.filestream) <= 0 )
        {
            counter++;
            printf("SERCOM attempt #%d.\n", counter );
            usleep(delay);
        }
        else break;
    }
    if(counter == attempts) return HANDSHAKE_FAILED;
    else
    {
        Write_SERCOM_Bytes(channel.filestream, id, 4);
        return HANDSHAKE_SUCCEEDED;
    }
}

void SERCOM::write( std::string data )
{
    Write_SERCOM_Bytes( channel.filestream, data.c_str(), (int)data.length() );
}

std::string SERCOM::read( int l )
{
    char data[l];
    Read_SERCOM_Bytes( channel.filestream, data, l );
    return std::string(data);
}
