/************************************************************************
 *  File: global_types.h
 *  Group: PCR Core
 ***********************************************************************/

#ifndef global_types_h
#define global_types_h

/***************************************************************************************/
/*                                    Includes                                         */
/***************************************************************************************/
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
