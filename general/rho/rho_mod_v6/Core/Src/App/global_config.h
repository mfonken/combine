/************************************************************************
 *  File: global_config.h
 *  Group: Rho Core
 ***********************************************************************/

#ifndef global_config_h
#define global_config_h

#include "global_types.h"

#error "This is only a template, copy and customize for application"

#define THIS_ID           0x01

/* Global configurations */
#define HOST_COMMUNICATION_PROTOCOL     USART_PROTOCOL_TYPE
#define HOST_DEFAULT_CHANNEL            UART_Primary

#define CAMERA_COMMUNICATION_PROTOCOL   I2C_PROTOCOL_TYPE
#define CAMERA_COMMUNICATION_CHANNEL    I2C_Primary

#define USART_TIMEOUT	              100
#define UART_TIMEOUT	              USART_TIMEOUT

#endif /* global_config_h */
