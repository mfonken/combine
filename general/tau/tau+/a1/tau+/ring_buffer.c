//
//  ring_buffer.c
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "ring_buffer.h"

uint8_t ringBufferAdd( ring_buffer_t * b, uint16_t v )
{
    b->buffer[b->index++] = v;
    //b->index &= BUFF_SIZE_MASK;
    return b->index;
}

uint16_t ringBufferRead( ring_buffer_t * b, uint8_t i )
{
    return b->buffer[i & BUFF_SIZE_MASK];
}

void ringBufferReset( ring_buffer_t * b )
{
    for(int i = 0; i < BUFF_SIZE; i++ )
    {
        b->buffer[i] = B_NULL;
    }
}
