/***********************************************************************************************//**
 * \file   app_interrupts.h
 * \brief  App Interrupt Controller
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef SYSTEM_APP_INTERRUPTS_H_
#define SYSTEM_APP_INTERRUPTS_H_

#define TIMER_PRESCALE		timerPrescale1024
#define TIMER_CLK_FRQ		SystemHFXOClockGet()
#define TICK_TO_MS			TIMER_CLK_FRQ / ( ( 1 << (TIMER_PRESCALE + 4) ) * 1000 )

#include "app_interrupts.h"

/* Libraries containing Gecko configuration values */
#include "em_timer.h"
#include "em_usart.h"

/* System utilities */
#include "../system/utilities/usart_sp.h"


#define NUM_AVAILABLE_IRQ FPUEH_IRQn + 1

typedef void (callback_f)(void);

typedef struct
{
	uint8_t accel:1;
	uint8_t  gyro:1;
	uint8_t   mag:1;
	uint8_t   cam:1;
} sync_t;

void sensorSyncSet( sync_t * );

/* Interrupt Registers */
void registerCallback( callback_f * handler, IRQn_Type callback_type );
void registerTimer( callback_f * callback, TIMER_TypeDef * timer, uint32_t period );
void enableTimer(   TIMER_TypeDef * timer );
void disableTimer(  TIMER_TypeDef * timer );
void resetTimer(  	TIMER_TypeDef * timer );
void enableUARTInterrupt( 	void );
void disableUARTInterrupt( 	void );
void enableGPIOInterrupt(	void );
void disableGPIOInterrupt( 	void );

#endif /* SYSTEM_APP_INTERRUPTS_H_ */
