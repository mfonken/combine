//
//  genlog.h
//  combine
//
//  Created by Matthew Fonken on 11/21/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef genlog_h
#define genlog_h

#include "master.h"

#ifndef LOG_LEVEL
#define LOG_LEVEL

enum LogLevel
{
    TEST = 0,
    DEBUG_0,
    DEBUG_1,
    DEBUG_2,
    ALWAYS,
    OFF
};
#endif

#ifndef LOG
#define LOG(L,...)              if(L >= GLOBAL_LOG_LEVEL) \
                                {  for(uint8_t i=L;i<ALWAYS;i++) \
                                printf("\t"); printf(__VA_ARGS__); }
#warning TODO: Implement printf for EFR32
#define LOG_BARE(L,...)         if(L >= GLOBAL_LOG_LEVEL) \
                                { printf(__VA_ARGS__); }
#endif

#endif /* genlog_h */
