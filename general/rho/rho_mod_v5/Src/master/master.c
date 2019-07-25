/***************************************************************************************/
/*                                    Includes                                         */
/***************************************************************************************/
#include "master.h"
#include "printers.h"

/***************************************************************************************/
/*                                  Core Routines                                      */
/***************************************************************************************/
static void InitializePlatform( void )
{
  PlatformFunctions.Init( &Platform, HOST_COMMUNICATION_PROTOCOL, (generic_handle_t)Master.IOs.HOST_DEFAULT_CHANNEL );
}

static void ConnectToHost( void )
{
  while( PlatformFunctions.Host.Command( PING_HOST, NEEDED ) != OK )
  {
    PlatformFunctions.Wait( HOST_COMMAND_WAIT_TIME );
  }
}

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

static void ExitInitialization( void )
{
#ifdef __RHO__
  RhoSystem.Functions.Perform.Activate();
#endif
  if( SystemFunctions.State.IsIn( &System, READY ) )
  {
    MasterFunctions.Run();
  }
  else
  {
    PlatformFunctions.Wait(250);
  }
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

uint8_t     HAL_GPIO_ReadPort(GPIO_TypeDef* GPIOx)
{
  return GPIOx->IDR;
}

/***************************************************************************************/
/*                                Master Initialize                                    */
/***************************************************************************************/
void Master_Connect( I2C_Handle_t * i2c, TIMER_Handle_t * timer, UART_Handle_t * usart )
{
  Master.IOs.I2C_Primary = i2c;
  Master.IOs.UART_Primary = usart;
  Master.Utilities.Timer_Primary = timer;
  
#warning "TODO: Figure out better capure DMA initializer"
//  if(HAL_DMA_Start_IT(timer->hdma[RHO_TIM_DMA_ID], (uint32_t)&CAMERA_PORT, (uint32_t)RhoSystem.Variables.Buffers.Capture, CAPTURE_BUFFER_SIZE) != HAL_OK)
//    Error_Handler();
  
  while(1)
  {
    sprintf(str_buf, "\r\n:0x%02x", GPIOB->IDR & 0x00ff );
    print(str_buf);
//    sprintf(str_buf, "\r\nCamera Value: 0x%8x", CAMERA_PORT & 0x00ff);
//    print(str_buf);
//    sprintf(str_buf, "\r\nCapture Value: 0x%8x", *(uint32_t *)RhoSystem.Variables.Buffers.Capture & 0x00ff);
//    print(str_buf);
    STM_Wait(1000);
  }
  MasterFunctions.Init();
}

void Master_Init( void )
{ 
  dmap_t x[CAPTURE_WIDTH] = { 0 }, y[CAPTURE_HEIGHT] = { 0 };
  SpoofDensityMap( x, CAPTURE_WIDTH );
  SpoofDensityMap( y, CAPTURE_HEIGHT );
  DrawDensityMap( x, CAPTURE_WIDTH );
//  DrawDensityMap( y, CAPTURE_HEIGHT );
//  uint16_t del = UNIVERSAL_DELIMITER;
//  print((char *)&del);
//  PrintBuffer( x, CAPTURE_WIDTH );
//  PrintBuffer( y, CAPTURE_HEIGHT );
//  print((char *)&del);
  
  while(1);
  /* Initialize state manager */
  SystemFunctions.Init( &System, &global_states_list );

  /* Initialize core platform */
  SystemFunctions.State.Enter( &System, INITIALIZING );

  /* Connect to host, this is critical for finishing initialization hence endless loop */
  //SystemFunctions.State.Enter( &System, CONNECTING_TO_HOST );

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
