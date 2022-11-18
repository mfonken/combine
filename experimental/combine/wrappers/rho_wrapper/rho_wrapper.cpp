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

static void PRINT_IMAGE(const cimage_t * image)
{
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Image:\n");
    for( int y = 0; y < image->height; y++ )
    {
        LOG_RHO_WRAP(DEBUG_RHO_WRAP, "%p - %d: ", &image->pixels[y*image->width], y);
        for( int x = 0; x < image->width; x++ )
        {
            LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "%d ", image->pixels[y*image->width + x] ? 1 : 0);
        }
        LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "\n");
    }
}

static void PRINT_THRESH(const cimage_t * image)
{
    // Print thresh buffer
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Thresh buffer:");
    for( index_t * p = RhoSystem.Variables.Buffers.Thresh; p < RhoSystem.Variables.Addresses.ThreshFill; p++)
    {
        index_t v = *p;
        if( v >= image->width)
        {
            int8_t b = -2;
            while( v >= image->width )
            {
                v -= image->width;
                b++;
            }
            LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "\n%d[%d]: ", v, b);
        }
        else
            LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "%d ", v);
    }
    LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "\n");
}

static void PRINT_DENSITY_MAPS( rho_core_t * core, rho_capture_t * capture )
{
    // Print density maps
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "X: ");
    for( int i = 0; i < core->density_map_pair.x.length; i++)
    {
        LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "%d ", core->density_map_pair.x.map[i]);
        for( int ii = 0; ii < MAX_BLOBS; ii++)
            if(i == capture->thresh_blob_loc[ii].y)
                LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "|%d| ", ii);
    }
    LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "\n");

    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Y: ");
    for( int i = 0; i < core->density_map_pair.y.length; i++)
    {
        LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "%d ", core->density_map_pair.y.map[i]);
        for( int ii = 0; ii < MAX_BLOBS; ii++)
            if(i == capture->thresh_blob_loc[ii].x)
                LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "|%d| ", ii);
    }
    LOG_RHO_WRAP_BARE(DEBUG_RHO_WRAP, "\n");
}

void RhoWrapper::Init( int width, int height )
{
    this->width = width;
    this->height = height;
    
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Initializing Rho Utility: %dx%d & [KTarg-%f, VarNorm-%.3f, VarSca-%.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    
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
    
    capture.blobs = RhoSystem.Variables.Utility.prediction_pair.blobs;
    capture.num_blobs = &RhoSystem.Variables.Utility.prediction_pair.num_blobs;
    
    PrintSizes();
}

RhoWrapper::~RhoWrapper()
{
    cimageDeinit(cimage);
}

void RhoWrapper::Reset()
{
    RhoCapture.ResetAll( &capture );
    
    RhoUtility.Reset.DensityMapPairKalmans( &RhoSystem.Variables.Utility );
    ResetThresh();
}

void RhoWrapper::ResetThresh()
{
    RhoSystem.Variables.Addresses.ThreshFill = RhoSystem.Variables.Buffers.Thresh;
    
    memset( RhoSystem.Variables.Utility.density_map_pair.x.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.x.length);
    memset( RhoSystem.Variables.Utility.density_map_pair.y.map, 0, sizeof(sdensity_t) * RhoSystem.Variables.Utility.density_map_pair.y.length);
}

double RhoWrapper::Perform( cv::Mat &mat )
{    
    if(!cimageFromMat( mat, cimage ))
        return -1;
    return Perform( cimage );
}

double RhoWrapper::Perform( cimage_t & img )
{
    //    pthread_mutex_lock(&c_mutex);
    //    pthread_mutex_lock(&density_map_pair_mutex);
    Reset();
    
    rho_core_t * core = &RhoSystem.Variables.Utility;
    
    /* Core Rho Functions */
    floating_t a = TIMESTAMP_MS();
    {
        int density = Decouple( core, img, backgrounding_event );
        if(density)
            RhoCore.Perform( core, backgrounding_event );
    }
    floating_t b = TIMESTAMP_MS();
    /* * * * * * * * * * */
    
//    pthread_mutex_unlock(&density_map_pair_mutex);
    memcpy((byte_t *)&packet, (byte_t*)&core->packet, sizeof(packet_t));
    backgrounding_event = false; // Generate background always and only once
//    pthread_mutex_unlock(&c_mutex);
    
    return b - a;
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
        
        
        if( new_thresh != RhoSystem.Variables.Addresses.ThreshFill )
        {
            *(RhoSystem.Variables.Addresses.ThreshFill) += CAPTURE_ROW_END;
            RhoSystem.Variables.Addresses.ThreshFill = new_thresh;
        }
        capture_address += image.width;
        if( capture.done )
            break;
        else if( capture.num_active == 0 )
        {
            y = capture.curr_edge->i;
            capture_address = RhoSystem.Variables.Addresses.Capture + y * image.width;
            y--;
        }
//        RhoSystem.Variables.Flags->EvenRowToggle = !RhoSystem.Variables.Flags->EvenRowToggle;
    }
    *(RhoSystem.Variables.Addresses.ThreshFill++) = CAPTURE_ROW_END;
}

int RhoWrapper::Decouple( rho_core_t * core, const cimage_t image, bool backgrounding )
{
//    if(backgrounding) ToggleBackgrounding(true);
    byte_t n = *capture.num_blobs;
    density_t min_density = core->filtered_coverage * RHO_MIN_BLOB_DENSITY_CONTINUITY;
#ifdef RHO_TRACK_BLOBS
    // Prepare blobs
    if( n >= 2)
    {
        RhoCapture.PrepareBlobsForCapture( &capture );
    }
#endif
    
#ifdef DEBUG_RHO_WRAP_BUFFERS
//    PRINT_IMAGE( &image );
#endif
    
    bool decouple_full = true;
#ifdef RHO_TRACK_BLOBS
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "%d blobs\n", *capture.num_blobs);
    if( n >= 2 )
    {
        if(n > 2)
            printf("#");
        double start = TIMESTAMP(TIME_US);
        DecoupleBlobs( image );
        LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Decouple>Blobs: %.3fms\n", (TIMESTAMP(TIME_US) - start) / 1.0e3);
        
        if( RhoSystem.Variables.Addresses.ThreshFill <= RhoSystem.Variables.Buffers.Thresh + min_density )
            ResetThresh();
        else
            decouple_full = false;
    }
#endif
    if(decouple_full)
    {
        double start = TIMESTAMP(TIME_US);
        DecoupleFull( image );
        *capture.num_blobs = 0;
        LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Decouple>Full: %.3fms\n", (TIMESTAMP(TIME_US) - start) / 1.0e3);
    }
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Thresh fill %ld | %d\n", RhoSystem.Variables.Addresses.ThreshFill - RhoSystem.Variables.Buffers.Thresh, min_density);
    RhoSystem.Variables.Addresses.CaptureEnd = RhoSystem.Variables.Addresses.CaptureMax; // Full frame has been processed
    RhoSystem.Variables.Addresses.ThreshEnd = RhoSystem.Variables.Addresses.ThreshFill;
    
#ifdef DEBUG_RHO_WRAP_BUFFERS
//    PRINT_THRESH( &image );
#endif
    
    section_process_t ProcessedSectionData[2];
    uint16_t end_row = core->centroid.y;
//    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "%p | %p\n", core->density_map_pair.x.map, core->density_map_pair.y.map);
    index_t thresh_proc = 0, rows_proc = 0;
    LOG_RHO_WRAP(DEBUG_RHO_WRAP, "Centroid: x%d y%d\n", core->centroid.x, end_row);
    for( byte_t i = 0; i < 2; i++ )
    {
        do { ProcessedSectionData[i] = RhoCapture.ProcessFrameSection( end_row,
                                                                      RhoSystem.Variables.Buffers.Thresh + thresh_proc,
                                                                      RhoSystem.Variables.Addresses.ThreshEnd,
                                                                      core->centroid.x,
                                                                      core->density_map_pair.y.map,
                                                                      core->density_map_pair.x.map,
                                                                      rows_proc,
                                                                      n < 2 ? NULL : &capture );
        } while( !ProcessedSectionData[i].complete &&
                RhoSystem.Variables.Addresses.CaptureEnd < RhoSystem.Variables.Addresses.CaptureMax );
        end_row = core->height;
        thresh_proc += ProcessedSectionData[i].thresh_proc - 1;
        rows_proc = ProcessedSectionData[i].rows_proc;
    }
    
#ifdef DEBUG_RHO_WRAP_BUFFERS
    PRINT_DENSITY_MAPS( core, &capture );
#endif
    
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_TOP_LEFT_INDEX]  = ProcessedSectionData[0].left;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_TOP_RIGHT_INDEX] = ProcessedSectionData[0].right;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_BTM_LEFT_INDEX]  = ProcessedSectionData[1].left;
    RhoSystem.Variables.Buffers.Quadrant[FRAME_QUADRANT_BTM_RIGHT_INDEX] = ProcessedSectionData[1].right;
    
    PopulateDensityMapOffsets( &core->density_map_pair, &capture, core->prediction_pair.num_blobs );
    
//    if(backgrounding) ToggleBackgrounding(false);
    return ProcessedSectionData[0].left + ProcessedSectionData[0].right + ProcessedSectionData[1].left + ProcessedSectionData[1].right;
}

void RhoWrapper::PopulateDensityMapOffsets( density_map_pair_t * d, rho_capture_t * c, int n )
{
#ifdef RHO_TRACK_BLOBS
    if(n >= 2)
    {
        printf("Offset:\n");
        index_pair_t loc = { 0, 0 }, prev = { 0, 0 };
        for( int i = 0; i < n && MAX_BLOBS; i++ )
        {
            loc.x = capture.thresh_blob_loc[i].x;
            loc.y = capture.thresh_blob_loc[i].y;
            
            d->y.buffer_loc[i] = loc.x;
            d->x.buffer_loc[i] = loc.y;
            
            uint8_t bi = capture.blobs_order[i];
            d->y.offset[i] = capture.blobs[bi].x + loc.x - prev.x;
            d->x.offset[i] = capture.blobs[bi].y + loc.y - prev.y;
            
            prev.x = loc.x;
            prev.y = loc.y;
            
            printf("%i - buff:%d|%d offset:%d|%d\n", i, d->y.buffer_loc[i], d->x.buffer_loc[i], d->y.offset[i], d->x.offset[i]);
        }
    }
    else
#endif
    {
        for( int i = 0; i < MAX_BLOBS; i++ )
        {
            d->y.offset[i] = 0;
            d->x.offset[i] = 0;
            d->y.buffer_loc[i] = 0;
            d->x.buffer_loc[i] = 0;
        }
    }
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
