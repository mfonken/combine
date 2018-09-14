//
//  systeminterface.c
//  C11_rtos
//
//  Created by Matthew Fonken on 9/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systeminterface.h"

void StartSystemInterfaceOS( )
{
}

void CreateSystemInterfaceOSTask( os_task_data_t * task_data )
{
    if( task_data->ID == SYSTEM_ACTION_ID_NONE ) return;
    printd("Creating OS Task: %s\n", task_id_strings[task_data->ID]);
    //    OSTaskCreate(&task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 &task_data->[0u],
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 task_data->,
    //                 &task_data->);
    
    task_data->p_task(task_data->p_arg);
}

void ResumeSystemInterfaceOSTask( os_task_data_t * task_data )
{
    printd("Resuming OS Task: %s\n", task_id_strings[task_data->ID]);
// Resume OS Task by task_data->OS_TCB & task_data-RTOS_ERR
//    OSTaskResume()
}

void SuspendSystemInterfaceOSTask( os_task_data_t * task_data )
{
    printd("Suspending OS Task: %s\n", task_id_strings[task_data->ID]);
// Suspend OS Task by task_data->OS_TCB & task_data-RTOS_ERR
//    OSTaskSuspend()
}

void DeleteSystemInterfaceOSTask( os_task_data_t * task_data )
{
    printd("Deleting OS Task: %s\n", task_id_strings[task_data->ID]);
//    OSTaskDelete()
}

void EnableSystemInterfaceHardwareInterrupt( os_task_data_t * task_data )
{
}
