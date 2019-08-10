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
rho(width, height),
name(name), width(width), height(height),
utility(n, f, num, width, height)
#ifdef __PSM__
,rho_drawer(&rho.core.PredictiveStateModel)
#endif
{
    utility.init();
    cimageInit(image, width, height);
    
    A = {(double)width/2., (double)height/2.};
    B = {(double)width/2., (double)height/2.};
    
    pthread_mutex_init(&predictions_mutex, NULL);
}

Tau::~Tau() {}

void Tau::init( void )
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

void Tau::trigger( void )
{
    double p = 0.;

    pthread_mutex_lock(&utility.outframe_mutex);
    utility.trigger();
    p = perform( utility.outimage );
    pthread_mutex_unlock(&utility.outframe_mutex);
    
    LOG_TAU("Tau perform: %.3fs\n", p);
    if(count < MAX_COUNT)
    {
        double pacc = accuracy;
        cma(p, &avg, ++count);
        stddev_sum += pacc;
        cma(current_accuracy, &accuracy, ++accuracy_count);
        if(accuracy_count > AVERAGE_COUNT) accuracy_count--;
    }
#ifdef __PSM__
    rho_drawer.DrawDetectionMap( &rho.core.DetectionMap );
#endif
//    rho.core.Thresh-=1; ///TEST
    rho.core.ThreshByte = (byte_t)rho.core.Thresh;
    if(!(rho.core.ThreshByte % THRESH_FRAME_PRINT_STEP))
    {
        imwrite(FRAME_SAVE_ROOT + string("thresh_frame_") + to_string(rho.core.ThreshByte) + string(".png"), utility.outframe);
    }
}

std::string Tau::serialize( void )
{
    return "";
}

double Tau::perform( cimage_t &img )
{
    bool background_event = !!( ++tick >= BACKGROUNDING_PERIOD );
    
    if( background_event )
    {
        utility.requestBackground();
        LOG_TAU(DEBUG_2, "Waiting for utility to generate background...\n");
        
        while(!utility.background_ready)
            utility.trigger();
        
        LOG_RHO(DEBUG_2, "Background ready.\n");
        rho.backgrounding_event = true;
    }
    
    double p = rho.perform( img, &packet );
    
    if( background_event )
    {
        utility.background_ready = false;
        tick = 0;
    }
    else
    {
        updateThresh();
        updatePrediction();
    }
    
    double Cx = utility.pCx-rho.core.Cx,
    Cy = utility.pCy-rho.core.Cy;
    current_accuracy = sqrt(Cx*Cx + Cy*Cy);
    
    printPacket(&packet, 4);
    return p;
}

void Tau::updateThresh()
{
    utility.thresh = rho.core.Thresh;
}

void Tau::updatePrediction()
{
    Point2f a(packet.py, packet.px),
            b(packet.sy, packet.sx);
    
    if( a.x > b.x ) swap(a,b);
#ifdef FISHEYE
    invfisheye(&a, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
    invfisheye(&b, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
#endif
    A = { a.x, a.y };
    B = { b.x, b.y };
}

void Tau::printPacket( GlobalPacket * p, int l )
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
