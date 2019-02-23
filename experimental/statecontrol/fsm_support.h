//
//  fsm_support.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/16/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef fsm_support_h
#define fsm_support_h

#include "control_structures.h"

#ifdef __cplusplus
extern "C" {
#endif    
    
#ifndef LOG_LEVEL
#define LOG_LEVEL
    enum LogLevel
    {
        TEST = 0,
        DEBUG_0,
        DEBUG_1,
        DEBUG_2,
        ALWAYS
    };
#endif
    
#ifndef TEST_LOG_LEVEL
#define TEST_LOG_LEVEL ALWAYS
#endif
    
#ifndef LOG
#define LOG(L,...) if(L >= TEST_LOG_LEVEL) \
{  for(uint8_t i=L;i<ALWAYS;i++) printf("\t"); printf(__VA_ARGS__); }
#endif
    
#ifdef STATEM_DEBUG
#define LOG_STATEM(L,...) LOG(L,"<State> " __VA_ARGS__)
#else
#define LOG_STATEM(...)
#endif
    
static inline uint8_t stateToSelection(uint8_t s) {return ((uint8_t)((s+1)/2) - 1);};
static inline const char *stateString(uint8_t s)
{
    static const char *strings[] = { "UN", "S0", "U0", "S1", "U1", "S2", "U2", "SM", "UM" };
    return strings[(uint8_t)s];
}
    
#ifdef __cplusplus
}
#endif

#endif /* fsm_support_h */
