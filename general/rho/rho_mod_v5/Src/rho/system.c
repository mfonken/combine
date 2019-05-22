#include "system.h"

void InitSystem(system_states_t * states, I2C_Handle_t * i2c, TIM_Handle_t * timer, USART_Handle_t * usart )
{
  System.state_list = states;
  System.i2c = i2c;
  System.timer = timer;
  System.usart = usart;
  
  SetState(STARTING);
}

system_state_t GetState(void)
{
  return (*System.state_list)[System.state];
}

void NextState(void)
{
  SetState( GetState().next );
}

void SetState( SYSTEM_STATE new_state )
{   
  System.state = new_state;
  
  GetState().routine();
}