//
//  profilemanager.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "profilemanager.h"

void InitProfile( sys_profile_t * profile )
{
    PerformSystemRoutine( &profile->routines->startup );
    
    for( uint8_t i = 0; i < profile->num_entries; i++ )
    {
        
    }
}

void InitProfileEntry( sys_profile_entry_t * entry )
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
            case SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULE:
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
