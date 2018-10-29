#ifndef OV9712_h
#define OV9712_h

#include "global_lib.h"
#include "OV9712_regs.h"

typedef struct 
{
	void (*init)(I2C_HandleTypeDef *);
	void (*write)(uint8_t, uint8_t);
	void (*enable)( void );
	void (*disable)( void );
} OV9712;

extern OV9712 Camera;


#endif
