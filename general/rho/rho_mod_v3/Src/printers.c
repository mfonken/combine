#include "printers.h"

/***************************************************************************************/
/*                                    Printers                                         */
/***************************************************************************************/
void printBuffers( uint32_t s )
{
    print( "Printing Thresh Buffer\r\n" );
    printBuffer( THRESH_BUFFER, thresh_buffer_size );

    print( "Printing Dx\r\n" );
    drawDensityMap(DENSITY_X, CAPTURE_BUFFER_HEIGHT);

    print( "Printing Dy\r\n" );
    drawDensityMap(DENSITY_Y, CAPTURE_BUFFER_WIDTH);
}

void printAddress( const char * s, uint32_t addr )
{
    sprintf((char *)hex, "%s: %8x\r\n", s, addr);
    print( hex );
}

void printAddresses( void )
{
    printAddress("CamPt", (uint32_t)CAMERA_PORT);
    printAddress("C bfr", (uint32_t)CAPTURE_BUFFER);
    printAddress("C end", (uint32_t)CAPTURE_BUFFER_END);
    printAddress("C max", (uint32_t)CAPTURE_BUFFER_MAX);
    printAddress("T bfr", (uint32_t)THRESH_BUFFER);
    printAddress("T end", (uint32_t)THRESH_BUFFER_END);
    printAddress("T max", (uint32_t)THRESH_BUFFER_MAX);
    printAddress("   Dx", (uint32_t)DENSITY_X);
    printAddress("   Dy", (uint32_t)DENSITY_Y);
    printAddress("    Q", (uint32_t)QUADRANT_BUFFER);
    printAddress("Q tot", (uint32_t)&QUADRANT_TOTAL);
    printAddress("Q prv", (uint32_t)&QUADRANT_PREVIOUS);
    printAddress("   Cx", (uint32_t)&CENTROID_X);
    printAddress("   Cy", (uint32_t)&CENTROID_Y);
    printAddress("   Xm", (uint32_t)&DENSITY_X_MAX);
    printAddress("T val", (uint32_t)&THRESH_VALUE);
    printAddress("M end", (uint32_t)&RHO_MEM_END);
}

void printCapture( void )
{
	for(int y = 0; y < CAPTURE_BUFFER_HEIGHT; y++ )
	{
		for(int x = 0; x < CAPTURE_BUFFER_WIDTH; x++ )
		{
			uint32_t p = x + y * CAPTURE_BUFFER_WIDTH;
			sprintf((char *)hex, " 0x%2x", CAPTURE_BUFFER[p]);
			print( hex );
		}
		print( "\r\n" );
	}
}

void printBuffer( index_t * a, int l )
{
    for( int i = 0; i < l; i++ )
    {
        index_t curr = a[i];
        if( curr == 0xaaaa )
            sprintf((char *)hex, "\r\n(%d):", i);
        else
            sprintf((char *)hex, " 0x%x", curr);
        print( hex );
    }
    print("\r\n");
}

void drawDensityMap( density_t * a, int l )
{
    for( int i = 0; i < l; i++ )
    {
        density_t curr = a[i];
        sprintf((char *)hex, "%4d[%2d]", curr, i);
        print( hex );
        if( curr > 10 ) curr = 10;
        for( ; curr > 0; curr--) print( " " );
        print( "|\r\n" );
    }
}

void printPredictionPair( prediction_pair_t * pr )
{
	int xp = (int)pr->x.primary.value;
	int yp = (int)pr->y.primary.value;
	int xs = (int)pr->x.secondary.value;
	int ys = (int)pr->y.secondary.value;
	print("Printing predictions ");
	sprintf((char *)hex, "\t\t\tP(%d, %d) | S(%d, %d)\r\n", xp, yp, xs, ys);
	print( hex );
}
