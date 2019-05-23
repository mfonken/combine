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
  
  NUM_SYSTEM_STATES
} system_state_enum;

typedef struct
{
  system_state_enum 
    state, 
    next;
  void (*routine)(void);
} system_state_t;

typedef system_state_t system_states_list_t[NUM_SYSTEM_STATES];

typedef struct
{
  uint8_t id;
} system_header_t;

typedef struct
{
  /* Header */
  system_header_t header;
  
  /* State Control */
  system_state_enum state;
  system_states_list_t * state_list;
  
  /* Utilities */
  I2C_Handle_t * i2c; 
  TIMER_Handle_t * timer; 
  USART_Handle_t * usart;
} system_global_t;

void InitSystem( system_states_list_t *, I2C_Handle_t *, TIMER_Handle_t *, USART_Handle_t * );
void NextState(void);
void SetState( system_state_enum );

static system_global_t System =
{
  { DEFAULT_ID },
  STARTING, // State
};

#endif