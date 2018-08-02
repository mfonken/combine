//
//  main.c
//  sensors
//
//  Created by Matthew Fonken on 7/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "batterymonitor.h"
#include "touchcontroller.h"
#include "motionsensor"

int main(int argc, const char * argv[]) {    
    printf("Battery monitor control register 0x%02x\n", BatteryMonitor.getControl());
    printf("Touch controller read 0x%02x\n", TouchController.read().index);
    
    
    
    return 0;
}
