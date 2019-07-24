#ifndef rho_support_h
#define rho_support_h

#include "rho_global.h"

#define SPOOF_CENTER_A_F  0.25
#define SPOOF_CENTER_B_F  0.75

#define SPOOF_RADIUS_A_F  10
#define SPOOF_RADIUS_B_F  9

static void SpoofDensityMap( dmap_t * map, index_t len )
{
  floating_t fl = (floating_t)len, fv;
  index_t xa = (index_t)(SPOOF_CENTER_A_F * fl),
          xb = (index_t)(SPOOF_CENTER_B_F * fl);
  int16_t x;
  
  memset( (uint8_t *)map, 0, len );
  
  /* Build A */
  x = (int16_t)xa - (int16_t)SPOOF_RADIUS_A_F; if( x < 0 ) x = 0;
  for( index_t i = 0; i < 2 * SPOOF_RADIUS_A_F && x < len; i++ )
  {
    fv = (floating_t)i - SPOOF_RADIUS_A_F;
    map[x++] = (index_t)sqrt( SPOOF_RADIUS_A_F * SPOOF_RADIUS_A_F - fv * fv ) << 1;
  }
  
   /* Build B */
  x = (int16_t)xb - (int16_t)SPOOF_RADIUS_B_F; if( x < 0 ) x = 0;
  for( index_t i = 0; i < 2 * SPOOF_RADIUS_B_F && x < len; i++ )
  {
    fv = (floating_t)i - SPOOF_RADIUS_B_F;
    map[x++] = (index_t)sqrt( SPOOF_RADIUS_B_F * SPOOF_RADIUS_B_F - fv * fv ) << 1;
  }
}

#endif /* rho_support_h */