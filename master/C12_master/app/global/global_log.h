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
#include <stdarg.h>
#include "em_gpio.h"

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

#define MAX_STR_LEN (1 << 8)
static void printd(char * str, ...)
{
  char buffer[MAX_STR_LEN];
  va_list args;
  va_start (args, str);
  vsnprintf (buffer, MAX_STR_LEN-1, str, args);
  for(int i = 0; buffer[i] != '\0' && i < MAX_STR_LEN; i++)
	  ITM_SendChar(buffer[i]);
}

#ifndef LOG
#define LOG(L,...)              if(L >= GLOBAL_LOG_LEVEL) \
                                {  for(uint8_t i=L;i<ALWAYS;i++) \
                                printd("\t"); printd(__VA_ARGS__); }
#warning TODO: Implement printf for EFR32
#define LOG_BARE(L,...)         if(L >= GLOBAL_LOG_LEVEL) \
                                { printd(__VA_ARGS__); }
#endif

#endif /* genlog_h */
