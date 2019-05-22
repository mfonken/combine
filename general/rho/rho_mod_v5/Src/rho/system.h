#ifndef system_h
#define system_h

#include <stdint.h>
#include "global_lib.h"

#define DEFAULT_ID      0xab

typedef enum
{
  STARTING = 0,
  INITIALIZING,
  CONNECTING_TO_HOST,
  CONFIGURING,
  READY,
  ACTIVE,
  INACTIVE,
  RECONFIGURING,
  
  NUM_system_state_tS
} system_state_t;

typedef struct
{
  system_state_t state;
  void (*routine)(void);
  system_state_t next;
} system_state_t_t;

typedef system_state_t_t system_state_ts_t[NUM_system_state_tS];

typedef struct
{
  uint8_t id;
} system_header_t;

typedef struct
{
  /* Header */
  system_header_t header;
  
  /* State Control */
  system_state_t state;
  system_state_ts_t * state_list;
  
  /* Utilities */
  I2C_Handle_t * i2c; 
  TIM_Handle_t * timer; 
  USART_Handle_t * usart;
} system_global_t;

void InitSystem( system_state_ts_t *, I2C_Handle_t *, TIM_Handle_t *, USART_Handle_t * );
void NextState(void);
void SetState( system_state_t );

static system_global_t System =
{
  { DEFAULT_ID },
  STARTING, // State
};

#endif