#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc.h"

void master_init( I2C_HandleTypeDef * );
void master_test( void );

#endif
