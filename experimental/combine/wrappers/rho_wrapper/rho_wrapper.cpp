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
    this->width = width;
    this->height = height;
    
    LOG_RW(RHO_DEBUG, "Initializing Rho Utility: %dx%d & [KTarg-%f, VarNorm-%.3f, VarSca-%.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    
//    pthread_mutex_init(&density_map_pair_mutex, NULL);
//    pthread_mutex_init(&c_mutex, NULL);

    cimageInit( cimage, width, height );
    
    
    RhoCore.Initialize(&RhoSystem.Variables.Utility, width, height);
    RhoSystem.Variables.Utility.subsample = 1; /// TODO: Find a place for this
    
    RhoSystem.Variables.Buffers.Thresh = (index_t *)malloc(sizeof(index_t) * THRESH_BUFFER_SIZE);
    RhoSystem.Variables.Addresses.ThreshFill = RhoSystem.Variables.Buffers.Thresh;
    RhoSystem.Variables.Buffers.Quadrant = RhoSystem.Variables.Utility.quadrant;
    
    backgrounding_event = false;
    
    PrintSizes();
}

double RhoWrapper::Perform( cv::Mat & mat )
{    
    if(!cimageFromMat( mat, cimage ))
        return -1;
    return Perform( cimage );
}

double RhoWrapper::Perform( cimage_t & img )
{
//    pthread_mutex_lock(&c_mutex);
//    pthread_mutex_lock(&density_map_pair_mutex);
    
    rho_core_t * core = &RhoSystem.Variables.Utility;
    
    RhoUtility.Reset.DensityMapPairKalmans( core );
    
    // Create blobs if available
    active_blobs = 0;
    if( core->prediction_pair.num_regions >= 2)
    {
        for( byte_t i = 0; i < 2; i++ )
        {
            order_t * xi = &core->prediction_pair.x.regions_order[(i + (core->prediction_pair.descending ? 1 : 0)) % 2];
            order_t * yi = &core->prediction_pair.y.regions_order[i];
            if(!xi->valid || !yi->valid)
            {
                active_blobs = 0;
                break;
            }
            
            region_t * xr = &core->prediction_pair.x.regions[xi->index];
            region_t * yr = &core->prediction_pair.y.regions[yi->index];
            
            /// Note: x and y are switched intentionally
            blobs[i].x = MAX( 0, yr->location - yr->width / 2 - blob_padding );
            blobs[i].y = MAX( 0, xr->location - xr->width / 2 - blob_padding );
            blobs[i].w = yr->width + 2 * blob_padding;
            blobs[i].h = xr->width + 2 * blob_padding;
            
            active_blobs++;
        }
    }
    
    /* Core Rho Functions */
    struct timeval a,b;
    gettimeofday( &a, NULL);
    Decouple( img, backgrounding_event, blobs, active_blobs );
#ifdef DO_NOT_TIME_ACQUISITION
    gettimeofday( &a, NULL);
#endif
    if(core->quadrant[0] + core->quadrant[1] + core->quadrant[2] + core->quadrant[3])
        RhoCore.Perform( core, backgrounding_event );
    gettimeofday( &b, NULL);

    /* * * * * * * * * * */
    
//    pthread_mutex_unlock(&density_map_pair_mutex);
    memcpy((byte_t *)&packet, (byte_t*)&core->packet, sizeof(packet_t));
    backgrounding_event = false; // Generate background always and only once
//    pthread_mutex_unlock(&c_mutex);
    
    return timeDiff(a,b);
}

void RhoWrapper::Reset()
{
    RhoSystem.Variables.Addresses.ThreshFill = RhoSystem.Variables.Buffers.Thresh;
    
    memset( RhoSystem.Variables.Utility.density_map_pair.x.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.x.length);
    memset( RhoSystem.Variables.Utility.density_map_pair.y.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.y.length);
}

void RhoWrapper::Decouple( const cimage_t image, bool backgrounding, blob_t * blobs, byte_t n )
{
    Reset();
//    if(backgrounding) ToggleBackgrounding(true);
    
    RhoCapture.ResetAll( &capture );
    
    // Prepare blobs
    if( n > 0)
    {
        for( byte_t i = 0; i < n; i++ )
            RhoCapture.AddBlob( &capture, blobs[i] );
        RhoCapture.PrepareBlobsForCapture( &capture, image.height );
    }
    
    RhoSystem.Variables.Addresses.Capture = (byte_t *)image.pixels;
    for( index_t y = 0; y < image.height; y += RhoSystem.Variables.Utility.subsample )
    {
        RhoSystem.Variables.Addresses.ThreshFill = RhoCapture.CaptureRow(
                              RhoSystem.Variables.Addresses.Capture,
                              RhoSystem.Variables.Utility.thresh_byte,
                              RhoSystem.Variables.Addresses.ThreshFill,
                              image.width,
                              RhoSystem.Variables.Utility.subsample );
        *(RhoSystem.Variables.Addresses.ThreshFill++) = CAPTURE_ROW_END;
        RhoSystem.Variables.Addresses.Capture += image.width;
//        RhoSystem.Variables.Flags->EvenRowToggle = !RhoSystem.Variables.Flags->EvenRowToggle;
    }
    RhoSystem.Variables.Addresses.ThreshEnd = RhoSystem.Variables.Addresses.ThreshFill;
//    for(int i = 0; i < RhoSystem.Variables.Addresses.ThreshFill - thresh_start; i++)
//    {
//        printf("0x%x ", *(thresh_start + i));
//    }
//    printf("\n");
    
    section_process_t ProcessedSectionData[2];
    uint16_t rows = RhoSystem.Variables.Utility.centroid.y;
//    printf("%p | %p\n", RhoSystem.Variables.Utility.density_map_pair.x.map, RhoSystem.Variables.Utility.density_map_pair.y.map);
    index_t thresh_proc = 0, rows_proc = 0;
    for( byte_t i = 0; i < 2; i++ )
    {
        do{ ProcessedSectionData[i] = RhoCapture.ProcessFrameSection( rows,
                                                                     RhoSystem.Variables.Buffers.Thresh + thresh_proc,
                                                                     RhoSystem.Variables.Addresses.ThreshEnd,
                                                                     RhoSystem.Variables.Utility.centroid.x,
                                                                     RhoSystem.Variables.Utility.density_map_pair.y.map,
                                                                     RhoSystem.Variables.Utility.density_map_pair.x.map + rows_proc );
        } while( !ProcessedSectionData[i].complete );
        rows = RhoSystem.Variables.Utility.height - rows;
        thresh_proc += ProcessedSectionData[i].thresh_proc;
        rows_proc += ProcessedSectionData[i].rows_proc;
    }
    
//    printf("%p | %p\n", RhoSystem.Variables.Utility.density_map_pair.x.map, RhoSystem.Variables.Utility.density_map_pair.y.map);
    
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_TOP_LEFT_INDEX]  = ProcessedSectionData[0].left;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_TOP_RIGHT_INDEX] = ProcessedSectionData[0].right;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_BTM_LEFT_INDEX]  = ProcessedSectionData[1].left;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_BTM_RIGHT_INDEX] = ProcessedSectionData[1].right;
    
//    if(backgrounding) ToggleBackgrounding(false);
}

std::vector<cv::Point2f> RhoWrapper::GetPredictions()
{
    rho_core_t * core = &RhoSystem.Variables.Utility;
    
    std::vector<cv::Point2f> pts(2);
    pts[0] = cv::Point2d(core->primary.x, core->primary.y);
    pts[1] = cv::Point2d(core->secondary.x, core->secondary.y);
    return pts;
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
