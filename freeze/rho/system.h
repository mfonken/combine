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
} SYSTEM_STATE;

typedef struct
{
  SYSTEM_STATE state;
  void (*routine)(void);
  SYSTEM_STATE next;
} system_state_t;

typedef system_state_t system_states_t[NUM_SYSTEM_STATES];

typedef struct
{
  uint8_t id;
} system_header_t;

typedef struct
{
  /* Header */
  system_header_t header;
  
  /* State Control */
  SYSTEM_STATE state;
  system_states_t * state_list;
  
  /* Utilities */
  I2C_HandleTypeDef * i2c; 
  TIM_HandleTypeDef * timer; 
  USART_HandleTypeDef * usart;
} system_global_t;

void InitSystem( system_states_t *, I2C_HandleTypeDef *, TIM_HandleTypeDef *, USART_HandleTypeDef * );
void NextState(void);
void SetState( SYSTEM_STATE );

static system_global_t System =
{
  { DEFAULT_ID },
  STARTING, // State
  
};

#endif