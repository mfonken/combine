//
//  bluetooth_serial.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef bluetooth_serial_hpp
#define bluetooth_serial_hpp

#include <stdio.h>

const char bluetooth_port[] = "/dev/tty.Bluetooth-Incoming-Port";
const char bluetooth_port_alt[] = "/dev/cu.Bluetooth-Incoming-Port";

void * BT_THREAD( void *data );

#endif /* bluetooth_serial_hpp */
