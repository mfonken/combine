//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#include "utility_master.h"
#include "environment_master.h"
#include "tau_master.h"
#include "combine.hpp"

using namespace cv;
using namespace std;

#define MAIN_FPS        10
#define UTILITY_FPS     15
#define TAU_FPS         15
#define COMBINE_FPS     30

#define DRAW
#define KEY_DELAY   1000/MAIN_FPS

#define PAUSE_ENVIRONMENT_KEY   ' '
#define PERFORMANCE_TEST_KEY    'p'
#define REQUEST_IMU_OFFSET_KEY  'c'
#define NUM_ENV_EVENTS           3

int main( int argc, const char * argv[] )
{
#ifdef HAS_CAMERA
    ImageUtility utility("ImageUtility");
#else
    ImageUtility utility("ImageUtility", "frames/small", 26, FNL_RESIZE_W, FNL_RESIZE_H);
#endif
    Tau tau("Tau", &utility, FNL_RESIZE_W, FNL_RESIZE_H);
    Combine combine("Combine", &tau, FNL_RESIZE_W, FNL_RESIZE_H);
    SerialWriter comm(SFILE, DATA_FILENAME), ratefile(SFILE, RATES_FILENAME);
    
    Environment env(&utility, UTILITY_FPS);
    env.addTest(&tau, TAU_FPS);
    env.addTest(&combine, &comm, COMBINE_FPS);
    
    env.start();
    usleep(50000);
    combine.request();
    env.pause();
    
    pthread_mutex_lock(&utility.outframe_mutex);
    TauDraw drawer(&tau, utility.outframe);
    pthread_mutex_unlock(&utility.outframe_mutex);
    
    usleep(500000);
    env.resume();
    
    while(1)
    {
#ifdef DRAW
        pthread_mutex_lock(&utility.outframe_mutex);
        drawer.draw(utility.outframe);
        imshow("Outframe", drawer.frame);
#ifdef DRAW_RHO_MAPS
        imshow("Rho X Map", drawer.RMX);
        imshow("Rho Y Map", drawer.RMY);
#endif
        pthread_mutex_unlock(&utility.outframe_mutex);
#endif

        /* Send rates to file */
        int rates[NUM_ENV_EVENTS];
        for(int i = 0; i < NUM_ENV_EVENTS; i++ )
            rates[i] = env.events.get(i)->rate();
        char rates_packet[255] = { ' ' };
        sprintf(rates_packet, "%d, %d, %d\r\n", rates[0], rates[1], rates[2] );
        ratefile.write(rates_packet);
        
        char c = waitKey(KEY_DELAY);
        switch(c)
        {
            case PAUSE_ENVIRONMENT_KEY:
                if(env.status != LIVE) env.resume();
                else env.pause();
                break;
            case PERFORMANCE_TEST_KEY:
                env.pause();
                usleep(10000);
                tau.avg = 0;
                tau.count = 0;
                env.start();
                usleep(10000000);
                env.pause();
                printf("Tau averaged %fms for %d iterations\n", tau.avg*1000, tau.count);
                break;
            case REQUEST_IMU_OFFSET_KEY:
                printf("Sending imu offset request\n");
                combine.request();
                break;
            default:
                if(utility.loop(c))
                {
                    utility.trigger();
                    tau.trigger();
                    combine.trigger();
                }
                break;
        }
    }
    return 0;
}
