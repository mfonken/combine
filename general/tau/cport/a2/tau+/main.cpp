#include <stdio.h>
#include <stdint.h>

#include "utility_master.h"
#include "environment_master.h"
#include "tau_master.h"
#include "combine_master.h"

using namespace cv;
using namespace std;

#define FPS         10
#define KEY_DELAY   1000/FPS

int main( int argc, const char * argv[] )
{
#ifdef HAS_CAMERA
    ImageUtility utility("ImageUtility");
#else
    ImageUtility utility("ImageUtility", "frames/small", 26, FNL_RESIZE_W, FNL_RESIZE_H);
#endif
    Tau tau("Tau", &utility, FNL_RESIZE_W, FNL_RESIZE_H);
//    Combine combine("Combine", &tau, FNL_RESIZE_W, FNL_RESIZE_H);
//    SerialWriter comm(SFILE, FILENAME);
    
    Environment env(&utility, 15);
    env.addTest( (TestInterface *)&tau, 30);
//    env.addTest(&combine, &comm, 20);
    
    env.start();
    usleep(1000000);
    env.pause();
    
    pthread_mutex_lock(&utility.outframe_mutex);
    TauDraw drawer(&tau, utility.outframe);
    pthread_mutex_unlock(&utility.outframe_mutex);
    
    usleep(1000000);
    env.resume();
    
    while(1)
    {
        pthread_mutex_lock(&utility.outframe_mutex);
        drawer.drawDensitiesOnFrame(utility.outframe);
        imshow("Outframe",  drawer.frame);
        imshow("Rho X Map", drawer.RMX);
        imshow("Rho Y Map", drawer.RMY);
        drawer.drawKalmans();
        pthread_mutex_unlock(&utility.outframe_mutex);
        
        char c = waitKey(KEY_DELAY);
        switch(c)
        {
            case ' ':
                if(env.status != LIVE) env.resume();
                else env.pause();
                break;
            default:
                if(utility.loop(c))
                {
                    utility.trigger();
                    tau.trigger();
                }
                break;
            case 's':
                env.pause();
                usleep(10000);
                tau.avg = 0;
                tau.count = 0;
                env.start();
                usleep(10000000);
                env.pause();
                printf("Tau averaged %fms for %d iterations\n", tau.avg*1000, tau.count);
                break;
        }
    }
    return 0;
}
