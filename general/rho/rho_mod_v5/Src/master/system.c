#include "system.h"

void InitSystem(
  system_t * system,
  system_states_list_t * states,
  system_state_enum starting_state )
{
  system.state_list = states;
  SystemFunctions.State.Set( system, starting_state);
}

void NextStateSystem( system_t * system )
{
  SystemFunctions.State.Set( system, SystemFunctions.State.Get(system).next );
}

system_state_t GetStateSystem( system_t * system )
{
  return (*system.state_list)[System.state];
}

void SetStateSystem( system_t * system, system_state_enum new_state )
{
  System.state = new_state;

  SystemFunctions.State.Get(system).routine();
}
