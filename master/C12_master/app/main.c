#ifdef APP_MAIN
//
//  main.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

// /* Types */
// #include "systemtypes.h"

/* Functions */
#include "systembridge.h"

int main(int argc, const char * argv[])
{
    Bridge.Start();
    return 0;
}
#endif
