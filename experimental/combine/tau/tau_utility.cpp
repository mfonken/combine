//
//  tau_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "tau_utility.hpp"

using namespace cv;

Tau::Tau( const char * name, int width, int height, std::string f, int num, std::string n )
:
TestInterface(1, name),
rho(width, height),
width(width), height(height),
utility(n, f, num, width, height)
#ifdef RHO_DRAWER
#ifdef __PSM__
,rho_drawer(&rho.core.PredictiveStateModelPair.y)
#endif
#endif
{
    utility.Init();
    cimageInit(image, width, height);
    
    A = {(double)width/2., (double)height/2.};
    B = {(double)width/2., (double)height/2.};
    
    pthread_mutex_init(&predictions_mutex, NULL);
}

Tau::~Tau() {}

void Tau::Init( void )
{
    rho.core.Thresh = DEFAULT_THRESH;
    count = 0;
    accuracy_count = 0;
    avg = 0;
    accuracy = 0;
    current_accuracy = 0;
    tick = 0;
    utility.generator_active = true;
}

static bool up = true;
static int sweep_speed = 1;
void Tau::Trigger( void )
{
    double p = 0.;

    utility.Trigger();
    
    p = Perform( utility.outimage );
    LOG_TAU("Tau perform: %.3fs\n", p);
    if(count < MAX_COUNT)
    {
        double pacc = accuracy;
        RhoUtility.Generate.CumulativeAverage(p, &avg, &count);
        stddev_sum += pacc;
        RhoUtility.Generate.CumulativeAverage(current_accuracy, &accuracy, &accuracy_count);
        if(accuracy_count > AVERAGE_COUNT) accuracy_count--;
    }
//    if(( up && rho.core.Thresh > THRESH_MAX ) || (!up && rho.core.Thresh < THRESH_MIN)) up = !up;
//    rho.core.Thresh += sweep_speed*(up?1:-1); ///TEST
    rho.core.ThreshByte = (byte_t)rho.core.Thresh;
    
#ifdef __PSM__
    rho_drawer.DrawDetectionMap( &rho.core.DetectionMap, rho.core.ThreshByte );
#endif
#ifdef PRINT_TUNING_STAGES
    if(!(rho.core.ThreshByte % THRESH_FRAME_PRINT_STEP))
    {
        imwrite(FRAME_SAVE_ROOT + string("thresh_frame_") + to_string(rho.core.ThreshByte) + string(".png"), utility.outframe);
    }
#endif
}

std::string Tau::Serialize( void )
{
    return "";
}

double Tau::Perform( cimage_t &img )
{
    bool background_event = !!( BACKGROUNDING_PERIOD && ++tick >= BACKGROUNDING_PERIOD );
    
    if( background_event )
    {
        utility.RequestBackground();
        LOG_TAU(DEBUG_1, "Waiting for utility to generate background...\n");
        
        while(!utility.background_ready)
            utility.Trigger();
        
        rho.backgrounding_event = true;
    }
    
    double p = 0;
    
#ifdef CV_TRACK_BLOBS
    vector<KeyPoint> keypoints;
    pthread_mutex_lock(&utility.keypoints_mutex);
    utility.detector.getKeypoints(keypoints);
    pthread_mutex_unlock(&utility.keypoints_mutex);
    if(keypoints.size() >= 2)
    {
        std::sort(keypoints.begin(), keypoints.end(), [](const KeyPoint& a, const KeyPoint& b) -> bool { return a.size > b.size; });
        packet.px = keypoints[0].pt.x; packet.py = keypoints[0].pt.y;
        packet.sx = keypoints[1].pt.x; packet.sy = keypoints[1].pt.y;
    }
    else
    {
        memset(&packet, 0, sizeof(packet));
    }
#else
    p = rho.Perform( img, &packet );
#endif
    
    if( background_event )
    {
        utility.background_ready = false;
        tick = 0;
    }
    else
    {
        UpdateThresh();
        UpdatePrediction();
    }

#ifdef CV_TRACK_BLOBS
#else
    double Cx = utility.pCx-rho.core.Centroid.x,
    Cy = utility.pCy-rho.core.Centroid.y;
    current_accuracy = sqrt( DISTANCE_SQ( Cx, Cy ) );
#endif
    
    PrintPacket(&packet, 4);
    return p;
}

void Tau::UpdateThresh()
{
    utility.thresh = rho.core.ThreshByte;
}

void Tau::UpdatePrediction()
{
    pthread_mutex_lock(&predictions_mutex);
    Point2f a(packet.py, packet.px),
            b(packet.sy, packet.sx);
    
    if( a.x > b.x ) swap(a,b);
#ifdef FISHEYE
    invfisheye(&a, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
    invfisheye(&b, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
#endif
    A = { a.x, a.y };
    B = { b.x, b.y };
    pthread_mutex_unlock(&predictions_mutex);
}

void Tau::PrintPacket( GlobalPacket * p, int l )
{
#ifdef TAU_DEBUG
    printf("Packet Size > %lubytes\n", sizeof(GlobalPacket));
    for(int i = 0; i < sizeof(GlobalPacket); )
    {
        printf("(%02d)", i);
        for( int j = 0; j < l && i < sizeof(GlobalPacket); j++, i++ )
            printf(" 0x%02x", *(unsigned char*)(&((unsigned char*)&p->header)[i]));
        printf("\n");
    }
    
    uint32_t px = *(uint32_t*)&p->px;
    double pxf = *(double*)&p->px;
    uint8_t * ptr = (uint8_t*)&px;
    
    uint8_t pxr[4] = {ptr[3],ptr[2],ptr[1],ptr[0]};
    double pxfr = *(double*)&pxr;
    printf("[%02x][%02x][%02x][%02x] %f:%f\n",ptr[0],ptr[1],ptr[2],ptr[3],pxf,pxfr);
#endif
}
