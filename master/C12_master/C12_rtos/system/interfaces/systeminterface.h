//
//  systeminterface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 9/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systeminterface_h
#define systeminterface_h

#include "profilemanager.h"
#include "sysiocontroller.h"

void CreateSystemInterfaceOSTask( os_task_data_t * );
void ResumeSystemInterfaceOSTask( os_task_data_t * );
void SuspendSystemInterfaceOSTask( os_task_data_t * );
void DeleteSystemInterfaceOSTask( os_task_data_t * );

typedef struct
{
    void (*Create)( os_task_data_t * );
    void (*Resume)( os_task_data_t * );
    void (*Suspend)( os_task_data_t * );
    void (*Delete)( os_task_data_t * );
} system_os_interface_functions;

typedef struct
{
    system_os_interface_functions OS;
} system_interface_functions;

static system_interface_functions InterfaceFunctions =
{
    .OS.Create = CreateSystemInterfaceOSTask,
    .OS.Resume = ResumeSystemInterfaceOSTask,
    .OS.Suspend = SuspendSystemInterfaceOSTask,
    .OS.Delete = DeleteSystemInterfaceOSTask
};


#endif /* systeminterface_h */
