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
static void InitializePlatform( void )
{
//    dmap_t x[CAPTURE_WIDTH] = { 0 }, y[CAPTURE_HEIGHT] = { 0 };
//    SpoofDensityMap( x, CAPTURE_WIDTH );
//    SpoofDensityMap( y, CAPTURE_HEIGHT );
//    DrawDensityMap( x, CAPTURE_WIDTH );
//    DrawDensityMap( y, CAPTURE_HEIGHT );
//    PrintDensityMaps( x, CAPTURE_WIDTH, y, CAPTURE_HEIGHT );

    PlatformFunctions.Init( &Platform, HOST_COMMUNICATION_PROTOCOL, (generic_handle_t)Master.IOs.HOST_DEFAULT_CHANNEL );
}

/* CONNECTING_TO_HOST State Routine */
static void ConnectToHost( void )
{
#ifdef PERFORM_HOST_PING_FOR_INIT
  while( PlatformFunctions.Host.Command( PING_HOST, NEEDED ) != OK )
  {
    PlatformFunctions.Wait( HOST_COMMAND_WAIT_TIME );
  }
#endif
}

/* CONFIGURING State Routine */
static void ConfigureApplication( void )
{
#ifdef __OV9712__
  OV9712_Functions.Init( &OV9712, Master.IOs.CAMERA_COMMUNICATION_CHANNEL, &Default_OV9712_Pins );
#endif
#ifdef __RHO__
  RhoSystem.Functions.Perform.ConnectToInterface( &PlatformFunctions, &Platform.CameraFlags );
  RhoSystem.Functions.Perform.Initialize( CAMERA_PORT, UART_TX_PORT );
#endif
}

/* READY State Routine */
static void ExitInitialization( void )
{
#ifdef __RHO__
  RhoSystem.Functions.Perform.Activate();
#endif
  if( SystemFunctions.State.IsIn( &System, READY ) )
    MasterFunctions.Run();
  else
    PlatformFunctions.Wait(250);
}

/* ACTIVE State Routine */
static inline void ApplicationCore( void )
{
#ifdef __RHO__
    RhoSystem.Functions.Perform.CoreProcess();
#else
#warning "No application core."
#endif
}

/* SYS_ERROR State Routine */
static void SystemError( void )
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
  Master.IOs.I2C_Primary = i2c;
  Master.Utilities.Timer_Primary = timer;
  Master.IOs.UART_Primary = usart;

#warning "TODO: Figure out better capure DMA initializer"
  STM_InitDMA( (uint32_t)&CAMERA_PORT, (uint32_t)RhoSystem.Variables.Buffers.Capture, (uint16_t)CAPTURE_BUFFER_SIZE, true );
HAL_GPIO_WritePin(GPIOB, LED_Pin|DEBUG_Pin, GPIO_PIN_SET);
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
