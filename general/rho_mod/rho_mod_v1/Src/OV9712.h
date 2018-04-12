#ifndef OV9712_h
#define OV9712_h

#include "OV9712_regs.h"
#include "stm32l4xx_hal.h"

#define INIT_DELAY_CYCLES	1000

typedef struct 
{
	void (*init)(I2C_HandleTypeDef *);
	void (*write)(uint8_t, uint8_t);
} OV9712;

extern OV9712 Camera;


#endif
