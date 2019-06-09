#include "master.h"

/***************************************************************************************/
/*                                  Core Routines                                      */
/***************************************************************************************/
static void InitializePlatform( void )
{
  PlatformFunctions.Init( &Platform, HOST_COMMUNICATION_PROTOCOL, (generic_handle_t)Master.IOs.HOST_DEFAULT_CHANNEL );
}

static void ConnectToHost( void )
{
  while( PlatformFunctions.Host.Command( PING_HOST, NEEDED ) != SUCCESS )
  {
    PlatformFunctions.Wait( HOST_COMMAND_WAIT_TIME );
  }
}

static void ConfigureApplication( void )
{
#ifdef __RHO__
  RhoSystem.Functions.Perform.ConnectToInterface( &Platform );
  RhoSystem.Functions.Perform.Configure( &Platform );
#endif
#ifdef __OV9712__
  OV9712_Functions.Init( &OV9712, Master.IOs.CAMERA_COMMUNICATION_CHANNEL, NULL );
#endif
}

static void ExitInitialization( void )
{
  if( SystemFunctions.State.IsIn( &System, READY ) )
  {
    MasterFunctions.Run();
  }
#ifdef __RHO__
  RhoSystem.Functions.Perform.Activate();
#endif
}

static inline void ApplicationCore( void )
{
#ifdef __RHO__
    RhoSystem.Functions.Perform.CoreProcess();
#else
#warning "No application core."
#endif
}

static void ErrorStateHandler( void )
{
    ///TODO: Implement handler method for unexpected entry of error state
}

/***************************************************************************************/
/*                                Master Initialize                                    */
/***************************************************************************************/
void Master_Connect( I2C_Handle_t * i2c, TIMER_Handle_t * timer, USART_Handle_t * usart )
{
  Master.IOs.I2C_Primary = i2c;
  Master.IOs.USART_Primary = usart;
  Master.Utilities.Timer_Primary = timer;

  MasterFunctions.Init();
}

void Master_Init( void )
{
  /* Initialize state manager */
  SystemFunctions.Init( &System, &global_states_list );

  /* Initialize core platform */
  SystemFunctions.State.Enter( &System, INITIALIZING );

  /* Connect to host, this is critical for finishing initialization hence endless loop */
  SystemFunctions.State.Enter( &System, CONNECTING_TO_HOST );

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
