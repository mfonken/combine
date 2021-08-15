/************************************************************************
 *  File: global_types.h
 *  Group: PCR Core
 ***********************************************************************/

#ifndef global_types_h
#define global_types_h

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//typedef struct
//{
//  uint8_t Address;
//  uint8_t Value;
//} register_t;

typedef volatile bool flag_t;
typedef struct
{
    flag_t Flag;
    void (*Callback)(void);
} callback_t;
typedef uint32_t * generic_handle_t;

typedef struct
{
flag_t
  Active,
  IRQ,
  Frame,
  Row,
  Backgrounding,
  UARTBusy,
  EvenRowToggle,
  BufferUpdate;
callback_t
  Capture;
} camera_application_flags;

#endif /* global_types_h */
