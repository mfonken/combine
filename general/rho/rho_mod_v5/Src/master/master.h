#ifndef master_h
#define master_h

#ifdef __RHO__
#include "rho_master.h"
#else
#include "global_lib.h"
#endif

#define MASTER_RUN_AFTER_INIT true

void Master_Init( I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t *, bool );
void Master_Run( void );

typedef struct
{
  void (*Init)( I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t *, bool );
  void (*Run)( void );
} master_functions_t;

static master_functions_t Master =
{
  .Init = Master_Init,
  .Run  = Master_Run
};

#endif
