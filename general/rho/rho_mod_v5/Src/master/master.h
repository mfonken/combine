#ifndef master_h
#define master_h

#include "global_lib.h"

#define MASTER_RUN_AFTER_INIT true


void master_init( I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t *, bool );
void master_run( void );

typedef struct
{
  void (*Init)( I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t *, bool );
  void (*Run)( void );
} master_functions_t;

static master_functions_t Master;

#endif
