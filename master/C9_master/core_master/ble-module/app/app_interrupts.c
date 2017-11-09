/***********************************************************************************************//**
 * \file   app_interrupts.c
 * \brief  App Interrupt Controller
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "app_interrupts.h"

/* Libraries containing Gecko configuration values */
#include "em_timer.h"
#include "em_usart.h"
#include "em_gpio.h"

/* System utilities */
#include "usart_sp.h"

/* Peripheral/Sensor headers */
#include "cam_controller.h"
#include "PCA9534A.h"
#include "LSM9DS1.h"
#include "CPT112S.h"

sync_t sync;
callback_f (*callbacks[NUM_AVAILABLE_IRQ]);

void sensorSyncSet( sync_t * s )
{
    sync = *s;
}

void USART0_RX_IRQHandler( void )
{
    USART_IntClear(CAM_UART, USART_IF_RXDATAV);
    callbacks[USART0_RX_IRQn]();
}

void TIMER0_IRQHandler(void)
{
    TIMER_IntClear( TIMER0, TIMER_IF_OF );
    callbacks[TIMER0_IRQn]();
}

void TIMER1_IRQHandler(void)
{
    TIMER_IntClear( TIMER1, TIMER_IF_OF );
    callbacks[TIMER1_IRQn]();
}

void registerCallback( callback_f * callback, IRQn_Type irqn )
{
    callbacks[irqn] = callback;
}


/* Interrupt Registers */
void registerTimer( callback_f * callback, TIMER_TypeDef * timer, uint32_t period )
{
	Print_Line( "Starting timer." );

	/* Select timer parameters */
	TIMER_Init_TypeDef timerInit =
	{
		.enable     = true,
		.debugRun   = true,
		.prescale   = TIMER_PRESCALE,
		.clkSel     = timerClkSelHFPerClk,
		.fallAction = timerInputActionNone,
		.riseAction = timerInputActionNone,
		.mode       = timerModeUp,
		.dmaClrAct  = false,
		.quadModeX4 = false,
		.oneShot    = false,
		.sync       = false,
	};

	/* Configure TIMER */
	TIMER_Init( timer, &timerInit );

	/* Set TIMER Top value */
	uint32_t p = TICK_TO_MS * period;
	TIMER_TopSet( timer, p );

	/* Enable timer interrupt vector in NVIC */
	IRQn_Type iqrn;
    if( timer == TIMER0)    iqrn = TIMER0_IRQn;
	else                    iqrn = TIMER1_IRQn;
	NVIC_EnableIRQ( iqrn );
	registerCallback( callback, iqrn );
}

void enableTimer( TIMER_TypeDef * timer )
{
	TIMER_IntEnable( timer, TIMER_IF_OF );
}

void disableTimer(  TIMER_TypeDef * timer )
{
	TIMER_IntDisable( timer, TIMER_IF_OF );
}

void resetTimer( TIMER_TypeDef * timer )
{
	TIMER_Reset( timer );
}

void enableUARTInterrupt( void )
{
	/* Prepare UART Rx interrupts */
	USART_IntClear(CAM_UART, _USART_IF_MASK);
	USART_IntEnable(CAM_UART, USART_IF_RXDATAV);
	NVIC_ClearPendingIRQ(USART0_RX_IRQn);
	NVIC_EnableIRQ(USART0_RX_IRQn);
}

void disableUARTInterrupt( void )
{
	USART_IntDisable(CAM_UART, USART_IF_RXDATAV);
}

void enableGPIOInterrupt( void )
{
	/* Enable interrupt in core for odd gpio interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Configure PA0 interrupt on rising edge */
	GPIO_IntConfig(gpioPortA, 0, true, false, true);
}

void disableGPIOInterrupt( void )
{
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
}
