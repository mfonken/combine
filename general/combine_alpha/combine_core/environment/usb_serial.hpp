//
//  usb_serial.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef usb_serial_hpp
#define usb_serial_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "environment.hpp"

#define SCALE 100

const char file_name[] = "/Users/matthewfonken/Desktop/out.txt";

void * DATA_WR_THREAD( void *data );

#endif /* usb_serial_hpp */
