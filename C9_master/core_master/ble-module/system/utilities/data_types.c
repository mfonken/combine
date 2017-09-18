/*
 * data_types.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Matthew Fonken
 */

/* Own header */
#include "data_types.h"

/* Note: These functions are designed for circular buffering.
 *		 Each has only one extra instruction than linear. It should be fine.
 */

uint8_t bufferAdd( buffer_t * b, uint8_t v )
{
    b->buffer[b->index++] = v;
    //b->index &= BUFF_SIZE_MASK;
    return b->index;
}

uint8_t bufferRead( buffer_t * b, uint8_t i )
{
    return b->buffer[i];// & BUFF_SIZE_MASK];
}

void bufferReset( buffer_t * b )
{
    for(int i = 0; i < BUFF_SIZE; i++ )
    {
        b->buffer[i] = B_NULL;
    }
}
