//
//  rho_client.c
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_client.h"

static inline void PixelThreshLoop( const register byte_t * capture_address,// capture buffer index
                                    address_t pthresh_index,                // pointer to thresh buffer index
                                    byte_t * thresh_address,                // address of thresh buffer
                                    register index_t * thresh_buffer )
{
    register index_t thresh_index = *(index_t *)pthresh_index;
    register index_t capture_index = 0;
    register byte_t thresh_value = *thresh_address;
    while( ( capture_index++ < RhoSystem.Variables.Utility.Width ) && !RhoSystem.Variables.Flags.Row )
        if( capture_address[capture_index] > thresh_value )
            thresh_buffer[thresh_index++] = capture_index;
    *(index_t *)pthresh_index = thresh_index;
}

static inline index_t CaptureFrame()
{
    while(RhoSystem.Variables.Flags.Row);
    index_t
        y_counter = (index_t)RhoSystem.Variables.Utility.Height,
        t_counter = 0,
        t_max = (index_t)((uint32_t)RhoSystem.Variables.Addresses.ThreshMax - (uint32_t)RhoSystem.Variables.Buffers.Thresh);
    while( ( --y_counter > 0 ) && ( t_counter < t_max ) )
    {
        RhoSystem.Variables.Buffers.Thresh[t_counter++] = Y_DEL;
        while(RhoSystem.Variables.Flags.Row);
        PixelThreshLoop( RhoSystem.Variables.Buffers.Capture,
                        &t_counter,
                         RhoSystem.Variables.Utility.Thresh,
                         RhoSystem.Variables.Buffers.Thresh );
        while(!RhoSystem.Variables.Flags.Row);
    }
    return t_counter;
}

static inline section_process_t ProcessFrameSection( register index_t t_counter, register index_t t_len, register index_t y_counter, register index_t y_len )
{
    register index_t t_value = 0;
    register density_2d_t Q_left = 0, Q_right = 0, Q_total = 0, Q_prev = 0;
    while( t_counter < t_len )
    {
        t_value = RhoSystem.Variables.Buffers.Thresh[t_counter++];
        if(t_value == Y_DEL)
        {
            Q_total = Q_left + Q_right;
            RhoSystem.Variables.Utility.DensityMapPair.x.map[y_counter] = Q_total - Q_prev;
            Q_prev = Q_total;
            if( ++y_counter > y_len ) break;
        }
        else if( t_value < RhoSystem.Variables.Utility.Cx )
        {
            Q_left++;
            RhoSystem.Variables.Utility.DensityMapPair.y.map[t_value]++;
        }
        else
        {
            Q_right++;
            RhoSystem.Variables.Utility.DensityMapPair.y.map[t_value]++;
        }
    }
    return (section_process_t){ t_counter, y_counter, Q_left, Q_right };
}

static inline void ActivateBackgrounding( void )
{
    RhoSystem.Variables.Flags.Backgrounding = true;
    RhoSystem.Variables.Buffers.DensityX = RhoSystem.Variables.Utility.DensityMapPair.x.background;
    RhoSystem.Variables.Buffers.DensityY = RhoSystem.Variables.Utility.DensityMapPair.y.background;
    RhoSystem.Variables.Buffers.Quadrant = RhoSystem.Variables.Utility.Qb;
}

static inline void DeactivateBackgrounding( void )
{
    RhoSystem.Variables.Flags.Backgrounding = false;
    RhoSystem.Variables.Buffers.DensityX = RhoSystem.Variables.Utility.DensityMapPair.x.map;
    RhoSystem.Variables.Buffers.DensityY = RhoSystem.Variables.Utility.DensityMapPair.y.map;
    RhoSystem.Variables.Buffers.Quadrant = RhoSystem.Variables.Utility.Q;
}

static inline void ProcessFrame( index_t t_len )
{
    if( HasPixelCountDrop( RhoSystem.Variables.Buffers.PixelCount, t_len ) )
        ActivateBackgrounding();
    else DeactivateBackgrounding();
    
    section_process_t top = ProcessFrameSection(          0, t_len,        0, RhoSystem.Variables.Utility.Cy     );
    section_process_t btm = ProcessFrameSection( top.pixels, t_len, top.rows, RhoSystem.Variables.Utility.Height );
    
    RhoSystem.Variables.Buffers.Quadrant[0] = top.left;
    RhoSystem.Variables.Buffers.Quadrant[1] = top.right;
    RhoSystem.Variables.Buffers.Quadrant[2] = btm.left;
    RhoSystem.Variables.Buffers.Quadrant[3] = btm.right;
    
    RhoSystem.Variables.Buffers.DensityX[CAPTURE_BUFFER_HEIGHT-1] = FRAME_ROW_END;
}

void ProcessRhoSystemFrameCapture( void )
{
    RhoSystem.Functions.Memory.Zero();
    while(!RhoSystem.Variables.Flags.Frame);
    
    ProcessFrame( CaptureFrame() );
}

inline void PerformRhoSystemProcess( void )
{
    RhoSystem.Functions.Perform.FrameCapture();
    RhoFunctions.Perform( &RhoSystem.Variables.Utility, RhoSystem.Variables.Flags.Backgrounding);
    
    if( TransmitPacket( &RhoSystem.Variables.Utility.Packet ) )
        return;
}

void ActivateRhoSystem(void)
{
    RhoSystem.Variables.Flags.Active = true;
    TransmitPacket( RhoSystem.Variables.Buffers.BeaconPacket );
}

void DeactivateRhoSystem(void)
{
    RhoSystem.Variables.Flags.Active = false;
    // TODO: zero period
    TransmitPacket( RhoSystem.Variables.Buffers.BeaconPacket );
}

/***************************************************************************************/
/*                                  Initializers                                       */
/***************************************************************************************/
void InitRhoSystem( void )
{
    SetActiveClientFlags( &RhoSystem.Variables.Flags );
    DeactivateBackgrounding();
    
#ifdef DYNAMIC_BUFFER
    RhoSystem.Variables.Addresses.CaptureEnd  = (address_t)RhoSystem.Variables.Buffers.Capture + CAPTURE_WIDTH;
#else
    RhoSystem.Variables.Address.CaptureEnd  = (address_t)RhoSystem.Variables.Buffer.Capture;
#endif
    RhoSystem.Variables.Addresses.CaptureMax  = (address_t)RhoSystem.Variables.Buffers.Capture + THRESH_BUFFER_SIZE;
    RhoSystem.Variables.Addresses.ThreshMax   = (address_t)RhoSystem.Variables.Buffers.Thresh + sizeof(index_t)*(THRESH_BUFFER_SIZE-CAPTURE_HEIGHT);
    RhoSystem.Variables.Addresses.ThreshEnd   = (address_t)RhoSystem.Variables.Buffers.Thresh;

    RhoSystem.Variables.Buffers.BeaconPacket = malloc( sizeof( packet_t ) );
    RhoSystem.Variables.Buffers.BeaconPacket->header.ID = BEACON_PACKET_ID;
    RhoSystem.Variables.Buffers.BeaconPacket->header.includes = BEACON_DEFAULT_PERIOD;
    
    RhoSystem.Functions.Memory.Zero();
    RhoFunctions.Init( &RhoSystem.Variables.Utility );
}

void ZeroRhoSystemMemory( void )
{
    memset( RhoSystem.Variables.Buffers.Thresh,   0, sizeof(index_t)   * THRESH_BUFFER_SIZE );
    memset( RhoSystem.Variables.Buffers.Quadrant, 0, sizeof(density_t) * 4                  );
}

