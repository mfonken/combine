/*
 * camera_sp.c
 *
 *  Created on: Jan 8, 2017
 *      Author: Matthew Fonken
 */

#include "cam_controller.h"

#define	diff( a, b ) ( a - b > 0 ) ? a - b : b - a

beacon_t 	beacons[MAX_CENTROIDS];
uint8_t  	num_tracked;
uint8_t	 	map[MAX_CENTROIDS];
buffer_t 	camera_buffer;
centroids_t	centroids;

uint32_t	beacon_vector[2];

uint8_t		centroids_to_buffer;

/****************************************************************//**
 * Camera Communication Functions
 *******************************************************************/

void Print_Beacons( void )
{
	Print_Line("Beacons:");
	for( int i = 0; i < num_tracked; i++ )
	{
		Print_String("\tm");
		Print_Int( beacons[map[i]].centroid.m );
		Print_String("(");
		Print_Int( beacons[map[i]].centroid.x );
		Print_Char(',');
		Print_Int( beacons[map[i]].centroid.y );
		Print_String(") - ");
		Print_Int( beacons[map[i]].persistence );
		Print_Char('|');
		Print_Int( beacons[map[i]].timestamp );
		Print_Line("");
	}
	Print_Line("");
}

void Camera_Init( void )
{
    /* TODO: Needs to be HW enabled and probably init every use */
    Print_Char( CAMERA_INIT );
    uint8_t n = Read_Char();
    num_tracked = 0;
    centroids_to_buffer = 0;
    bufferReset( &camera_buffer );
}

void Camera_Enable( void )
{
    SYSCTL_Enable_Camera();
    NVIC_EnableIRQ(USART0_RX_IRQn);
    //RF_Session_Init( DEFAULT_BEACON_INTENSITY, DEFAULT_BEACON_DURATION );
}

void Camera_Disable( void )
{
    SYSCTL_Disable_Camera();
    NVIC_DisableIRQ(USART0_RX_IRQn);
    //RF_Session_End();
}

uint8_t Camera_Buffer( uint8_t in )
{
	if( in == CENTROID_HEAD ) camera_buffer.index = 0;
	uint8_t r = bufferAdd( &camera_buffer, in );
	return r;
}

uint8_t Camera_Check( void )
{
	disableUARTInterrupt();
    uint8_t index, end;
	end = camera_buffer.index;
	index = 0;
	while( index != end )
	{
		if( bufferRead( &camera_buffer, index ) == CENTROID_HEAD )
		{
			uint16_t n = bufferRead( &camera_buffer, index + 1 );
            if( n < 2 || n > 16 ) goto invalid;
			centroids.count = n;
			if( ( end - index ) < ( ( n * 6 ) + 2 ) )
			{
				centroids.count = 0;
				enableUARTInterrupt();
				return CAM_NULL_CMD;
			}
			uint8_t read_index;
			for( uint8_t i = 0 ; i < n; i++ )
			{
				read_index = index + ( i * 6 ) + 2;

				uint8_t topX = bufferRead( &camera_buffer, read_index );
				uint8_t btmX = bufferRead( &camera_buffer, read_index + 1 );
				uint8_t topY = bufferRead( &camera_buffer, read_index + 2 );
				uint8_t btmY = bufferRead( &camera_buffer, read_index + 3 );
				uint8_t topM = bufferRead( &camera_buffer, read_index + 4 );
				uint8_t btmM = bufferRead( &camera_buffer, read_index + 5 );

				centroids.centroid[i].x = ( topX << 8 ) + btmX;
				centroids.centroid[i].y = ( topY << 8 ) + btmY;
				centroids.centroid[i].m = ( topM << 8 ) + btmM;
			}
			Beacon_Check();
            bufferReset( &camera_buffer );
			enableUARTInterrupt();
			return centroids.count;
		}
        index++;
	}
invalid:
	enableUARTInterrupt();
	return CAM_NULL_CMD;
}

/********************************************************************
 * Beacon Tracking Functions
 *******************************************************************/

void Beacon_Add( centroid_t * a )
{
	Beacon_Copy( &beacons[num_tracked].centroid, a );
	beacons[num_tracked].persistence = 1;
	beacons[num_tracked].timestamp = timestamp();
	map[num_tracked] = num_tracked;
	if( num_tracked < MAX_CENTROIDS ) num_tracked++;
}

void Beacon_Check( void )
{
	for( int i = 0; i < centroids.count; i++ )
	{
		bool claimed = false;
		if( centroids.centroid[i].m >= MIN_CENTROID_MASS )
		{
			for( int j = 0; j < num_tracked; j++ )
			{
				if( Beacon_Compare( &beacons[map[j]].centroid, &centroids.centroid[i] ) )
				{
					claimed = true;
					Beacon_Update( j, &centroids.centroid[i]);
					j++;
				}
			}
			if( !claimed )
			{
				Beacon_Add( &centroids.centroid[i] );
			}
		}
	}
}

bool Beacon_Compare( centroid_t * a, centroid_t * b ){
	/*TODO: Complete this compare check */
	uint8_t c = diff( a->x, b->x ) ;
	uint8_t d = diff( a->y, b->y ) ;
	//uint8_t e = diff( a->m, b->m ) ;
	if( c <= MAX_X_DIFF &&
		d <= MAX_Y_DIFF )// &&
	//	e <= MAX_M_DIFF  )
	{
		return true;
	}
	return false;
}

/* Call this on sync */
bool Beacon_Compose( cartesian2_t vis[2] )
{
	if( centroids.count >= 2 )
	{
		vis[0].x = beacons[map[0]].centroid.x;
		vis[0].y = beacons[map[0]].centroid.y;
		vis[1].x = beacons[map[1]].centroid.x;
		vis[1].y = beacons[map[1]].centroid.y;
		centroids.count = 0;
		return true;
	}
	return false;
}

void Beacon_Copy( centroid_t * a, centroid_t * b)
{
	a->x = b->x;
	a->y = b->y;
	a->m = b->m;
}

void Beacon_Sort( void )
{
	/* Sudo resort by persistence */
    for( int i = 0; i < ( num_tracked - 1 ); i++ )
    {
        if( beacons[map[i]].persistence < beacons[map[i+1]].persistence )
        {
            /* Swap index in map */
            uint8_t temp = map[i];
            map[i] = map[i+1];
            map[i+1] = temp;
        }
    }
}

void Beacon_Purge( void )
{
	/* Purge old beacons */
	uint8_t purged = 0;
	for( int i = 0; i < num_tracked; i++ )
	{
		uint32_t t = timestamp();
		uint32_t diff = t - beacons[map[i]].timestamp;
		if( diff > MAX_TRACK_AGE )
		{
			map[i] = map[i + 1 + purged];
			purged++;
//			Print_String("Purged - ");
//			Print_Int(i);
//			Print_Line("");
		}
	}
	num_tracked -= purged;

	/* Purge duplicates */
	for( int i = 0; i < num_tracked-1; i++ )
	{
		for( int j = i+1; j < num_tracked; j++ )
		{
			if( Beacon_Compare( &beacons[map[i]].centroid, &beacons[map[j]].centroid ) )
			{
				for( int k = j; k < num_tracked-1; k++ )
				{
					map[k] = map[k + 1];
				}
				num_tracked--;
			}
		}
	}
}

void Beacon_Update( uint8_t index, centroid_t * b )
{
	Beacon_Copy( &beacons[map[index]].centroid, b );
	beacons[map[index]].timestamp = timestamp();
	beacons[map[index]].persistence++;

	Beacon_Purge();
	Beacon_Sort();
}
