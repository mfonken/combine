#ifndef OV9712_h
#define OV9712_h

#include "global_lib.h"
#include "OV9712_regs.h"

static void OV9712_Init( I2C_Handle_t * );
static void OV9712_Write( uint8_t, uint8_t );
static void OV9712_Enable( void );
static void OV9712_Disable( void );

typedef struct 
{
  void (*Init)( I2C_Handle_t *);
  void (*Write)( uint8_t, uint8_t);
  void (*Enable)( void );
  void (*Disable)( void );
} OV9712_functions;

OV9712_functions OV9712 = 
{
  .Init = OV9712_Init,
  .Write = OV9712_Write,
  .Enable = OV9712_Enable,
  .Disable = OV9712_Disable
};

#endif
