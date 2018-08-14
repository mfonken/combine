//
//  taumanager.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef taumanager_h
#define taumanager_h

#include <stdio.h>

typedef struct
{
    
} tau_packet_t;

void TauManagerInit(void);

typedef struct
{
    void (*Init)(void);
} tau_functions;

static tau_functions TauFunctions =
{
    .Init = TauManagerInit
};

#endif /* taumanager_h */
