//
//  profilemanager.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "profilemanager.h"
#include "systemmanager.h"

void InitProfile( system_profile_t * profile )
{
    SystemFunctions.Registers.TaskShelf( &profile->shelf );
    for( uint8_t i = 0; i < NUM_SYSTEM_STATES; i++ )
        SystemFunctions.Registers.StateProfile( i, profile->state_profiles[i] );
    SystemFunctions.Perform.Routine( &profile->routines[SYSTEM_ACTIVITY_STARTUP] );
}

void InitProfileEntry( system_profile_entry_t * entry )
{
    if( entry->header.state == SYSTEM_PROFILE_ENTRY_STATE_ENABLED)
    {
        switch( entry->header.type )
        {
            case SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            case SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULED:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            default:
                break;
        }
    }
}

void FetchProfile(void)
{
    
}
void PerformProfile(void)
{
    
}
void UpdateProfile(void)
{
    
}
void StoreProfile(void)
{
    
}
