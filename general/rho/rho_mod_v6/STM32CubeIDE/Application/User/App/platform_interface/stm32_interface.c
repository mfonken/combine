/************************************************************************
 *  File: stm32_interface.c
 *  Group: Rho Hardware Interface
 ***********************************************************************/

/************************************************************************
 *                             Includes                                 *
 ***********************************************************************/
#include <platform_interface/stm32_interface.h>

/************************************************************************
 *                             Master Instance                          *
 ***********************************************************************/
master_t Master;

/************************************************************************
 *                           Interrupt Handlers                         *
 ***********************************************************************/
//static int hrefs = 0;
inline void STM_InterruptHandler( uint16_t GPIO_Pin )
{
    /* Applicaiton Specific */
#ifdef __RHO__
  if(!CameraFlags.IRQ) return;
  switch(GPIO_Pin)
  {
    case VSYNC_Pin:
        CameraFlags.Frame = !(flag_t)( VSYNC_GPIO_Port->IDR & VSYNC_Pin );
//        if(!Platform.CameraFlags.Frame)
//          Platform.CameraFlags.IRQ = false;
//        else
//          hrefs = 0;
        break;
    case HREF_Pin:
        CameraFlags.Row = (flag_t)( HREF_GPIO_Port->IDR & HREF_Pin );
//        if(Platform.CameraFlags.Frame && Platform.CameraFlags.Row)
//          HAL_GPIO_WritePin(GPIOB, LED_Pin|DEBUG_Pin, GPIO_PIN_SET);
        /* Row capture is callback connected on HREF=HIGH */
        if( !CameraFlags.Row
         && CameraFlags.Capture.Flag
         && CameraFlags.Capture.Callback != NULL )
            CameraFlags.Capture.Callback();
        break;
    default:
        return;
  }
  if(!CameraFlags.Row || CameraFlags.Frame )
  {
//    LOG(ALWAYS, "0x%08x", Master.Utilities.Timer_Primary->hdma[RHO_TIM_DMA_ID]->Instance->CNDTR );
    HAL_DMA_Abort(Master.Utilities.Timer_Primary->hdma[RHO_TIM_DMA_ID]);
    STM_InitDMA( (uint32_t)&CAMERA_PORT, _dma_destination, _dma_size, true );
//    LOG(ALWAYS, "0x%08x", Master.Utilities.Timer_Primary->hdma[RHO_TIM_DMA_ID]->Instance->CNDTR );
//    LOG(ALWAYS, ENDL);
  }
#endif
}
void STM_InterruptEnable( void )
{
//  STM_ResumeDMA(RHO_TIM_IT_CC, RHO_TIM_CHANNEL);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}
void STM_InterruptDisable( void )
{
//  STM_PauseDMA(RHO_TIM_IT_CC, RHO_TIM_CHANNEL);
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);
  HAL_NVIC_DisableIRQ(EXTI4_IRQn);
}

/************************************************************************
 *                              DMA Handlers                            *
 ***********************************************************************/
inline void STM_PauseDMA( uint32_t interrupt, uint32_t channel )
{
  __HAL_TIM_DISABLE_IT(Master.Utilities.Timer_Primary, interrupt);
  TIM_CCxChannelCmd(Master.Utilities.Timer_Primary->Instance, channel, TIM_CCx_DISABLE);
}
inline void STM_ResumeDMA( uint32_t interrupt, uint32_t channel )
{
  __HAL_TIM_ENABLE_IT(Master.Utilities.Timer_Primary, interrupt ); //RHO_TIM_IT_CC);
  TIM_CCxChannelCmd(Master.Utilities.Timer_Primary->Instance, channel/*RHO_TIM_CHANNEL*/, TIM_CCx_ENABLE);
}
inline void STM_ResetDMA( void * dma_destination, uint8_t dma_id )
{
    if(dma_destination != NULL)
        Master.Utilities.Timer_Primary->hdma[dma_id/*RHO_TIM_DMA_ID*/]->Instance->CMAR = dma_destination;
}
void STM_InitDMA( uint32_t src, uint32_t dst, uint16_t size, bool init_state )
{
	return;//
//  if(HAL_DMA_Start_IT(Master.Utilities.Timer_Primary->hdma[RHO_TIM_DMA_ID], src, dst, size) != HAL_OK)
//    Error_Handler();
//  __HAL_TIM_ENABLE_DMA(Master.Utilities.Timer_Primary, RHO_TIM_DMA_CC);
//  if(init_state) STM_ResumeDMA();
//  _dma_destination = dst;
//  _dma_size = size;
}
uint32_t STM_GetDMAFillAddress( void )
{
  return 0;//_dma_destination + ( (int32_t)_dma_size - (int32_t)Master.Utilities.Timer_Primary->hdma[RHO_TIM_DMA_ID]->Instance->CNDTR );
}

/************************************************************************
 *                             UART Handlers                            *
 ***********************************************************************/
inline uint8_t STM_UartTxDMA( UART_Handle_t * huart, uint8_t * buffer, uint16_t length )
{
  return 0;//HAL_UART_Transmit( Master.IOs.UART_Primary, buffer, length, UART_TIMEOUT ); //HAL_UART_Transmit_DMA
}

inline uint16_t STM_UartRxDMA( UART_Handle_t * huart, uint8_t * buffer )
{
  ///TODO: Actually implement
  return 1;
}
//inline bool STM_UartCompleted( UART_Handle_t * huart )
//{
//#ifdef __RHO__
//  Platform.CameraFlags.UARTBusy = 0;
//#endif
//  return false;
//}

/************************************************************************
 *                              I2C Handlers                            *
 ***********************************************************************/
inline void STM_I2CMasterTx( I2C_Handle_t * hi2c, uint16_t addr, uint8_t * buffer, uint16_t length, uint32_t timeout )
{
  HAL_I2C_Master_Transmit( hi2c, addr, buffer, length, timeout);
}

/************************************************************************
 *                             GPIO Handlers                            *
 ***********************************************************************/
inline void STM_SetPortMode( GPIO_TypeDef * port, uint16_t pin, uint8_t mode )
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = mode;
  HAL_GPIO_Init( (GPIO_TypeDef *)port, &GPIO_InitStruct );
}
inline uint8_t STM_ReadPort( GPIO_TypeDef * port )
{
    return port->IDR;
}
inline void STM_WritePin( GPIO_TypeDef * port, uint16_t pin, uint8_t state )
{
  HAL_GPIO_WritePin( port, pin, (GPIO_PinState)state);
}

/************************************************************************
 *                             Time Handlers                            *
 ***********************************************************************/
inline uint32_t STM_Timestamp(void)
{
  return HAL_GetTick();
}
inline void STM_Wait( uint32_t nTime )
{
  HAL_Delay( nTime );
}
inline uint32_t STM_SysClockFreq(void)
{
  return HAL_RCC_GetSysClockFreq();
}

/************************************************************************
 *                            System Handlers                           *
 ***********************************************************************/
void STM_Reset( void )
{
    NVIC_SystemReset();
}
