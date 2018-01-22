//
//  usb_serial.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "file_writer.hpp"

using namespace std;

FileWriter::FileWriter()
{
    file_name = "/Users/matthewfonken/Desktop/out.txt";
}

void * FileWriter::init( void *data )
{
    std::ofstream outfile;
    outfile.open(file_name, std::ofstream::out | std::ofstream::trunc);
    if (!outfile.is_open())
    {
        printf("Failed to open %s\n", file_name);
        while(1);
    }
    printf("Opened %s\n", file_name);
    return NULL;
}

void * FileWriter::trigger( void *data )
{
    std::ofstream outfile;
    outfile.open(file_name, std::ofstream::out | std::ofstream::trunc);
    
//    outfile.write(kin_packet,l);
    outfile.close();
    return NULL;
}
