//
//  profilemanager.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <profilemanager.h>

void InitProfile()
{
    system_profile_t init_profile = PROFILE_TEMPLATE;
    ProfileFunctions.Load( &init_profile );
    
    /* Initialize System using profile */
    SystemFunctions.Init( &Profile );
}

void LoadProfile( system_profile_t * p_loaded_profile )
{
    memcpy( &Profile, p_loaded_profile, sizeof(system_profile_t));
}
