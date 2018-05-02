//
//  imu_wrapper.c
//  IMU Math
//
//  Created by Matthew Fonken on 12/19/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "imu_wrapper.h"

#define DEL  sizeof(char)
#define SEP  sizeof(char)
#define FLSZ (1+1+3)
#define BUFFSIZE(X) (int)(DEL+(int)X*(SEP+FLSZ))

#define MIN_PACKET_LEN_RAW BUFFSIZE(9)
#define MIN_PACKET_LEN_ORI BUFFSIZE(6)

#define BUFFER_LENGTH   (1 << 7)
#define MAX_TOKENS      10

#define PACKET_DEL_START ' '
#define PACKET_DEL_END '\n'

//double mag_cal[] = { -2.396, 38.040, 1.093 };

static int init( imu_t * imu )
{
    switch(imu->channel.interface)
    {
        default:
        case SERCOM:
            imu->channel.descriptor = Init_SERCOM_Default();
            Write_SERCOM_Byte(imu->channel.descriptor, INIT_TRIGGER_VAL);
            return (int)SERCOM;
            break;
    }
    
    imu->state.status.calibrated = 0;
    imu->state.status.active = 0;
    imu->state.action = CALIBRATING;
    imu->state.reference.x = 0.;
    imu->state.reference.y = 0.;
    imu->state.reference.z = 0.;
    return -1;
}

const imu IMU =
{
    .init = init,
    .update = IMU_Update
};

static int correctOrientationPacket( double tokens[7], double values[6] )
{
    int n = tokens[1];
    
    if( n >= 3 )
    {
        for( int i = 2; i <= 4; i++ )
        {
            if( !tokens[i] ) return 0;
        }
        values[0] = tokens[4] * DEG_TO_RAD;
        values[1] = tokens[3] * DEG_TO_RAD;
        values[2] = -( tokens[2] * DEG_TO_RAD );
    }
    if( n >= 6 )
    {
        values[3] = tokens[5];
        values[4] = tokens[6];
        values[5] = tokens[7];
    }
    return n;
}

static int correctOffsetPacket( double tokens[7], double values[6] )
{
    return correctOrientationPacket( tokens, values );
}

void IMU_Update(imu_t * imu)
{
    IMU_Request( imu, 'o' );
    char line[BUFFER_LENGTH];
    int len = Read_SERCOM_IMU_Packet( imu, line, 100 );
    double tokens[MAX_TOKENS] = {0}, v[6] = {0};
    packet_id_t packet_id = (packet_id_t)tokenifyPacket( line, len, 6, tokens, (char)NULL_PACKET_ID );

    char m = 0;
    int n = 0;
    switch( packet_id )
    {
        case NULL_PACKET_ID:
            return;
        case MESSAGE_ID: // Message
            m = (state_action_t)tokens[MESSAGE_TOKEN_INDEX];
            if( m < 0 || m >= NUM_ACTIONS ) m = 0;
            imu->state.action = m;
            switch(m)
            {
                case CALIBRATING:
                    imu->state.status.sc = (char)tokens[SYSTEM_CAL_TOKEN_INDEX];
                    imu->state.status.ac = (char)tokens[ACCEL_CAL_TOKEN_INDEX];
                    imu->state.status.gc = (char)tokens[GYRO_CAL_TOKEN_INDEX];
                    imu->state.status.mc = (char)tokens[MAG_CAL_TOKEN_INDEX];
                    imu->state.status.calibrated = 0b0;
                    imu->state.status.active = 0b0;
                    break;
                case ACTIVATING:
                    imu->state.status.sc = FULL_CALIBRATION;
                    imu->state.status.ac = FULL_CALIBRATION;
                    imu->state.status.gc = FULL_CALIBRATION;
                    imu->state.status.mc = FULL_CALIBRATION;
                    imu->state.status.calibrated = 0b1;
                    imu->state.status.active = 0b0;
                    break;
                case RUNNING:
                    imu->state.status.active = 0b1;
                    break;
                default:
                    break;
            }
#ifdef IMU_DEBUG
            printf("Receiving message packet: act:%s sc:%d ac:%d gc:%d mc:%d\n", state_action_strings[imu->state.action], (int)imu->state.status.sc, (int)imu->state.status.ac, (int)imu->state.status.gc, (int)imu->state.status.mc);
#endif
            break;
        case ORIENTATION_ID: // Orientation
            imu->state.action = RUNNING;
            n = correctOrientationPacket( tokens, v );
            if( n >= 3 )
            {
                imu->pitch          = v[1];
                imu->roll           = v[0];
                imu->yaw            = v[2];
            }
            if( n >= 6 )
            {
                imu->accel_raw[0]   = v[3];
                imu->accel_raw[1]   = v[4];
                imu->accel_raw[2]   = v[5];
            }
#ifdef IMU_DEBUG
            printf("Receiving orientation packet: %f %f %f\n", v[0], v[1], v[2]);
#endif
            break;
        case OFFSET_ID:
            imu->state.action = UPDATING;
            n = correctOffsetPacket( tokens, v );
            if( n >= 3 )
            {
                imu->state.reference.x = v[0];
                imu->state.reference.y = v[1];
                imu->state.reference.z = v[2];
            }
            break;
        default:
            break;
    }
}

static inline long now( void )
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_usec + tv.tv_sec*1000000);
}

static bool isTimedout( long stamp, long timeout )
{
    return ( ( now() - stamp ) > timeout );
}

int Read_SERCOM_IMU_Packet( imu_t * imu, char * out, long timeout )
{
    
    long stamp = now();

    char buffer[BUFFER_LENGTH];
    int bytes_read = -1, ptr = 0, isnl = 0, started = 0;
    while( !isnl && ptr < BUFFER_LENGTH-1 )
    {
        if( isTimedout(stamp, timeout) ) return 0;
        /* Wait for new bytes */
        bytes_read = 0;
        while(bytes_read <= 0 )
        {
            bytes_read = Read_SERCOM_Bytes(imu->channel.descriptor, buffer, (size_t)BUFFER_LENGTH);
            if( isTimedout(stamp, timeout) ) return 0;
        }
        Read_SERCOM_Bytes(imu->channel.descriptor, buffer, (size_t)BUFFER_LENGTH);
        
        /* Process new bytes */
        int i = ptr;
        for(int j = 0; !isnl && i < ptr + bytes_read && j < BUFFER_LENGTH; j++)
        {
            char c = buffer[j];
            switch( c )
            {
                case PACKET_DEL_START:
                    started = 1;
                    break;
                case PACKET_DEL_END:
                    if(started)
                        isnl = 1;
                    break;
                case '\\':
                case '\0':
                    break;
                default:
                    if(started)
                        out[i++] = buffer[j];
                    break;
            }
        }
        ptr += bytes_read;
    }
    out[ptr] = ',';
    out[++ptr] = '\0';
    if(ptr < MIN_PACKET_LEN_ORI) return 0;
    
//    printf("\nR(%d): %s\n", ptr, out);
    
    return ptr;
}

void IMU_Request( imu_t * imu, char c )
{
    Write_SERCOM_Byte(imu->channel.descriptor, c);
}
