/************************************************************************
 *  File: printers.h
 *  Group: PCR Core
 ***********************************************************************/

#ifndef printers_h
#define printers_h

#include "resources.h"
#include "platform.h"

#define PRINT_BUFFER_LEN ( 1 << 8 )
static char str_buf[PRINT_BUFFER_LEN];

#ifndef LOG_LEVEL
#define LOG_LEVEL
enum LogLevel
{
    OFF = -1,
    TEST = 0,
    DEBUG_0,
    DEBUG_1,
    DEBUG_2,
    ALWAYS
};
#endif

#define CORE_LOG_LEVEL          DEBUG_2

#ifndef LOG
#define LOG(L,...)              if(L >= CORE_LOG_LEVEL) \
                                {  for(uint8_t i=L;i<ALWAYS;i++) \
                                print("\t"); sprintf(str_buf, __VA_ARGS__); print(str_buf); print(ENDL); }
#define LOG_BARE(L,...)         if(L >= CORE_LOG_LEVEL) \
                                { sprintf(str_buf, __VA_ARGS__); print(str_buf); }
#endif

#define STATE_DEBUG             DEBUG_2

void print( char * Buf );
void DrawCurve( uint8_t * a, int32_t l );
void PrintDualBuffer( uint8_t * x, int32_t lx, uint8_t * y, int32_t ly );
void PrintBuffer( uint8_t * a, int32_t l );

#endif
