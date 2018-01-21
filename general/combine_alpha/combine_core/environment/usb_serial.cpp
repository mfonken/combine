//
//  usb_serial.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "usb_serial.hpp"

using namespace std;

void * DATA_WR_THREAD( void *data )
{
    environment_t edat = *( ( environment_t * ) data );
    std::ofstream outfile;
    outfile.open(file_name, std::ofstream::out | std::ofstream::trunc);
    if (!outfile.is_open())
    {
        printf("Failed to open %s\n", file_name);
        while(1);
    }
    printf("Opened %s\n", file_name);
    int counter = 1;
    while(1)
    {
        outfile.open(file_name, std::ofstream::out | std::ofstream::trunc);
        char kin_packet[MAX_BUFFER];
        double r[3], p[3];
        r[0] = edat.kin.values.rotation[0];
        r[1] = edat.kin.values.rotation[1];
        r[2] = edat.kin.values.rotation[2];
        p[0] = edat.kin.filters.position[0].value * SCALE;
        p[1] = edat.kin.filters.position[1].value * SCALE;
        p[2] = edat.kin.filters.position[2].value * SCALE;
        
        int l = sprintf(kin_packet, "f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\r\n", r[0], r[1], r[2],  p[0], p[1], p[2] );
        
        outfile.write(kin_packet,l);
        outfile.close();
        
        if(++counter > OUT_FPS) counter = 1;
        usleep(OUT_UDL);
    }
}
