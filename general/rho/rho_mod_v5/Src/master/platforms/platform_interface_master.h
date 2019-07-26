#ifndef platform_interface_master_h
#define platform_interface_master_h

#ifdef __STM32__
#include "stm32_interface.h"
#else
#error "No valid platform specified."
#endif

#endif /* platform_interface_master_h */
