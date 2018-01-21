//
//  bluetooth_serial.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "bluetooth_serial.hpp"
#include "environment.hpp"

void * BT_THREAD( void *data )
{
    int bluetooth_filestream = -1;
    environment_t edat = *( ( environment_t * ) data );
    SERCOM_Channel chan = {-1, bluetooth_port, bluetooth_port_alt, B115200, CS8, 0};
    Init_SERCOM(&chan);
    
    printf("Waiting for start\n");
    int received = -1;
    while( received <= 0 )
    {
        received = Test_SERCOM(bluetooth_filestream);
        printf("Received %d bytes\n", received);
        usleep(1000000);
    }
    
    printf("Sending handshake\n");
    char test[] = "ab\r\n";
    Write_SERCOM_Bytes(bluetooth_filestream, test, 4);
    
    printf("Bluetooth thread started\n");
    int counter = 1;
    while(1)
    {
        char kin_packet[MAX_BUFFER];
        vec3_t n = edat.kin.r;
        int l = sprintf(kin_packet, "n,%.4f,%.4f,%.4f\r\n", n.i, n.j, n.k);
        
        Write_SERCOM_Bytes(bluetooth_filestream, kin_packet, l);
        
        if(++counter > OUT_FPS) counter = 1;
        usleep(OUT_UDL);
    }
}
