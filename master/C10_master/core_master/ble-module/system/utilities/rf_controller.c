/*
 * rf_controller.c
 *
 *  Created on: Jan 12, 2017
 *      Author: Matthew Fonken
 */

#include "rf_controller.h"

beacon_device_t beacon_status;

void RF_Session_Init( uint8_t i, uint32_t d )
{
	/* Enable timer interrupt vector in NVIC */
	NVIC_EnableIRQ( CRYOTIMER_IRQn );

	/* Enable overflow interrupt */
	CRYOTIMER_IntEnable( TIMER_IF_OF );

	beacon_status.session_duration = d;
	beacon_status.intensity = i;
	RF_Tx( &beacon_status );
	beacon_status.session_active = true;
}

void RF_Session_End( void )
{
	CRYOTIMER_IntDisable( TIMER_IF_OF );
	beacon_status.session_active = false;
}

void RF_Kick( void )
{
	RF_Tx( &beacon_status );
}

bool RF_Step( bool i )
{
    bool full_step = true;
    if( i )
    {
        /* If intensity is too high, cap at max */
        if( beacon_status.intensity > INTENSITY_TOP_STEP )
        {
            beacon_status.intensity = INTENSITY_MAX;
            
        }
        else
        {
            beacon_status.intensity += INTENSITY_STEP;
        }
	}
    else
    {
        if( beacon_status.intensity > INTENSITY_STEP )
        {
            beacon_status.intensity -= INTENSITY_STEP;
        }
        /* If intensity is too low, don't change */
        else
        {
            full_step = false;
        }
    }
    RF_Kick();
	return full_step;
}

void RF_Tx( beacon_device_t * b)
{
	USART_Tx( BEACON_USART, b->session_duration );
	USART_Tx( BEACON_USART, b->intensity );
}
