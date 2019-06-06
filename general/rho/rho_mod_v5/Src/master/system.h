#ifndef system_h
#define system_h

#include <stdint.h>
#ifdef __RHO__
#include "rho_master.h"
#else
#ifdef __OV9712__
#include "OV9712.h"
#else
#include "platform.h"
#endif
#endif

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
} system_t;

static system_t System =
{
  { DEFAULT_ID },
  STARTING, // State
};

void InitSystem( system_t * );
void NextStateSystem( system_t * );
system_state_t GetStateSystem( system_t * );
void SetStateSystem( system_t *,  system_state_enum );

typedef struct
{
  void (*Next)( system_t * );
  system_state_t (*Get)( system_t * );
  void (*Set)( system_t *,  system_state_enum );
} system_state_functions;

typedef struct
{
  void (*Init)( system_t * );
  system_state_functions State;
} system_functions;

static system_functions SystemFunctions =
{
  .Init = InitSystem,
  .State.Next =  NextStateSystem,
  .State.Get = GetStateSystem,
  .State.Set = SetStateSystem
};

#endif
