/***********************************************************************************************//**
 * \file   app.c
 * \brief  Main application.
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "app.h"

/* App variables */
app_t 			mode;
kinetic_t		kinetics;
sensor_data_t 	sensors;

/* Interrupt variables */
uint8_t buffer_rx;
uint8_t cam_index;

/***********************************************************************************************//**
 * Interrupt Handlers
 **************************************************************************************************/

/* Camera USART */
//void USART0_RX_IRQHandler( void );
void CameraHandler( void )
{
	uint8_t in = USART_Rx( CAM_UART );
	uint8_t r = Camera_Buffer( in );
}

/* Sensor sync timer */
//void TIMER0_IRQHandler(void)
void SyncTimerHandler( void )
{
	disableTimer( SYNC_TIMER );
//	Kinetic_Update_Rotation( &sensors.synced.imu, &kinetics );
//	app();
//	Print_Char('.');

	uint8_t r = Camera_Check();

	Print_Beacons();
	if ( r != CAM_NULL_CMD )
	{
//		Beacon_Compose( sensors.synced.beacons );
//		Kinetic_Update_Rotation( &sensors.synced.imu, &kinetics );
//		Kinetic_Update_Position( &sensors.synced.imu, &kinetics, sensors.synced.beacons );

//		app();
	}
	enableTimer( SYNC_TIMER );
}

/* Force sensor Timer */
//void TIMER1_IRQHandler(void)
void ForceTimerHandler( void )
{
	Print_Line( "\tTimer 1." );
}

/* Beacon RF timer */
//void CRYOTIMER_IRQHandler(void)
void BeaconTimerHandler( void )
{
	CRYOTIMER_IntClear( TIMER_IF_OF );      	// Clear overflow flag

	/* TODO: Add beacon intensity check - RF_Step(up/down)*/
	RF_Kick();
	Print_Line( "\tCryotimer 0." );
}

/* SYSCTL Interrupt Handler */
//void GPIO_ODD_IRQHandler(void)
void SYSCTLInterruptHandler( void )
{
	exitSleepMode();
}

/***********************************************************************************************//**
 * Application
 **************************************************************************************************/

/* App Initialize */
void app_init( void )
{
	USART0->IEN = USART_IEN_RXDATAV;
    registerCallback( &CameraHandler, USART0_RX_IRQn );

    IMU_Init( &sensors.synced.imu );
    Print_Line( "IMU Initialized." );
    Kinetic_Init( &sensors.synced.imu, &kinetics );
	Print_Line( "Kinetic Initialized." );

	registerTimer( &SyncTimerHandler, SYNC_TIMER, SYNC_TIMER_PERIOD );
//	registerTimer( &ForceTimerHandler, FORCE_TIMER, FORCE_TIMER_PERIOD );

	mode._2d = _2D_MODE_DEFAULT;
	mode._3d = _3D_MODE_DEFAULT;
	mode._sl = _SLEEP_MODE_DEFAULT;
	appModeSet( &mode );
}

void app( void )
{
//	Print_Beacons();
//	Print_String( "\tTick - " );
//	double t;
//	t = (double)timestamp();
//	t /= 1000;
//	Print_Double_Ascii( t );
//	Print_String( "s \r\n" );

//	Print_Char('f');
//	Print_Char(',');
//	Print_Double_Ascii( kinetics.rotation[0] * 57.295779 );
//	Print_Char(',');
//	Print_Double_Ascii( kinetics.rotation[1] * 57.295779 );
//	Print_Char(',');
//	Print_Double_Ascii( kinetics.rotation[2] * 57.295779 );
//	Print_Line("");
	Print_String("Coordinates - (");
	Print_Double_Ascii( kinetics.truePositionFilter[0].value );
	Print_Char(',');
	Print_Double_Ascii( kinetics.truePositionFilter[1].value );
	Print_Char(',');
	Print_Double_Ascii( kinetics.truePositionFilter[2].value );
	Print_Line(").");
}

void appModeSet( app_t * m )
{
	app_t mode = *m;
	if( mode._sl )
	{
		enterSleepMode();
		return;
	}
	if( mode._2d )	enableStylusSensors();
	else			disableStylusSensors();
	if( mode._3d ) 	enableSpatialSensors();
	else			disableSpatialSensors();
	sync_t sync;
    sync.accel 	= 1;//mode._2d; // Enabled for both 2D and 3D mode
    sync.gyro  	= 1;//mode._2d;
	sync.mag	= mode._3d;
	sync.cam	= mode._3d;
	enableSyncTimer( &sync );
}

void autoDetectMode( void )
{
	/*TODO: Scan camera and force sensor to determine appropriate mode.
	 * 		Camera has two or more beacons.
	 * 		Force sensor has force.
	 */
	mode._2d = _2D_MODE_DEFAULT;
	mode._3d = _3D_MODE_DEFAULT;
	mode._sl = _SLEEP_MODE_DEFAULT;
}

void enableSyncTimer( sync_t * s )
{
	sensorSyncSet( s );
	enableTimer( SYNC_TIMER );
}
void disableSyncTimer( void )
{
	disableTimer( SYNC_TIMER );
}

void enableStylusSensors( void )
{
	SYSCTL_Enable_Force_Sensor();
	enableTimer( FORCE_TIMER );
	/* TODO: Enable IMU for updated board */
}

void disableStylusSensors( void )
{
	SYSCTL_Disable_Force_Sensor();
	disableTimer( FORCE_TIMER );
	/* TODO: Disable IMU for updated board */
}

void enableSpatialSensors( void )
{
	Camera_Enable();
	SYSCTL_Enable_Magnometer();
	/* TODO: Enable full IMU for updated board */
}

void disableSpatialSensors( void )
{
	Camera_Disable();
	SYSCTL_Disable_Magnometer();
	/* TODO: Disable full IMU for updated board */
}

void enterSleepMode( void )
{
	disableTimer( SYNC_TIMER );
	disableTimer( FORCE_TIMER );
	disableStylusSensors();
	disableSpatialSensors();

	enableGPIOInterrupt();
	EMU_EnterEM3( true );
}

void exitSleepMode( void )
{
	EMU_EnterEM1();
	disableGPIOInterrupt();

	autoDetectMode();

	enableTimer( SYNC_TIMER );
	enableTimer( FORCE_TIMER );
}
