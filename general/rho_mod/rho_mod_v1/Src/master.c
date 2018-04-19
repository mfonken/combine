#include "master.h"
#include <string.h>

uint8_t readyString[] = "Ready\r\n",
				testStartString[] = "Starting Test\r\n",
				alertString[] = "Alert!\r\n",
				interruptString[] = "Interrupt #",
				rowStartString[] = "Starting row\r\n",
				frameStartString[] = "Starting frame\r\n";

extern void rho_init(void);
extern void frame_start(void);
extern void frame_end(void);
extern void row_int(void);
extern void pclk_int(void);
extern void pclk_swi(void);
extern void rho_process(void);

extern void asm_test(void);

/************************************************/
/******************** Memory ********************/
/************************************************/
capture_t
	CAPTURE_BUFFER[CAPTURE_BUFFER_SIZE];	/* Raw capture buffer for DMA */
index_t
	THRESH_BUFFER[THRESH_BUFFER_SIZE];		/* Threshold processing buffer */
density_t
	DENSITY_X[CAPTURE_WIDTH],										/* Processed density X array */
	DENSITY_Y[CAPTURE_HEIGHT],										/* Processed density Y array */
	QUADRANT_BUFFER[4],																	/* Quadrant density array */
	QUADRANT_TOTAL,															/* Total density */
	QUADRANT_PREVIOUS;																/* Previous row density */
index_t
	CENTROID_X,
	CENTROID_Y;
density_t
	DENSITY_Y_MAX;
address_t
	CAPTURE_BUFFER_END,
	CAPTURE_BUFFER_MAX,
	THRESH_BUFFER_END,										/* Max threshold buffering size */
	THRESH_BUFFER_MAX,
	THRESH_VALUE,													/* Shared threshold value */
	RHO_MEM_END;


/************************************************/

uint32_t CAMERA_PORT;
uint8_t proc_flag = 0;
uint32_t flag = 0;

#define HEX_LEN 20
uint8_t hex[HEX_LEN];
uint16_t x, y;

//static void TransferComplete(DMA_HandleTypeDef *DmaHandle) { flag = 1; }
static void printAddr( const char * s, uint32_t addr )
{
	sprintf((char *)hex, "%s: %8x\r\n", s, addr);
	USB_TX( hex );
	HAL_Delay(100);
}

static void initTimerDMA( TIM_HandleTypeDef * timer, DMA_HandleTypeDef * dma )
{	
	USB_TX("Starting DMA from ");
	sprintf((char *)hex, "0x%8x > 0x%8x\r\n", (uint32_t)CAMERA_PORT, (uint32_t)CAPTURE_BUFFER);
	USB_TX( hex );
	//timer->hdma[TIM2_DMA_ID]->XferCpltCallback = TransferComplete;
  if(HAL_DMA_Start_IT(timer->hdma[TIM2_DMA_ID], CAMERA_PORT, (uint32_t)CAPTURE_BUFFER, CAPTURE_BUFFER_SIZE) != HAL_OK) Error_Handler();
  __HAL_TIM_ENABLE_DMA(timer, TIM2_DMA_CC);
  __HAL_TIM_ENABLE_IT(timer, TIM2_IT_CC);
  TIM_CCxChannelCmd(timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
//static void pauseDMA( TIM_HandleTypeDef * timer )
//{
//	TIM_CCxChannelCmd(timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
//}

//static void resumeDMA( TIM_HandleTypeDef * timer )
//{
//	TIM_CCxChannelCmd(timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
//}

void zero_memory( void )
{
	memset(CAPTURE_BUFFER, 	0x11, sizeof(capture_t) * CAPTURE_BUFFER_SIZE);
	memset(THRESH_BUFFER, 	0, sizeof(byte_t)		 * THRESH_BUFFER_SIZE	);
	memset(DENSITY_X,				0, sizeof(density_t) * CAPTURE_WIDTH	 		);
	memset(DENSITY_Y,				0, sizeof(density_t) * CAPTURE_HEIGHT			);
	memset(QUADRANT_BUFFER,	0, sizeof(density_t) * 4									);
	QUADRANT_PREVIOUS = 0;												
}

static void init_memory( void )
{
	zero_memory();
	CAMERA_PORT = (address_t)&(GPIOA->IDR);
	CAPTURE_BUFFER_END = (address_t)CAPTURE_BUFFER;
	CAPTURE_BUFFER_MAX = (address_t)CAPTURE_BUFFER + CAPTURE_BUFFER_SIZE;
	THRESH_BUFFER_MAX = (address_t)THRESH_BUFFER + THRESH_BUFFER_SIZE;
	THRESH_BUFFER_END = THRESH_BUFFER_MAX; //(address_t)THRESH_BUFFER;
	CENTROID_X = 5;
	CENTROID_Y = 2;
	THRESH_VALUE = 0x10;
}

static void printBuffer( index_t * a, int l )
{
	for( int i = 0; i < l; i++ )
	{
		index_t curr = a[i];
		if( curr == 0xaaaa )
			sprintf((char *)hex, "\r\n(%d):", i);
		else
			sprintf((char *)hex, " 0x%x", curr);
		USB_TX( hex );
	}
	USB_TX("\r\n");
}

static void drawDensityMap( density_t * a, int l )
{
	for( int i = 0; i < l; i++ )
	{
		density_t curr = a[i];
		sprintf((char *)hex, "%4d(%2d)", curr, i);
		USB_TX( hex );
		if( curr > 10 ) curr = 10;
		for( ; curr > 0; curr--) USB_TX( " " );
		USB_TX( "|\r\n" );
	}
}

void printBuffers( void )
{
	USB_TX( "Printing Thresh Buffer\r\n" );
	printBuffer(THRESH_BUFFER, THRESH_BUFFER_SIZE);
	
	USB_TX( "Printing Dx\r\n" );
	drawDensityMap(DENSITY_X, THRESH_BUFFER_SIZE);
	
	USB_TX( "Printing Dy\r\n" );
	drawDensityMap(DENSITY_Y, THRESH_BUFFER_SIZE);
	
	frame_start();
}

void delay( uint32_t ms )
{
	HAL_Delay(ms);
}

void master_init( TIM_HandleTypeDef * timer, DMA_HandleTypeDef * dma, I2C_HandleTypeDef * i2c )
{
	HAL_Delay(2000);	
	init_memory();
	initTimerDMA( timer, dma );
	
	printAddr("CamPt", (uint32_t)CAMERA_PORT);
	printAddr("C bfr", (uint32_t)CAPTURE_BUFFER);
	printAddr("C end", (uint32_t)CAPTURE_BUFFER_END);
	printAddr("C max", (uint32_t)CAPTURE_BUFFER_MAX);
	printAddr("T bfr", (uint32_t)THRESH_BUFFER);
	printAddr("   Dx", (uint32_t)DENSITY_X);
	printAddr("   Dy", (uint32_t)DENSITY_Y);
	printAddr("    Q", (uint32_t)QUADRANT_BUFFER);
	printAddr("Q tot", (uint32_t)&QUADRANT_TOTAL);
	printAddr("Q prv", (uint32_t)&QUADRANT_PREVIOUS);
	printAddr("   Cx", (uint32_t)&CENTROID_X);
	printAddr("   Cy", (uint32_t)&CENTROID_Y);
	printAddr("   Ym", (uint32_t)&DENSITY_Y_MAX);
	printAddr("T end", (uint32_t)&THRESH_BUFFER_END);
	printAddr("T val", (uint32_t)&THRESH_VALUE);	
	printAddr("M end", (uint32_t)&RHO_MEM_END);
	
	//Camera.init(i2c);
	USB_TX( readyString );
	
	/*
	asm_test();
	*/
	HAL_Delay(4000);
	USB_TX("-");
	HAL_Delay(1000);
	while(1)
	{		
		pclk_int();
		row_int();
	}
}

void master_test( void )
{
	USB_TX( testStartString );
	HAL_Delay(1000);
	
	while(1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{
	switch(GPIO_Pin)
	{
		case VSYNC_Pin:
			frame_start();
		case HREF_Pin:
			row_int();
		default:
			break;
	}
}

