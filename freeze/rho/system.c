#include "system.h"

void InitSystem(system_states_t * states, I2C_HandleTypeDef * i2c, TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
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