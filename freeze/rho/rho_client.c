//
//  rho_client.c
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_client.h"

static inline void PixelThreshLoop( address_t pt_index,
                                      const register byte_t * pixel,
                                      byte_t * thresh_address,
                                      register index_t * thresh_buffer )
{
    register index_t t_index = *(index_t *)pt_index;
    register index_t x_index = RhoSystem.Variables.Utility.height;
    register byte_t thresh_value = *thresh_address;
    while( ( x_index > 0 ) && !RhoSystem.Variables.Flag.Row )
        if( *pixel > thresh_value )
            thresh_buffer[--t_index] = --x_index;
    *(index_t *)pt_index = t_index;
}

static inline index_t CaptureFrame()
{
    while(!RhoSystem.Variables.Flag.Row);
    
    index_t
        y_counter = (index_t)RhoSystem.Variables.Utility.height,
        t_counter = (index_t)((uint32_t)RhoSystem.Variables.Address.ThreshMax - (uint32_t)RhoSystem.Variables.Buffer.Thresh);
    while( ( y_counter > 0 ) && ( t_counter > 0 ) )
    {
        RhoSystem.Variables.Buffer.Thresh[--t_counter] = Y_DEL;
        while(RhoSystem.Variables.Flag.Row);

        PixelThreshLoop( &t_counter,
                         RhoSystem.Variables.Address.CameraPort,
                         RhoSystem.Variables.Utility.thresh,
                         RhoSystem.Variables.Buffer.Thresh );
        
        while(!RhoSystem.Variables.Flag.Row);
        --y_counter;
    }
    return y_counter;
}

static inline section_process_t ProcessFrameSection( register index_t t_counter, register index_t t_len, register index_t y_counter, register index_t y_len )
{
    register index_t t_value = 0;
    register density_2d_t Q_left = 0, Q_right = 0, Q_total = 0, Q_prev = 0;
    while( t_counter < t_len )
    {
        t_value = RhoSystem.Variables.Buffer.Thresh[t_counter++];
        if(t_value == Y_DEL)
        {
            Q_total = Q_left + Q_right;
            RhoSystem.Variables.Utility.density_map_pair.x.map[y_counter] = Q_total - Q_prev;
            Q_prev = Q_total;
            if( ++y_counter > y_len ) break;
        }
        else if( t_value < RhoSystem.Variables.Utility.Cx )
        {
            Q_left++;
            RhoSystem.Variables.Utility.density_map_pair.y.map[t_value]++;
        }
        else
        {
            Q_right++;
            RhoSystem.Variables.Utility.density_map_pair.y.map[t_value]++;
        }
    }
    return (section_process_t){ t_counter, y_counter, Q_left, Q_right };
}

static inline void ProcessFrame( index_t t_len )
{
    section_process_t top = ProcessFrameSection(          0, t_len,        0, RhoSystem.Variables.Utility.Cy     );
    section_process_t btm = ProcessFrameSection( top.pixels, t_len, top.rows, RhoSystem.Variables.Utility.height );
    
    RhoSystem.Variables.Utility.Q[0] = top.left;
    RhoSystem.Variables.Utility.Q[1] = top.right;
    RhoSystem.Variables.Utility.Q[2] = btm.left;
    RhoSystem.Variables.Utility.Q[3] = btm.right;
}

static inline void ActivateBackgrounding( void )
{
    RhoSystem.Variables.Buffer.DensityX = RhoSystem.Variables.Utility.density_map_pair.x.background;
    RhoSystem.Variables.Buffer.DensityY = RhoSystem.Variables.Utility.density_map_pair.y.background;
    RhoSystem.Variables.Buffer.Quadrant = RhoSystem.Variables.Utility.Qb;
}

static inline void DeactivateBackgrounding( void )
{
    RhoSystem.Variables.Buffer.DensityX = RhoSystem.Variables.Utility.density_map_pair.x.map;
    RhoSystem.Variables.Buffer.DensityY = RhoSystem.Variables.Utility.density_map_pair.y.map;
    RhoSystem.Variables.Buffer.Quadrant = RhoSystem.Variables.Utility.Q;
}

void ProcessRhoSystemFrameCapture( void )
{
    RhoSystem.Variables.Flag.Backgrounding ? ActivateBackgrounding() : DeactivateBackgrounding();
    RhoSystem.Functions.Memory.Zero();
    while(!RhoSystem.Variables.Flag.Frame);
    
    ProcessFrame( CaptureFrame() );

    RhoSystem.Variables.Buffer.DensityX[CAPTURE_BUFFER_HEIGHT-1] = FRAME_ROW_END;
}

inline void PerformRhoSystemProcess( void )
{
    RhoFunctions.Perform( &RhoSystem.Variables.Utility, RhoSystem.Variables.Flag.Backgrounding);
}

/***************************************************************************************/
/*                                  Initializers                                       */
/***************************************************************************************/
void InitRhoSystem( void )
{
    RhoSystem.Variables.Buffer.DensityX = RhoSystem.Variables.Utility.density_map_pair.x.map;
    RhoSystem.Variables.Buffer.DensityY = RhoSystem.Variables.Utility.density_map_pair.y.map;
    RhoSystem.Variables.Buffer.Quadrant = RhoSystem.Variables.Utility.Q;
    
#ifdef DYNAMIC_BUFFER
    RhoSystem.Variables.Address.CaptureEnd  = (address_t)RhoSystem.Variables.Buffer.Capture + CAPTURE_WIDTH;
#else
    RhoSystem.Variables.Address.CaptureEnd  = (address_t)RhoSystem.Variables.Buffer.Capture;
#endif
    RhoSystem.Variables.Address.CaptureMax  = (address_t)RhoSystem.Variables.Buffer.Capture + THRESH_BUFFER_SIZE;
    RhoSystem.Variables.Address.ThreshMax   = (address_t)RhoSystem.Variables.Buffer.Thresh + sizeof(index_t)*(THRESH_BUFFER_SIZE-CAPTURE_HEIGHT);
    RhoSystem.Variables.Address.ThreshEnd   = (address_t)RhoSystem.Variables.Buffer.Thresh;

    RhoSystem.Functions.Memory.Zero();
    RhoFunctions.Init( &RhoSystem.Variables.Utility );
}

void ZeroRhoSystemMemory( void )
{
    memset( RhoSystem.Variables.Buffer.Thresh,   0, sizeof(index_t)   * THRESH_BUFFER_SIZE );
    memset( RhoSystem.Variables.Buffer.Quadrant, 0, sizeof(density_t) * 4                  );
}

