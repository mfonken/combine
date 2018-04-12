#include "master.h"
#include <string.h>

uint8_t readyString[] = "Ready\r\n",
				testStartString[] = "Starting Test\r\n",
				alertString[] = "Alert!\r\n";

extern void rho_init(void);
extern void frame_start(void);
extern void frame_end(void);
extern void row_int(void);
extern void pclk_int(void);
extern void pclk_swi(void);
extern void rho_process(void);

extern void asm_test(void);

static uint8_t USB_TX(uint8_t* Buf)
{
	uint16_t Len = 0;
	uint8_t c = 0xff;
	for( ; c != '\0'; c = Buf[Len]) Len++;
	
  uint8_t result = USBD_OK;

  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  if ((Len%64==0) && (result==USBD_OK))
  {
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, 0);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  }
  return result;
}

void master_init( I2C_HandleTypeDef * i2c_port )
{
	HAL_GPIO_WritePin( CAM_EN_GPIO_Port, CAM_EN_Pin, GPIO_PIN_SET );
	//Camera.init(i2c_port);
	USB_TX( readyString );
}

void master_test( void )
{
	USB_TX( testStartString );
	HAL_Delay(1000);
	USB_TX( alertString );
	
	while(1);
	
	rho_init();
	frame_start();
	asm_test();
	frame_end();
	row_int();
	pclk_int();
	rho_process();
	while(1);
}
