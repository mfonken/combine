#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc.h"

#define APP_RX_DATA_SIZE  1000

#define  MaxCommandsInBuffer 10

void master_init( I2C_HandleTypeDef * );
void master_test( void );

static struct
{
    int pos_receive, pos_process; //pos_receive is the current position in buffer to save incoming data. pos_process is the index of data in buffer which has been processed.
    //if pos_receive=pos_process, it means all data were processed, waiting for new data coming
    unsigned char IsCommandDataReceived; //anynumber >0 means data were received. 0 means no data is available
    uint8_t UserRxBufferFS[MaxCommandsInBuffer][APP_RX_DATA_SIZE];//it could save <MaxCommandsInBuffer> number of commands
    uint8_t CommandsLens[MaxCommandsInBuffer]; //save the len of each command
} s_RxBuffers;

#endif
