//
//  systembridge.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembridge_h
#define systembridge_h

#include "application.h"
//#include "systemhandlers.h"
#include "C12_profile.h"

void Start(void)
{
    OSFunctions.Init();
    App.Init();
    
    OSFunctions.Start();
    App.Start();
}

typedef struct
{
    void (*Start)(void);
} bridge_functions;

static bridge_functions Bridge =
{
    .Start = Start
};

#endif /* systembridge_h */



