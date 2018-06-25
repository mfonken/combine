#ifndef printers_h
#define printers_h

#include "Rho\rho_utility.h"

void dprintBuffers( void );
void dprintCapture( void );
void printBuffers( uint32_t s );
void printAddress( const char * s, uint32_t addr );
void printAddresses( void );
void printCapture( void );
void printBuffer( index_t * a, int l );
void drawDensityMap( density_t * a, int l );
void printPredictionPair( prediction_pair_t * pr );

#endif
