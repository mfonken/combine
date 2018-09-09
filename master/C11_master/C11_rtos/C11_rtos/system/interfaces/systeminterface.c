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
    //    OSTaskCreate(&BluetoothTaskTCB,
    //                 "Bluetooth Task",
    //                 BluetoothTask,
    //                 0u,
    //                 stack_priority,
    //                 &BluetoothTaskStk[0u],
    //                 BLUETOOTH_STACK_SIZE / 10u,
    //                 BLUETOOTH_STACK_SIZE,
    //                 0u,
    //                 0u,
    //                 0u,
    //                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
    //                 &os_err);
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
