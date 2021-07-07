/************************************************************************
 *  File: master.c
 *  Group: Rho Core
 ***********************************************************************/

/***************************************************************************************/
/*                                    Includes                                         */
/***************************************************************************************/
#include "master.h"
#include "printers.h"

/***************************************************************************************/
/*                                  Core Routines                                      */
/***************************************************************************************/
/* INITIALIZING State Routine */
void InitializePlatform( void )
{
    PlatformFunctions.Init( &Platform, HOST_COMMUNICATION_PROTOCOL, (generic_handle_t)Master.IOs.HOST_DEFAULT_CHANNEL );
}

/* CONFIGURING State Routine */
void ConfigureApplication( void )
{
#ifdef __YOUR_APP__
  /// Initialize your application
#endif
}

/* READY State Routine */
void ExitInitialization( void )
{
#ifdef __YOUR_APP__
  /// Activate your application
#endif
  if( SystemFunctions.State.IsIn( &System, READY ) )
    MasterFunctions.Run();
  else
    PlatformFunctions.Wait(250);
}

/* ACTIVE State Routine */
static inline void ApplicationCore( void )
{
#ifdef __YOUR_APP__
    /// Run your application core
#else
#warning "No application core."
#endif
}

/* SYS_ERROR State Routine */
void SystemError( void )
{
    LOG( ALWAYS, "System error! Resetting in" );
    for( byte_t i = 3 ; i > 0; i-- )
    {
        LOG_BARE( ALWAYS, " %d", i );
        PlatformFunctions.Wait(1000);
    }
    PlatformFunctions.Reset();
}

/***************************************************************************************/
/*                                Master Initialize                                    */
/***************************************************************************************/
void Master_Connect( I2C_Handle_t * i2c, TIMER_Handle_t * timer, UART_Handle_t * usart )
{
  MasterFunctions.Init();
}

void Master_Init( void )
{
  /* Initialize state manager */
  SystemFunctions.Init( &System, &global_states_list );

  /* Initialize core platform */
  SystemFunctions.State.Enter( &System, INITIALIZING );

  /* Configure application items */
  SystemFunctions.State.Enter( &System, CONFIGURING );

  /* Exit initialization, enter run */
  SystemFunctions.State.Enter( &System, READY );
}

/***************************************************************************************/
/*                                   Master Run                                        */
/***************************************************************************************/
void Master_Run( void )
{
  SystemFunctions.State.Set( &System, ACTIVE );

  while(1)
  {
    SystemFunctions.State.Perform( &System );
  }
}
