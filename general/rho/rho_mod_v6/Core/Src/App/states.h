#ifndef STATE_H_
#define STATE_H_

#define CUSTOM_SYSTEM_STATES

typedef enum
{
  INITIALIZING = 1,
  CONNECTING_TO_HOST,
  CONFIGURING,
  READY,
  ACTIVE,
  IDLE,
  RECONFIGURING,
  SYS_ERROR,
  NUM_SYSTEM_STATES
} system_state_enum;

static const char * system_state_enum_strings[] =
{
  "INITIALIZING",
  "CONNECTING_TO_HOST",
  "CONFIGURING",
  "READY",
  "ACTIVE",
  "IDLE",
  "RECONFIGURING",
  "SYS_ERROR",
  "UNKNOWN"
};

#endif /* STATE_H_ */
