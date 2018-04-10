#include "master.h"
#include <string.h>

extern void rho_init(void);
extern void frame_start(void);
extern void frame_end(void);
extern void row_int(void);
extern void pclk_int_l(void);
extern void pclk_int_h(void);
extern void rho_process(void);

extern void asm_test(void);

void master_init( I2C_HandleTypeDef * i2c_port )
{
	HAL_GPIO_WritePin( CAM_EN_GPIO_Port, CAM_EN_Pin, GPIO_PIN_SET );
	HAL_Delay(0);
	Camera.init(i2c_port);
}

void master_test( void )
{
	rho_init();
	frame_start();
	
	asm_test();
	frame_end();
	row_int();
	pclk_int_l();
	pclk_int_h();
	pclk_int_l();
	pclk_int_h();
	rho_process();
	while(1);
}
