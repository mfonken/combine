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

#define DEBUG_RHO_WRAP DEBUG_1
#define DEBUG_RHO_WRAP_BUFFERS
#define RHO_TRACK_BLOBS
#define DO_NOT_TIME_ACQUISITION

#ifdef DEBUG_RHO_WRAP
#define LOG_RHO_WRAP(L, ...) LOG(L, "<RhoWrapper> " __VA_ARGS__)
#define LOG_RHO_WRAP_BARE(L, ...) LOG_BARE(L, __VA_ARGS__)
#else
#define LOG_RHO_WRAP(L, ...)
#define LOG_RHO_WRAP_BARE(L, ...)
#endif


class RhoWrapper
{
    cimage_t cimage;
    
public:
//    byte_t blob_padding = RHO_BLOB_PADDING_PX;
    int                 width;
    int                 height;
//    rho_core_t          core;
//    pthread_mutex_t     c_mutex;
//    pthread_mutex_t     density_map_pair_mutex;
    bool                backgrounding_event;
    
    rho_capture_t capture;
    rho_packet_t packet;
    
    ~RhoWrapper();
    void Init( int, int );
    double Perform( cv::Mat& );
    double Perform( cimage_t& );
    void Reset();
    void ResetThresh();
    void DecoupleFull( const cimage_t );
    void DecoupleBlobs( const cimage_t );
    int Decouple( rho_core_t *, const cimage_t, bool );
    void PopulateDensityMapOffsets( density_map_pair_t * d, rho_capture_t * c, int n );
    std::vector<cv::Point2f> GetPredictions();
    void ToggleBackgrounding( bool );
    void PrintSizes( void );
};

#endif /* rho_wrapper_hpp */
