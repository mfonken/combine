/************************************************************************
 *  File: printers.c
 *  Group: PCR Core
 ***********************************************************************/

#include "printers.h"

/***************************************************************************************/
/*                                    Printers                                         */
/***************************************************************************************/

inline void print( char * Buf )
{
  TransmitToHost( (uint8_t *)Buf, strlen((const char *)Buf) );
}

#define MAX_PRINT_INDEX 40
#define DENSITY_PRINT_SMOOTH_THRESH 1
#define DENSITY_PRINT_SHARP_THRESH 4
void DrawCurve( uint8_t * a, int32_t l )
{
    uint8_t curr, prev = 0;
    int32_t diff = 0, vert = 0;
    for( uint32_t i = 0; i < l; i++ )
    {
        curr = a[i];
        sprintf(str_buf, "%3d:", i);
        print( str_buf );
        if( curr > MAX_PRINT_INDEX ) curr = MAX_PRINT_INDEX;
        diff = (int32_t)curr - (int32_t)prev;
        vert = curr < prev ? curr : prev;
        prev = curr;

        for( ; vert > 0; vert--) print(" ");
        int d = abs(diff);
        if(diff > 0) for( ; d > 1; d--) print("¯");
        else for( ; d > 2; d--) print("_");
        if( diff > DENSITY_PRINT_SMOOTH_THRESH )
            print("\\");
        else if( diff < -DENSITY_PRINT_SMOOTH_THRESH )
            print("/");
        else if( curr == MAX_PRINT_INDEX)
            print(">");
        else
            print("|");
        print(ENDL);
    }
}

void PrintDualBuffer( uint8_t * x, int32_t lx, uint8_t * y, int32_t ly )
{
  uint16_t del = UNIVERSAL_DELIMITER;
  print((char *)&del);
  PrintBuffer( x, lx );
  PrintBuffer( y, ly );
  print((char *)&del);
}

void PrintBuffer( uint8_t * a, int32_t l )
{
    int32_t blocks = 1 + ( ( l - 1 ) / PRINT_BUFFER_LEN ), i, j = 0;
    while( blocks-- >= 0 )
    {
        for( i = 0; i < PRINT_BUFFER_LEN && j < l; i++, j++ )
            str_buf[i] = a[j] > 255 ? 255 : a[j];
        TransmitToHost( (uint8_t *)str_buf, i );
    }
}
