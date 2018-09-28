//
//  main.c
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "rho_client.h"

int main(int argc, const char * argv[])
{
    RhoSystem.Functions.Perform.Init();
    RhoSystem.Functions.Perform.FrameCapture();
    RhoSystem.Functions.Perform.RhoProcess();
    return 0;
}
