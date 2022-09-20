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
    
    RhoSystem.Variables.Addresses.CaptureMax = cimage.pixels + width * height;
    RhoSystem.Variables.Buffers.Thresh = (index_t *)malloc(sizeof(index_t) * THRESH_BUFFER_SIZE);
    RhoSystem.Variables.Addresses.ThreshMax = (address_t)( RhoSystem.Variables.Buffers.Thresh + THRESH_BUFFER_SIZE );
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
    active_blob_confidence = 0.0;
    if( core->prediction_pair.num_regions >= 2)
    {
        for( byte_t i = 0; i < core->prediction_pair.num_blobs; i++ )
        {
            index_pair_t * bo = &core->prediction_pair.blobs_order[i];
            kalman_t * fx = &core->prediction_pair.x.trackers[bo->x].kalman;
            kalman_t * fy = &core->prediction_pair.y.trackers[bo->y].kalman;
            byte_t tox = core->prediction_pair.x.trackers_order[bo->x];
            byte_t toy = core->prediction_pair.y.trackers_order[bo->y];
            order_t * rox = &core->prediction_pair.x.regions_order[tox];
            order_t * roy = &core->prediction_pair.y.regions_order[toy];
            if(!fx->valid || !fy->valid || !rox->valid || !roy->valid )
            {
                active_blobs = 0;
                break;
            }

            region_t * rx = &core->prediction_pair.x.regions[rox->index];
            region_t * ry = &core->prediction_pair.y.regions[roy->index];

            floating_t testA = Kalman.Test( fx, fx->rate );
            floating_t testB = Kalman.Test( fy, fy->rate );
            blobs[i].y = MAX( 0, testA - rx->width / 2 - blob_padding );
            blobs[i].x = MAX( 0, testB - ry->width / 2 - blob_padding );
            blobs[i].h = rx->width + 2 * blob_padding;
            blobs[i].w = ry->width + 2 * blob_padding;
            active_blob_density = ( rx->density + ry->density ) / 2 * 0.9;

            active_blobs++;
            active_blob_confidence = fx->K[0];//( fx->K[0] + fy->P[0][0] ) / 2;
            printf("CONFIDENCE: %.2f\n", active_blob_confidence);

//            printf("[%d (%d, %d) %dx%d]\n", i, blobs[i].x, blobs[i].y, blobs[i].w, blobs[i].h);
        }
    }
    
    /* Core Rho Functions */
    floating_t a = TIMESTAMP_MS();
    
    Decouple( img, backgrounding_event, blobs, active_blobs );
    
#ifdef DO_NOT_TIME_ACQUISITION
    a = TIMESTAMP_MS();
#endif
    
    if(core->quadrant[0] + core->quadrant[1] + core->quadrant[2] + core->quadrant[3])
        RhoCore.Perform( core, backgrounding_event );
    floating_t b = TIMESTAMP_MS();

    /* * * * * * * * * * */
    
//    pthread_mutex_unlock(&density_map_pair_mutex);
    memcpy((byte_t *)&packet, (byte_t*)&core->packet, sizeof(packet_t));
    backgrounding_event = false; // Generate background always and only once
//    pthread_mutex_unlock(&c_mutex);
    
    return b - a;
}

void RhoWrapper::Reset()
{
    RhoSystem.Variables.Addresses.ThreshFill = RhoSystem.Variables.Buffers.Thresh;
    
    memset( RhoSystem.Variables.Utility.density_map_pair.x.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.x.length);
    memset( RhoSystem.Variables.Utility.density_map_pair.y.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.y.length);
}

void RhoWrapper::DecoupleFull( const cimage_t image )
{
    RhoSystem.Variables.Addresses.Capture = (byte_t *)image.pixels;
    index_t * new_thresh = NULL;
    for( index_t y = 0; y < image.height; y += RhoSystem.Variables.Utility.subsample )
    {
         new_thresh = RhoCapture.CaptureRow(
                                RhoSystem.Variables.Addresses.Capture,
                                RhoSystem.Variables.Utility.thresh_byte,
                                RhoSystem.Variables.Addresses.ThreshFill,
                                0,
                                image.width,
                                RhoSystem.Variables.Utility.subsample );
        if( new_thresh != RhoSystem.Variables.Addresses.ThreshFill )
        {
            *(RhoSystem.Variables.Addresses.ThreshFill) = CAPTURE_ROW_END + y;
            RhoSystem.Variables.Addresses.ThreshFill = new_thresh + 1;
        }
        RhoSystem.Variables.Addresses.Capture += image.width;
//        RhoSystem.Variables.Flags->EvenRowToggle = !RhoSystem.Variables.Flags->EvenRowToggle;
    }
    *(RhoSystem.Variables.Addresses.ThreshFill++) = CAPTURE_ROW_END;
}

void RhoWrapper::DecoupleBlobs( const cimage_t image )
{
    RhoSystem.Variables.Addresses.Capture = (byte_t *)image.pixels;
    
//    // Print pixels
//    for( int y = 0; y < image.height; y++ )
//    {
//        printf("%p - %d: ", &image.pixels[y*image.width], y);
//        for( int x = 0; x < image.height; x++ )
//            printf("%d ", image.pixels[y*image.width + x]);
//        printf("\n");
//    }
    
    index_t y = capture.curr_edge->i;
    byte_t * capture_address = RhoSystem.Variables.Addresses.Capture + y * image.width;
    index_t * new_thresh = NULL;
    for( ; y < image.height; y += RhoSystem.Variables.Utility.subsample )
    {
        new_thresh = RhoCapture.CaptureBlobs(
                                           &capture,
                                            y,
                                            capture_address,
                                            RhoSystem.Variables.Utility.thresh_byte,
                                            RhoSystem.Variables.Addresses.ThreshFill,
                                            image.width,
                                            RhoSystem.Variables.Utility.subsample);
        
        capture_address += image.width;
        if( new_thresh != RhoSystem.Variables.Addresses.ThreshFill )
        {
            *(RhoSystem.Variables.Addresses.ThreshFill) = CAPTURE_ROW_END + y;
            RhoSystem.Variables.Addresses.ThreshFill = new_thresh + 1;
        }
        if( capture.done )
            break;
        else if( capture.num_active == 0 )
        {
            y = capture.curr_edge->i;
//            printf("c - %p + %d = ", RhoSystem.Variables.Addresses.Capture, y * image.width);
            capture_address = RhoSystem.Variables.Addresses.Capture + y * image.width;
//            printf("%p\n", capture_address);
        }
//        RhoSystem.Variables.Flags->EvenRowToggle = !RhoSystem.Variables.Flags->EvenRowToggle;
    }
    *(RhoSystem.Variables.Addresses.ThreshFill++) = CAPTURE_ROW_END;
}

void RhoWrapper::Decouple( const cimage_t image, bool backgrounding, blob_t * blobs, byte_t n )
{
    Reset();
//    if(backgrounding) ToggleBackgrounding(true);
    
    RhoCapture.ResetAll( &capture );
    
    // Prepare blobs
    if( n >= 2)
    {
        for( byte_t i = 0; i < n; i++ )
            RhoCapture.AddBlob( &capture, blobs[i] );
        RhoCapture.PrepareBlobsForCapture( &capture, image.height - 1 );
    }
    /// TODO: Change CaptureRow over to blobs when appropriate (has blobs + confidence)
    
    printf("%d blobs\n", n);
    if( n >= 2 && active_blob_confidence >= RHO_MIN_BLOB_CONFIDENCE )
    {
        double start = TIMESTAMP(TIME_US);
        DecoupleBlobs( image );
        printf("Decouple>Blobs: %.3fms\n", (TIMESTAMP(TIME_US) - start) / 1.0e3);
    }
    if( RhoSystem.Variables.Addresses.ThreshFill <= RhoSystem.Variables.Buffers.Thresh + active_blob_density )
    {
        double start = TIMESTAMP(TIME_US);
        DecoupleFull( image );
        printf("Decouple>Full: %.3fms\n", (TIMESTAMP(TIME_US) - start) / 1.0e3);
    }
    RhoSystem.Variables.Addresses.CaptureEnd = RhoSystem.Variables.Addresses.CaptureMax; // Full frame has been processed
    RhoSystem.Variables.Addresses.ThreshEnd = RhoSystem.Variables.Addresses.ThreshFill;
    
//    // Print thresh buffer
//    for( index_t * p = RhoSystem.Variables.Buffers.Thresh; p < RhoSystem.Variables.Addresses.ThreshFill; p++)
//    {
//        if( *p >= CAPTURE_ROW_END)
//            printf("\n%d: ", *p - CAPTURE_ROW_END);
//        else
//            printf("%d ", *p);
//    }
//    printf("\n");
    
    section_process_t ProcessedSectionData[2];
    uint16_t end_row = RhoSystem.Variables.Utility.centroid.y;
//    printf("%p | %p\n", RhoSystem.Variables.Utility.density_map_pair.x.map, RhoSystem.Variables.Utility.density_map_pair.y.map);
    index_t thresh_proc = 0, rows_proc = 0;
    for( byte_t i = 0; i < 2; i++ )
    {
        do{ ProcessedSectionData[i] = RhoCapture.ProcessFrameSection( end_row,
                                                                      RhoSystem.Variables.Buffers.Thresh + thresh_proc,
                                                                      RhoSystem.Variables.Addresses.ThreshEnd,
                                                                      RhoSystem.Variables.Utility.centroid.x,
                                                                      RhoSystem.Variables.Utility.density_map_pair.y.map,
                                                                      RhoSystem.Variables.Utility.density_map_pair.x.map,
                                                                      rows_proc );
        } while( !ProcessedSectionData[i].complete &&
                RhoSystem.Variables.Addresses.CaptureEnd < RhoSystem.Variables.Addresses.CaptureMax );
        end_row = RhoSystem.Variables.Utility.height;
        thresh_proc += ProcessedSectionData[i].thresh_proc;
        rows_proc = ProcessedSectionData[i].rows_proc;
    }
    
//    // Print density maps
//    printf("X: ");
//    for( int i = 0; i < RhoSystem.Variables.Utility.density_map_pair.x.length; i++)
//    {
//        printf("%d ", RhoSystem.Variables.Utility.density_map_pair.x.map[i]);
//    }
//    printf("\n");
//
//    printf("Y: ");
//    for( int i = 0; i < RhoSystem.Variables.Utility.density_map_pair.y.length; i++)
//    {
//        printf("%d ", RhoSystem.Variables.Utility.density_map_pair.y.map[i]);
//    }
//    printf("\n");
    
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
