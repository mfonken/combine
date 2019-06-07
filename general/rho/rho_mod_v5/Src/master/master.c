#include "master.h"

/***************************************************************************************/
/*                                  Core Routines                                      */
/***************************************************************************************/
static void InitializePlatform()
{
  PlatformFunctions.Init( &Platform, USART_PROTOCOL_TYPE, (generic_handle_t)usart );
}

static void ConnectToHost()
{
  while( PlatformFunctions.Host.Command( PING_HOST, NEEDED ) != SUCCESS )
  {
    PlatformFunctions.Wait( HOST_COMMAND_WAIT_TIME );
  }
}

static void ConfigureApplication()
{
#ifdef __RHO__
  RhoSystem.Functions.Perform.ConnectToInterface( &Platform );
#endif
#ifdef __OV9712__
  OV9712_Functions.Init( &OV9712, i2c, NULL );
#endif
}

static void ExitInitialization()
{
  if( SystemFunctions.State.Get(System) == READY )
  {
    Master.Run();
  }
#ifdef __RHO__
  RhoSystem.Functions.Perform.Activate();
#endif
}

static inline void ApplicationCore()
{
#ifdef __RHO__
    RhoSystem.Functions.Perform.CoreProcess();
#endif
}

/***************************************************************************************/
/*                                Master Initialize                                    */
/***************************************************************************************/
void Master_Init( I2C_Handle_t * i2c, TIMER_Handle_t * timer, USART_Handle_t * usart )
{
  /* Initialize state manager */
  SystemFunctions.Init( &System, global_states_list );

  /* Initialize core platform */
  SystemFunctions.State.Enter(System, INITIALIZING );

  /* Connect to host, this is critical for finishing initialization hence endless loop */
  SystemFunctions.State.Enter(System, CONNECTING_TO_HOST );

  /* Configure application items */
  SystemFunctions.State.Enter(System, CONFIGURING );

  /* Exit initialization, enter run */
  SystemFunctions.State.Enter(System, READY );
}

/***************************************************************************************/
/*                                   Master Run                                        */
/***************************************************************************************/
void Master_Run( void )
{
  System.State.Set(ACTIVE);

  while(1)
  {
    System.State.Perform();
  }
}
