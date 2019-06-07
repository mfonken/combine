#ifndef master_h
#define master_h

/***************************************************************************************/
/*                                    Includes                                         */
/***************************************************************************************/
#include "system.h"

/***************************************************************************************/
/*                              Function Definitions                                   */
/***************************************************************************************/
void Master_Init( I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t *, bool );
void Master_Run( void );

/***************************************************************************************/
/*                              Function Structures                                    */
/***************************************************************************************/
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

/***************************************************************************************/
/*                                  Routines Definitions                               */
/***************************************************************************************/
static void InitializePlatform();
static void ConnectToHost();
static void ConfigureApplication();
static void ExitInitialization();
static inline void ApplicationCore();

/***************************************************************************************/
/*                                Core State List                                      */
/***************************************************************************************/
system_states_list_t global_states_list =
{
  { INITIALIZING,       CONNECTING_TO_HOST, InitializePlatform    },
  { CONNECTING_TO_HOST, CONFIGURING,        ConnectToHost         },
  { CONFIGURING,        READY,              ConfigureApplication  },
  { READY,              ACTIVE,             ExitInitialization    },
  { ACTIVE,             IDLE,               ApplicationCore       }
  { 0 },
};

#endif
