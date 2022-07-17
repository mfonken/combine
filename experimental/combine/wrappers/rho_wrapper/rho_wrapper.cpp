//
//  rho_wrapper.cpp
//  tau+
//
//  Created by Matthew Fonken on 3/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_wrapper.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define DO_NOT_TIME_ACQUISITION

void RhoWrapper::Init( int width, int height )
{
    width = width;
    height = height;
    
    LOG_RW(RHO_DEBUG, "Initializing Rho Utility: %dx%d & [KTarg-%f, VarNorm-%.3f, VarSca-%.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    
//    pthread_mutex_init(&density_map_pair_mutex, NULL);
//    pthread_mutex_init(&c_mutex, NULL);

    cimageInit( cimage, width, height );
    RhoSystem.Variables.Buffers.Thresh = (index_t *)malloc(sizeof(index_t) * THRESH_BUFFER_SIZE);
    RhoSystem.Variables.Addresses.Thresh = RhoSystem.Variables.Buffers.Thresh;
    
    RhoCore.Initialize(&core, width, height);
    backgrounding_event = false;
    
    PrintSizes();
}

double RhoWrapper::Perform( cv::Mat & mat )
{    
    cimageFromMat( mat, cimage );
    return Perform( cimage );
}

double RhoWrapper::Perform( cimage_t & img )
{
//    pthread_mutex_lock(&c_mutex);
//    pthread_mutex_lock(&density_map_pair_mutex);
    
    /* Core Rho Functions */
    struct timeval a,b;
    gettimeofday( &a, NULL);
    Decouple( img, backgrounding_event );
#ifdef DO_NOT_TIME_ACQUISITION
    gettimeofday( &a, NULL);
#endif
    if(core.quadrant[0] + core.quadrant[1] + core.quadrant[2] + core.quadrant[3])
        RhoCore.Perform( &core, backgrounding_event );
    gettimeofday( &b, NULL);

    /* * * * * * * * * * */
    
//    pthread_mutex_unlock(&density_map_pair_mutex);
    memcpy((byte_t *)&packet, (byte_t*)&core.packet, sizeof(packet_t));
    backgrounding_event = false; // Generate background always and only once
//    pthread_mutex_unlock(&c_mutex);
    
    return timeDiff(a,b);
}

void RhoWrapper::Decouple( const cimage_t image, bool backgrounding )
{
//    if(backgrounding) ToggleBackgrounding(true);
    
    printf("Start: %p\n", RhoSystem.Variables.Addresses.Thresh);
    RhoSystem.Variables.Addresses.Capture = (byte_t *)image.pixels;
    for( index_t y = 0; y < image.height; y += RhoSystem.Variables.Utility.subsample )
    {
        RhoSystem.Variables.Addresses.Thresh = RhoCapture.CaptureRow(RhoSystem.Variables.Utility.subsample,
                              RhoSystem.Variables.Addresses.Capture,
                              RhoSystem.Variables.Utility.thresh_byte,
                              RhoSystem.Variables.Addresses.Thresh,
                              image.width );
        *(RhoSystem.Variables.Addresses.Thresh++) = CAPTURE_ROW_END;
//        RhoSystem.Variables.Flags->EvenRowToggle = !RhoSystem.Variables.Flags->EvenRowToggle;
    }
    printf("End %p\n", RhoSystem.Variables.Addresses.Thresh);
    
    section_process_t ProcessedSectionData[2];
    uint16_t rows = RhoSystem.Variables.Utility.centroid.y;
    for( byte_t i = 0; i < 2; i++ )
    {
        do{ ProcessedSectionData[i] = RhoCapture.ProcessFrameSection( rows,
                RhoSystem.Variables.Buffers.Thresh,
                RhoSystem.Variables.Addresses.Thresh,
                RhoSystem.Variables.Utility.centroid.x,
                RhoSystem.Variables.Utility.density_map_pair.y.map,
                RhoSystem.Variables.Utility.density_map_pair.x.map );
        } while( !ProcessedSectionData[i].complete );
        rows = RhoSystem.Variables.Utility.height;
    }
    
//    if(backgrounding) ToggleBackgrounding(false);
}

void RhoWrapper::ToggleBackgrounding(bool on)
{
//    if( on )
//    {
//        RhoSystem.Variables.Buffers.DensityX = core.density_map_pair.x.background;
//        RhoSystem.Variables.Buffers.DensityY = core.density_map_pair.y.background;
//        RhoVariables.ram.CX_ADDR = &core.secondary.y;
//        RhoVariables.ram.CY_ADDR = &core.secondary.x;
//        RhoSystem.Variables.Buffers.Quadrant = core.quadrant_background;
//    }
//    else
//    {
//        RhoSystem.Variables.Buffers.DensityX = core.density_map_pair.x.map;
//        RhoSystem.Variables.Buffers.DensityY = core.density_map_pair.y.map;
//        RhoVariables.ram.CX_ADDR = &core.centroid.x;
//        RhoVariables.ram.CY_ADDR = &core.centroid.y;
//        RhoSystem.Variables.Buffers.Quadrant = core.quadrant;
//        core.density_map_pair.x.has_background = true;
//        core.density_map_pair.y.has_background = true;
//    }
}

void RhoWrapper::PrintSizes( void )
{
//    size_t
//    aa = CAPTURE_WIDTH + CAPTURE_HEIGHT,
//    a = sizeof(redistribution_variables),
//    b = sizeof(rho_detection_variables),
//    c = sizeof(prediction_predict_variables),
//    d = sizeof(rho_core_t)-sizeof(core.cframe),
//    e = a + b + c + d,
//    f = sizeof(density_map_pair_t),
//    g = sizeof(prediction_pair_t),
//    h = sizeof(pid_filter_t),
//    j = sizeof(kalman_filter_t)
//#ifdef __PSM__
//    ,k = sizeof(gaussian_mixture_model_t),
//    l = sizeof(hidden_markov_model_t),
//    m = sizeof(fsm_system_t),
//    n = sizeof(psm_t);
//    LOG_RHO(RHO_DEBUG_INIT, "\tgmm: %luB\thmm: %luB\tfsm: %luB\tpsm: %luB\n",k,l,m,n);
//#else
//    ;
//#endif
//    LOG_RHO(RHO_DEBUG_INIT, "\tSizes> Frame:%.3fkB RedVar:%luB SelVars:%luB PredVars:%luB Rho:%lukB > Tot:%.3fkB\n", ((double)aa)/1024, a, b, c, d>>10, ((double)e)/1024);
//    LOG_RHO(RHO_DEBUG_INIT, "\target_densitymapp:%luB\tpredensity_map:%luB\tpid: %luB\trkal: %luB\n",f,g,h,j);
}
