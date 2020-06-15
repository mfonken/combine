//
//  profilemanager.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef profilemanager_h
#define profilemanager_h

#include "systemtypes.h"

void InitProfile( system_profile_t * );

void FetchProfile(void);
void PerformProfile(void);
void UpdateProfile(void);
void StoreProfile(void);

typedef struct
{
    void (*Init)( system_profile_t * );
    void (*Fetch)(void);
    void (*Perform)(void);
    void (*Update)(void);
    void (*Store)(void);
} profile_functions;

static profile_functions ProfileFunctions =
{
    .Init = InitProfile,
    .Fetch = FetchProfile,
    .Perform = PerformProfile,
    .Update = UpdateProfile,
    .Store = StoreProfile
};

#endif /* profilemanager_h */
