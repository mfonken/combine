#include "system.h"

void InitSystem(
  system_t * system,
  system_states_list_t * state_list )
{
  system->state_list = state_list;
//  system.state_list = states;
  SystemFunctions.State.Set( system, system->state );
}

void NextStateSystem( system_t * system )
{
  SystemFunctions.State.Enter( system, SystemFunctions.State.Get(system).next );
}

system_state_t GetStateSystem( system_t * system )
{
  return *(system->state_list)[system->state];
}

void SetStateSystem( system_t * system, system_state_enum new_state )
{
  System.state = new_state;
}

void PerformStateSystem( system_t * system )
{
  void (*routine)(void) = SystemFunctions.State.Get(system).routine;
  if( routine != NULL ) routine();
}

void EnterStateSystem( system_t * system, system_state_enum new_state )
{
  SystemFunctions.State.Set( system, new_state);
  SystemFunctions.State.Perform( system );
}

bool IsInStateSystem( system_t * system, system_state_enum check_state )
{
  return ( SystemFunctions.State.Get( system ).state == check_state );
}
