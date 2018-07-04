#include "master.h"
#include <string.h>

/*
#include "master.h"
master_init( &hi2c1, &htim2, &huart1 );
*/

//#define TX_FRAME
#define ACTIVE_CAMERA

/************************* LOCAL HANDLES **************************/
UART_HandleTypeDef 	* this_uart;
TIM_HandleTypeDef 	* this_timer;
/******************************************************************/

/******************* STATIC LOCAL DEBUG GROUPS ********************/
#define HEX_LEN 20
uint8_t hex[HEX_LEN];

const uint8_t
				startingString[] = "******************\r\nStarting.\r\n",
				readyString[] = "Ready\r\n",
				testStartString[] = "Starting Test\r\n",
				alertString[] = "Alert!\r\n",
				interruptString[] = "Interrupt #";

const uint16_t BUFFER_TX_DEL = (0xabcd),
							 BUFFER_TX_END = (0x12ef);

#define UART_TIMEOUT	100
/******************************************************************/

/***************************** Memory *****************************/

system_t rho_system = DEFAULT_SYSTEM_INITIALIZATION;

capture_t * CAPTURE_BUFFER 	= svb.capture;
index_t 	* THRESH_BUFFER		= svb.thresh;
density_t * DENSITY_X				= svb.density_x,
					* DENSITY_Y				= svb.density_y,
					* QUADRANT_BUFFER = svb.quadrant;

density_t * QUADRANT_TOTAL 		= &svd.quadrant_total,
					*	QUADRANT_PREVIOUS = &svd.quadrant_previous,
					*	DENSITY_X_MAX 		= &svd.density_x_max;
index_t		*	CENTROID_X				= &svd.CENTROID_X,
					*	CENTROID_Y				= &svd.CENTROID_Y;
address_t	*	THRESH_BUFFER_END = &svd.thresh_end,
					*	THRESH_BUFFER_MAX = &svd.thresh_max,
					* THRESH_VALUE			= &svd.thresh_value;

uint16_t thresh_buffer_size = THRESH_BUFFER_SIZE;
/******************************************************************/

/*************** Static, Inline, & Extern Functions ***************/
extern inline void rho_init(void);
extern inline void frame_start(void);
extern inline void frame_end(void);
extern inline void row_int(void);
extern inline bool pixel_proc(void);
extern inline void rho_proc( uint32_t t );
extern inline void asm_test(void);

//static FLOAT now( void ) { return (FLOAT)HAL_GetTick()/1000; }
static void tog( void )
{ HAL_GPIO_TogglePin( CAM__CS_GPIO_Port, CAM__CS_Pin ); }
static void set( GPIO_PinState s )
{ HAL_GPIO_WritePin( CAM__CS_GPIO_Port, CAM__CS_Pin, s ); }
inline void irqEnable( void ) { svf.irq = 1; }
inline void irqDisable( void ) { svf.irq = 0; }
inline void rowFlagSet( void ) { svf.row = 1; }
inline void rowFlagReset( void ) { svf.row = 0; }
inline flag_t rowFlagGet( void ) { return svf.row; }
inline void frameFlagSet( void ) { svf.frame = 1; }
inline void frameFlagReset( void ) { svf.frame = 0; }
/******************************************************************/

inline void Point_Dmaps_To_Background( void )
{
	rho_system.variables.rho.density_map_pair.x.background = svb.density_x;
	rho_system.variables.rho.density_map_pair.y.background = svb.density_y;
}

inline void Point_Dmaps_To_Foreground( void )
{
	rho_system.variables.rho.density_map_pair.x.map = svb.density_x;
	rho_system.variables.rho.density_map_pair.y.map = svb.density_y;
}

void performBackgroundEvent( void )
{
	Point_Dmaps_To_Background();
	RhoFunctions.Perform( &rho_system.variables.rho, true );
	Point_Dmaps_To_Foreground();
}

void frameProcessor( void )
{
	while(!svf.frame);
	tog();
	svc.v = 1;
	zero_memory();

	/* * * * * Register Protection Start * * * * */
	//frame_start();
	while(!svf.row);
	svc.w = 0;
	uint32_t row_sum = 0, limit = svd.thresh_max - 10;
	while( svc.v < CAPTURE_HEIGHT && svc.w < limit )
	{
		while(svf.row);
		tog();
		svb.thresh[svc.w++] = (index_t)0xaaaa;

		svc.p = 0;
		svc.h = 0;

		while( ++svc.h < CAPTURE_WIDTH && !svf.row
			//&& pixel_proc()
		)//;
		{
			if( svb.capture[svc.h] > svd.thresh_value )
				svb.thresh[svc.w++] = (index_t)svc.h;
		}
		tog();
		//}
		while(!svf.row);
		svc.v++;
	}
	while(svf.frame);
	row_sum = 0;

	/* Rho Process (C) */
	index_t c = 0, y = 0, p = 0;
	for( ; p < svc.w; p++ )
	{
		c = svb.thresh[p];
		if( c == 0xaaaa )
		{
			svb.density_x[y++] = row_sum;
			row_sum = 0;
		}
		else if( c < CAPTURE_WIDTH )
		{
			svb.density_y[c]++;
			row_sum++;
		}
	}
	//rho_proc(0);
	svb.density_x[CAPTURE_BUFFER_HEIGHT-1] = 0xef;
	dprintBuffers();

	/* * * * *  Register Protection End  * * * * */
	//rho_system.variables.rho.density_map_pair.x.max = svb.density_x_max;
	//RhoFunctions.Perform( &Rho, false );
	tog();
}

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/
void master_init( I2C_HandleTypeDef * i2c, TIM_HandleTypeDef * timer, UART_HandleTypeDef * uart )
{
	this_uart = uart;
	this_timer = timer;
	irqDisable();
    /* Debug delay */
	UART_Clear();

	print( startingString );

	init_memory();
	printAddresses();

#ifdef ACTIVE_CAMERA
	Camera.init(i2c);
	initTimerDMA();
	irqEnable();
#endif
	svc.f = 0;
	while( svc.f < 1000 )
	{
		svc.f++;
		if( !(bool)(svc.f%10) )
			rho_system.functions.perform.Perform_Background_Event();
#ifdef ACTIVE_CAMERA
	#ifdef TX_FRAME
			frameTx();
	#else
			rho_system.functions.perform.Process_Frame();
	#endif
#else
	#ifdef TX_FRAME
			spoofFrameTx();
	#else
			spoofFrameProcessor();
	#endif
		HAL_Delay(5);
#endif
		HAL_Delay(45);
	}
#ifdef ACTIVE_CAMERA
	irqDisable();
	Camera.disable();
	rho_system.functions.utility.Pause_DMA();
#endif
  while(1);
}

void master_test( void )
{
	HAL_Delay(10);
	spoofDensityMaps();
	dprintBuffers();

	/* Generate background event */
	for( volatile uint8_t i = 0; i < 100; i++)
	{
		bool background_event = !(bool)(i%10);
		if( background_event )
			rho_system.functions.perform.Perform_Background_Event();
	}
	int xp = (int)rho_system.variables.rho.prediction_pair.x.primary.value;
	int yp = (int)rho_system.variables.rho.prediction_pair.y.primary.value;
	int xs = (int)rho_system.variables.rho.prediction_pair.x.secondary.value;
	int ys = (int)rho_system.variables.rho.prediction_pair.y.secondary.value;
	print("Printing predictions ");
	sprintf((char *)hex, "\tP(%d, %d) | S(%d, %d)\r\n", xp, yp, xs, ys);
	print( hex );
	return;
	while(1);
}
/***************************************************************************************/
/*                                    Callbacks                                        */
/***************************************************************************************/
inline void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(!svf.irq) return;
	switch(GPIO_Pin)
	{
		case VSYNC_Pin:
			svf.frame = !(flag_t)(VSYNC_GPIO_Port->IDR & VSYNC_Pin);
			return;
		case HREF_Pin:
			svf.row 	=  (flag_t)( HREF_GPIO_Port->IDR & HREF_Pin);
			return;
		default:
			return;
	}
}

void initTimerDMA( void )
{
    if(HAL_DMA_Start_IT(this_timer->hdma[TIM2_DMA_ID], svd.camera_port, (uint32_t)svb.capture, CAPTURE_BUFFER_SIZE) != HAL_OK) Error_Handler();
    __HAL_TIM_ENABLE_DMA(this_timer, TIM2_DMA_CC);
    __HAL_TIM_ENABLE_IT(this_timer, TIM2_IT_CC);
    TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
inline void pauseDMA( void )
{
    TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
}
inline void resumeDMA( void )
{
    TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
inline void resetDMA( void )
{
	this_timer->hdma[TIM2_DMA_ID]->Instance->CMAR = (uint32_t)svb.capture;
}
void UART_Clear( void )
{
	uint8_t ascii_clear = 0x0c;
	HAL_UART_Transmit( this_uart, &ascii_clear, 1, UART_TIMEOUT );
}
void print( const uint8_t * Buf )
{
	HAL_UART_Transmit( this_uart, (uint8_t *)Buf, strlen((const char *)Buf), UART_TIMEOUT );
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	svf.uart_dma_busy = 0;
}

/***************************************************************************************/
/*                                  Initializers                                       */
/***************************************************************************************/
void zero_memory( void )
{
	memset(svb.thresh,  	0, sizeof(index_t)      * THRESH_BUFFER_SIZE  );
	memset(svb.density_x, 0, sizeof(density_t)  	* CAPTURE_HEIGHT      );
	memset(svb.density_y, 0, sizeof(density_t) 		* CAPTURE_WIDTH      	);
	memset(svb.quadrant,	0, sizeof(density_t) 		* 4                   );
	svd.quadrant_previous = 0;
}

void init_memory( void )
{
	svf.uart_dma_busy = 0;

	svf.frame = 0;
	svf.row = 0;
	zero_memory();

	svd.camera_port 		= (address_t)&(GPIOA->IDR);
	svd.uart_tx_address = (address_t)&(USART1->TDR);
#ifdef DYNAMIC_BUFFER
	svd.capture_end = (address_t)svb.capture+CAPTURE_WIDTH;
#else
	svd.capture_end = (address_t)svb.capture;
#endif
	svd.capture_max = (address_t)svb.capture + THRESH_BUFFER_SIZE;
	svd.thresh_max = (address_t)svb.thresh + sizeof(index_t)*(THRESH_BUFFER_SIZE-CAPTURE_HEIGHT);
	svd.thresh_end = (address_t)svb.thresh;
	svd.CENTROID_X = 3;
	svd.CENTROID_Y = 3;
	svd.thresh_value = DEFAULT_THRESH;

	/* Initialize Rho Type */
	rho_system.variables.rho.density_map_pair.x.length   = CAPTURE_HEIGHT;
	rho_system.variables.rho.density_map_pair.y.length   = CAPTURE_WIDTH;
	rho_system.variables.rho.density_map_pair.x.max[0]   = 0;
	rho_system.variables.rho.density_map_pair.x.max[1]   = 0;
	rho_system.variables.rho.density_map_pair.y.max[0]   = 0;
	rho_system.variables.rho.density_map_pair.y.max[1]   = 0;

	rho_kalman_t
			* kpxl = &rho_system.variables.rho.density_map_pair.x.kalmans[0],
			* kpxh = &rho_system.variables.rho.density_map_pair.x.kalmans[1],
			* kpyl = &rho_system.variables.rho.density_map_pair.y.kalmans[0],
			* kpyh = &rho_system.variables.rho.density_map_pair.y.kalmans[1];
	RhoKalman.init(kpxl, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(kpxh, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(kpyl, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(kpyh, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);

	rho_system.variables.rho.density_map_pair.x.map = svb.density_x;
	rho_system.variables.rho.density_map_pair.y.map = svb.density_y;
}

/***************************************************************************************/
/*                                 Frame Functions                                     */
/***************************************************************************************/
void spoofFrameProcessor( void )
{
	while(svf.uart_dma_busy);
	zero_memory();
	spoofPixels();
	frame_start();

	uint16_t x = 0, y = 0, p = 0, t = 0;
	for( ; y < CAPTURE_HEIGHT; y++ )
	{
		svb.thresh[t++] = 0xaaaa;
		for( x = 0; x < CAPTURE_WIDTH; x++, p++ )
		{
			if( svb.capture[p] > svd.thresh_value )
				svb.thresh[t++] = x;
		}
	}
	thresh_buffer_size = t - CAPTURE_HEIGHT + 1;
	rho_proc( thresh_buffer_size << 1 );
}

void frameTx( void )
{
	while(!svf.frame);
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_DEL, 2, UART_TIMEOUT );
	while(svf.frame)
	{
		while(!svf.row);
		HAL_UART_Transmit_DMA( this_uart, (uint8_t *)svb.capture, CAPTURE_WIDTH );
		while(svf.row && svf.frame);
	}
	tog();
	HAL_Delay(2);
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_END, 2, UART_TIMEOUT );
}

void spoofFrameTx( void )
{
	spoofPixels();
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_DEL, 2, UART_TIMEOUT );
	uint32_t x = 0, y;
	for( y = 0; y < CAPTURE_BUFFER_HEIGHT; y++ )
	{
		dprint( ((uint8_t *)svb.capture) + x, CAPTURE_BUFFER_WIDTH );
		x += CAPTURE_BUFFER_WIDTH;
		tog();
		while(svf.uart_dma_busy);
	}
	HAL_Delay(1);
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_END, 2, UART_TIMEOUT );
}

/***************************************************************************************/
/*                              Spoof Generators                                       */
/***************************************************************************************/
void spoofPixels( void )
{
	capture_t c = svd.thresh_value;
	index_t p, y, x;

	for( y = 0, p = 0; y < CAPTURE_BUFFER_HEIGHT; y++ )
	{
		for( x = 0; x < CAPTURE_BUFFER_WIDTH; x++, p++ )
		{
			svb.capture[p] 	= c;
		}
	}
	if( svd.thresh_value < 245 ) svd.thresh_value+=10;
	else svd.thresh_value = 10;
}

void spoofDensityMaps( void )
{
	density_t S = 100, B = 10;
	index_t a,b;
	a = CAPTURE_WIDTH / 4; b = 3 * a;
	for(index_t x = 0; x < CAPTURE_WIDTH;  x++ )
		svb.density_y[x] = B;
	svb.density_y[a] 		= S;
	svb.density_y[a+1] 	= S;
	svb.density_y[b] 		= S;
	svb.density_y[b+1] 	= S;


	a = CAPTURE_HEIGHT / 4; b = 3 * a;
	for(index_t y = 0; y < CAPTURE_HEIGHT; y++ )
		svb.density_x[y] = B;
	svb.density_x[a] 		= S;
	svb.density_x[a+1] 	= S;
	svb.density_x[b] 		= S;
	svb.density_x[b+1] 	= S;

	for( uint8_t i = 0; i < 4; i++ )
		svb.quadrant[i] = S*2;
	svd.quadrant_total = S*8;
}

void dprintBuffers( void )
{
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_DEL, 2, UART_TIMEOUT );
	dprint( (uint8_t *)svb.density_y, sizeof(density_t)*(CAPTURE_BUFFER_WIDTH + CAPTURE_BUFFER_HEIGHT ) );
}

void dprintCapture( void )
{
	HAL_UART_Transmit( this_uart, (uint8_t *)&BUFFER_TX_DEL, 2, UART_TIMEOUT );
	dprint( (uint8_t *)svb.capture, sizeof(density_t)*CAPTURE_BUFFER_SIZE );
}

void dprint( uint8_t * scrAddr, uint16_t len )
{
	while(svf.uart_dma_busy);
	svf.uart_dma_busy = 1;
	while(HAL_UART_Transmit_DMA( this_uart, scrAddr, len ) != HAL_OK);// Error_Handler();
}

/***************************************************************************************/
/*                                    Printers                                         */
/***************************************************************************************/
void printBuffers( uint32_t s )
{
    print( "Printing Thresh Buffer\r\n" );
    printBuffer( svb.thresh, thresh_buffer_size );

    print( "Printing Dx\r\n" );
    drawDensityMap(svb.density_x, CAPTURE_BUFFER_HEIGHT);

    print( "Printing Dy\r\n" );
    drawDensityMap(svb.density_y, CAPTURE_BUFFER_WIDTH);
}

void printAddress( const char * s, uint32_t addr )
{
    sprintf((char *)hex, "%s: %8x\r\n", s, addr);
    print( hex );
}

void printAddresses( void )
{
    printAddress("CamPt", (uint32_t)svd.camera_port);
    printAddress("C bfr", (uint32_t)svb.capture);
    printAddress("C end", (uint32_t)svd.capture_end);
    printAddress("C max", (uint32_t)svd.capture_max);
    printAddress("T bfr", (uint32_t)svb.thresh);
    printAddress("T end", (uint32_t)svd.thresh_end);
    printAddress("T max", (uint32_t)svd.thresh_max);
    printAddress("   Dx", (uint32_t)svb.density_x);
    printAddress("   Dy", (uint32_t)svb.density_y);
    printAddress("    Q", (uint32_t)svb.quadrant);
    printAddress("Q tot", (uint32_t)&svd.quadrant_total);
    printAddress("Q prv", (uint32_t)&svd.quadrant_previous);
    printAddress("   Cx", (uint32_t)&svd.CENTROID_X);
    printAddress("   Cy", (uint32_t)&svd.CENTROID_Y);
    printAddress("   Xm", (uint32_t)&svd.density_x_max);
    printAddress("T val", (uint32_t)&svd.thresh_value);
    printAddress("M end", (uint32_t)&svd.rho_mem_end);
}

void printCapture( void )
{
	for(int y = 0; y < CAPTURE_BUFFER_HEIGHT; y++ )
	{
		for(int x = 0; x < CAPTURE_BUFFER_WIDTH; x++ )
		{
			uint32_t p = x + y * CAPTURE_BUFFER_WIDTH;
			sprintf((char *)hex, " 0x%2x", svb.capture[p]);
			print( hex );
		}
		print( "\r\n" );
	}
}

void printBuffer( index_t * a, int l )
{
    for( int i = 0; i < l; i++ )
    {
        index_t curr = a[i];
        if( curr == 0xaaaa )
            sprintf((char *)hex, "\r\n(%d):", i);
        else
            sprintf((char *)hex, " 0x%x", curr);
        print( hex );
    }
    print("\r\n");
}

void drawDensityMap( density_t * a, int l )
{
    for( int i = 0; i < l; i++ )
    {
        density_t curr = a[i];
        sprintf((char *)hex, "%4d[%2d]", curr, i);
        print( hex );
        if( curr > 10 ) curr = 10;
        for( ; curr > 0; curr--) print( " " );
        print( "|\r\n" );
    }
}

void printPredictionPair( prediction_pair_t * pr )
{
	int xp = (int)pr->x.primary.value;
	int yp = (int)pr->y.primary.value;
	int xs = (int)pr->x.secondary.value;
	int ys = (int)pr->y.secondary.value;
	print("Printing predictions ");
	sprintf((char *)hex, "\t\t\tP(%d, %d) | S(%d, %d)\r\n", xp, yp, xs, ys);
	print( hex );
}
