//
//  profilemanager.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef profilemanager_h
#define profilemanager_h

#include "application.h"

void InitProfile(void);
void LoadProfile( system_profile_t * );

typedef struct
{
    void (*Init)(void);
    void (*Load)( system_profile_t * );
} profile_functions;

static profile_functions ProfileFunctions =
{
    .Init = InitProfile,
    .Load = LoadProfile
};

#endif /* profilemanager_h */
