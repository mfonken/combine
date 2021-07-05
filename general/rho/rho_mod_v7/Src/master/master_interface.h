/************************************************************************
 *  File: master_interface.h
 *  Group: Rho Core
 ***********************************************************************/

#ifndef master_interface_h
#define master_interface_h

#include "global_config.h"

/***************************************************************************************/
/*                                  Master Definitions                                   */
/***************************************************************************************/
typedef struct
{
  I2C_Handle_t * I2C_Primary;
  UART_Handle_t * UART_Primary;
//  USB_Handle_t * USB_Primary;
} master_ios_t;

typedef struct
{
  TIMER_Handle_t * Timer_Primary;
} master_utilities_t;

typedef struct
{
  master_ios_t IOs;
  master_utilities_t Utilities;
} master_t;

extern master_t Master;

#endif
