/************************************************************************
 *  File: platform_interface_master.h
 *  Group: Rho Hardware Interface
 ***********************************************************************/

#ifndef platform_interface_master_h
#define platform_interface_master_h

/* Platform-Specific interfaces */
#ifdef __STM32__
#include "stm32_interface.h"
#else
#error "No valid platform specified."
#endif



#endif /* platform_interface_master_h */
