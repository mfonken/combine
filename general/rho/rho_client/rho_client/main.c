//
//  main.c
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "stm32_interface.h"
#include "rho_client.h"

int main(int argc, const char * argv[])
{
    RhoSystem.Functions.Perform.Init(CAMERA_PORT, UART_TX_PORT);
    

    RhoSystem.Functions.Perform.RhoProcess();
    return 0;
}
