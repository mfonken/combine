//
//  profilemanager.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef profilemanager_h
#define profilemanager_h

#include <stdio.h>

#include "systemmanager.h"

void InitProfile( system_profile_t * );
void InitProfileEntry( system_profile_entry_t * );

void FetchProfile( );
void PerformProfile( );
void UpdateProfile( );
void StoreProfile( );

typedef struct
{
    void (*Init)(void);
    void (*Fetch)(void);
    void (*Perform)(void);
    void (*Update)(void);
    void (*Store)(void);
} profile_functions;

static profile_functions ProfileFunctions =
{
    .Init = InitProfile,
    .InitEntry = InitProfileEntry,
    .Fetch = FetchProfile,
    .Perform = PerformProfile,
    .Update = UpdateProfile,
    .Store = StoreProfile
};

#endif /* profilemanager_h */
