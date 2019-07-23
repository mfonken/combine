#ifndef printers_h
#define printers_h

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
void DrawDensityMap( density_t * a, int l );
void PrintAddress( const char * s, uint32_t addr );
void PrintBuffer( index_t * a, int l );

#endif
