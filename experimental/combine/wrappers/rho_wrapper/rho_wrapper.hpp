//
//  rho_wrapper.hpp
//  tau+
//
//  Created by Matthew Fonken on 3/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_wrapper_hpp
#define rho_wrapper_hpp

#include "opencv_master.h"
#include "rho_structures.hpp"
#include "rho_master.h"
#include "image_types.h"
#include "image_cpp_types.h"

#define DEBUG_RW

#ifdef DEBUG_RW
#define LOG_RW(L, ...) LOG(L, "<RhoWrapper> " __VA_ARGS__)
#else
#define LOG_RW(L, ...)
#endif


class RhoWrapper
{
    cimage_t cimage;
    
public:
    int                 width;
    int                 height;
//    rho_core_t          core;
//    pthread_mutex_t     c_mutex;
//    pthread_mutex_t     density_map_pair_mutex;
    bool                backgrounding_event;
    
    rho_capture_t capture;
    rho_packet_t packet;
    byte_t blob_padding = 50;
    blob_t blobs[MAX_BLOBS];
    byte_t active_blobs = 0;
    
    void Init( int, int );
    double Perform( cv::Mat& );
    double Perform( cimage_t& );
    void Reset();
    void Decouple( const cimage_t, bool, blob_t * blobs, byte_t n );
    std::vector<cv::Point2f> GetPredictions();
    void ToggleBackgrounding( bool );
    void PrintSizes( void );
};

#endif /* rho_wrapper_hpp */
