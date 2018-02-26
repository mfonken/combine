/*
 * data_types.h
 *
 *  Created on: Jan 11, 2017
 *      Author: Matthew Fonken
 */

#ifndef SYSTEM_UTILITIES_DATA_TYPES_H_
#define SYSTEM_UTILITIES_DATA_TYPES_H_

#include <stdint.h>

#ifndef B_NULL
#define B_NULL 0xff
#endif

/* Size should be a power of 2 */
#define BUFF_SIZE 32
#define BUFF_SIZE_MASK ( BUFF_SIZE - 1 )

typedef struct
{
	uint8_t buffer[BUFF_SIZE];
    uint8_t index;
} buffer_t;

uint8_t bufferAdd(   buffer_t * b, uint8_t v );
uint8_t bufferRead(  buffer_t * b, uint8_t i );
void    bufferReset( buffer_t * b );

#endif /* SYSTEM_UTILITIES_DATA_TYPES_H_ */
