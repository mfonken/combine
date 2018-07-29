#include <stdio.h>
#include <stdint.h>

#include "environment_master.h"
#include "utility_master.h"
#include "tau_master.h"
#include "combine_master.h"

using namespace cv;
using namespace std;

#define MAIN_FPS        60
#define TAU_FPS         60
#define COMBINE_FPS     0

#define sleep(X) usleep(X*1E6)
#define KEY_DELAY   1E3/MAIN_FPS

int main( int argc, const char * argv[] )
{
    Tau tau("Tau", FNL_RESIZE_W, FNL_RESIZE_H
#ifndef HAS_CAMERA
, "ImageUtility", IMAGE_SOURCE_PATH, IMAGE_SOURCE_NUM_FRAMES
#endif
    );
    Environment env(&tau, TAU_FPS);
    
#ifdef HAS_IMU
    Combine combine("Combine", &tau, FNL_RESIZE_W, FNL_RESIZE_H);
    SerialWriter comm(SFILE, FILENAME);
    env.addTest(&combine, &comm, COMBINE_FPS);
#endif

    env.pause();
    
    TauDraw drawer(&tau, tau.utility.outframe);
    
    sleep(0.1);
    env.resume();
    
    Mat probabilities(Size(800, 200), CV_8UC3, Scalar(245,245,245));
    
    while(1)
    {
//        imshow(TITLE_STRING, tau.utility.outframe);
        
        pthread_mutex_lock(&tau.utility.outframe_mutex);
        pthread_mutex_lock(&tau.rho.c_mutex);
        drawer.drawDensitiesOnFrame(tau.utility.outframe);
        pthread_mutex_unlock(&tau.rho.c_mutex);
        pthread_mutex_unlock(&tau.utility.outframe_mutex);
        imshow(TITLE_STRING,  drawer.frame);
        drawer.drawRhoProbabilities(probabilities);
        imshow("Probabilities", probabilities);
        
        char c = waitKey(KEY_DELAY);
        switch(c)
        {
            case ' ':
                if(env.status != LIVE) env.resume();
                else env.pause();
                break;
            default:
                if(tau.utility.loop(c))
                {
                    tau.utility.trigger();
                    tau.trigger();
                }
                break;
            case 's':
                env.pause();
                sleep(0.01);
                tau.avg = 0;
                tau.count = 0;
                env.start();
                sleep(10);
                env.pause();
                printf("%d\n", tau.count);
//                printf("Tau averaged %fms for %d iterations\n", tau.avg*1000, tau.count);
                break;
        }
    }
    return 0;
}

