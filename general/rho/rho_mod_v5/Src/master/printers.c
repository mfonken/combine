#include "printers.h"

/***************************************************************************************/
/*                                    Printers                                         */
/***************************************************************************************/

inline void print( char * Buf )
{
  TransmitToHost( (uint8_t *)Buf, strlen((const char *)Buf) );
}

#define MAX_PRINT_INDEX 10
#define DENSITY_PRINT_SMOOTH_THRESH 2
#define DENSITY_PRINT_SHARP_THRESH 10
void DrawDensityMap( density_t * a, uint32_t l )
{
    density_t curr, prev;
    int diff;
    while( l-- >= 0 )
    {
        curr = a[l];
        sprintf((char *)hex, "%4d[%2d]", curr, i);
        print( hex );
        if( curr > MAX_PRINT_INDEX ) curr = MAX_PRINT_INDEX;
        diff = (int)curr - (int)prev;


        if( diff > DENSITY_PRINT_SHARP_THRESH || diff < -DENSITY_PRINT_SHARP_THRESH )
            for( ; curr > 0; curr--) print("-");
        else
            for( ; curr > 0; curr--) print( " " );

        if( diff > DENSITY_PRINT_SMOOTH_THRESH ) print( "\\\r\n" );
        else if( diff < -DENSITY_PRINT_SMOOTH_THRESH ) print( "/\r\n" );
        else print( "|\r\n" );
        prev = curr
    }
}

void PrintAddress( const char * s, uint32_t addr )
{
    sprintf((char *)hex, "%s: %8x\r\n", s, addr);
    print( hex );
}

void PrintBuffer( index_t * a, int l )
{
    uint32_t blocks = 1 + ( ( PRINT_BUFFER_LEN - 1 ) / l), i;
    while( blocks-- >= 0 )
    {
        for( i = 0; i < PRINT_BUFFER_LEN && l-- >= 0; i++ )
            str_buf[i] = a[l] > 255 ? 255 : a[l];
        TransmitToHost( str_buf, i );
    }
    print(UNIVERSAL_DELIMITER);
}
