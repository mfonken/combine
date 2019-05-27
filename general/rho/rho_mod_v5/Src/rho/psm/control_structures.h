//
//  control_structures.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/11/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef control_structures_h
#define control_structures_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include "kumaraswamy.h"
  
/* FSM state tree with fsm base */
typedef struct
{
    double map[NUM_STATES][NUM_STATES];
    uint8_t length;
} fsm_map_t;
    
/* System self-diagnostic state control type */
typedef struct
{
    state_t         state;
    state_t         prev;
    state_t         next;
    uint8_t         selection_index;
    stability_t     stability;
    fsm_map_t       probabilities;
} fsm_system_t;
   
#ifdef __cplusplus
}
#endif

#endif /* control_structures_h */
