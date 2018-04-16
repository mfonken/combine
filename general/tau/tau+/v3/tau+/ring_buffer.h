//
//  ring_buffer.h
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ring_buffer_h
#define ring_buffer_h

#include <stdio.h>

#ifndef B_NULL
#define B_NULL 0xff
#endif

/* Size should be a power of 2 */
#define BUFF_SIZE 32
#define BUFF_SIZE_MASK ( BUFF_SIZE - 1 )

typedef struct
{
    uint16_t buffer[BUFF_SIZE];
    uint8_t  index;
} ring_buffer_t;

uint8_t     ringBufferAdd(      ring_buffer_t * b, uint16_t v );
uint16_t    ringBufferRead(     ring_buffer_t * b, uint8_t i );
void        ringBufferReset(    ring_buffer_t * b );

#endif /* ring_buffer_h */
    
#ifdef __cplusplus
}
#endif
