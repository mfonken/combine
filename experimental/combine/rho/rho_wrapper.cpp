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

//using namespace cv;

Rho::Rho( int width, int height ) : width(width), height(height)
{
    LOG_RHO(RHO_DEBUG, "Initializing Rho Utility: %dx%d & [KTarg-%f, VarNorm-%.3f, VarSca-%.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    
    pthread_mutex_init(&density_map_pair_mutex, NULL);
    pthread_mutex_init(&c_mutex, NULL);
    
    RhoCore.Initialize(&core, width, height);
    backgrounding_event = false;
    
    PrintSizes();
}

double Rho::Perform( cimage_t & img, GlobalPacket * p )
{
    pthread_mutex_lock(&c_mutex);
    pthread_mutex_lock(&density_map_pair_mutex);
    
    /* Core Rho Functions */
    struct timeval a,b;
    gettimeofday( &a, NULL);
    Decouple( img, backgrounding_event );
#ifdef DO_NOT_TIME_ACQUISITION
    gettimeofday( &a, NULL);
#endif
    if(core.Q[0] + core.Q[1] + core.Q[2] + core.Q[3])
        RhoCore.Perform( &core, backgrounding_event );
    gettimeofday( &b, NULL);

    /* * * * * * * * * * */
    
    pthread_mutex_unlock(&density_map_pair_mutex);
    memcpy((byte_t *)p, (byte_t*)&core.Packet, sizeof(packet_t));
    backgrounding_event = false; // Generate background always and only once
    pthread_mutex_unlock(&c_mutex);
    
    return timeDiff(a,b);
}

/* Interrupt (Simulated Hardware-Driven) Density map generator */
void Rho::Decouple( const cimage_t image, bool backgrounding )
{
    if( backgrounding )
    {
        RhoVariables.ram.Dx      =  core.DensityMapPair.x.background;
        RhoVariables.ram.Dy      =  core.DensityMapPair.y.background;
        RhoVariables.ram.CX_ADDR = &core.Secondary.y;
        RhoVariables.ram.CY_ADDR = &core.Secondary.x;
        RhoVariables.ram.Q       =  core.Qb;
    }
    
    RhoInterrupts.RHO_FUNCTION( image );
    
    if( backgrounding )
    {
        RhoVariables.ram.Dx      =  core.DensityMapPair.x.map;
        RhoVariables.ram.Dy      =  core.DensityMapPair.y.map;
        RhoVariables.ram.CX_ADDR = &core.Centroid.x;
        RhoVariables.ram.CY_ADDR = &core.Centroid.y;
        RhoVariables.ram.Q       =  core.Q;
        core.DensityMapPair.x.has_background = true;
        core.DensityMapPair.y.has_background = true;
    }
}

void Rho::PrintSizes( void )
{
    size_t
    aa = CAPTURE_WIDTH + CAPTURE_HEIGHT,
    a = sizeof(redistribution_variables),
    b = sizeof(rho_detection_variables),
    c = sizeof(prediction_predict_variables),
    d = sizeof(rho_core_t)-sizeof(core.cframe),
    e = a + b + c + d,
    f = sizeof(density_map_pair_t),
    g = sizeof(prediction_pair_t),
    h = sizeof(pid_filter_t),
    j = sizeof(kalman_filter_t)
#ifdef __PSM__
    ,k = sizeof(gaussian_mixture_model_t),
    l = sizeof(hidden_markov_model_t),
    m = sizeof(fsm_system_t),
    n = sizeof(psm_t);
    LOG_RHO(RHO_DEBUG_INIT, "\tgmm: %luB\thmm: %luB\tfsm: %luB\tpsm: %luB\n",k,l,m,n);
#else
    ;
#endif
    LOG_RHO(RHO_DEBUG_INIT, "\tSizes> Frame:%.3fkB RedVar:%luB SelVars:%luB PredVars:%luB Rho:%lukB > Tot:%.3fkB\n", ((double)aa)/1024, a, b, c, d>>10, ((double)e)/1024);
    LOG_RHO(RHO_DEBUG_INIT, "\target_densitymapp:%luB\tpredensity_map:%luB\tpid: %luB\trkal: %luB\n",f,g,h,j);
}
