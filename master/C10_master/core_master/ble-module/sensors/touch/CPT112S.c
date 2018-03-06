/*
 * CPT112S.c
 *
 *  Created on: Jan 10, 2017
 *      Author: Matthew Fonken
 */

/* Own header */
#include "CPT112S.h"

void Touch_Init( void )
{

}

void Touch_Read( uint8_t * i2c_read_data )
{
    I2C_Read( TOUCH_CTR_ADDR, i2c_read_data, 3 );
}
