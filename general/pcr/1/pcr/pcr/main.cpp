//
//  main.cpp
//  pcr
//
//  Created by Matthew Fonken on 12/15/18.
//  Copyright © 2018 Matthew Fonken. All rights reserved.
//

#include "tau_master.h"

int main( int argc, const char * argv[] )
{
    TauDrawer tau("Tau", FRAME_WIDTH, FRAME_HEIGHT
#ifndef HAS_CAMERA
        , FRAME_IMAGE_IMAGE_SOURCE_PATH, FRAME_IMAGE_SOURCE_NUM_FRAMES
#endif
        );
    Environment env(&tau, TAU_FPS);
    
    env.pause();
    sleep(0.1);
    env.resume();
    
    while(1)
    {
        pthread_mutex_lock(&tau.utility.outframe_mutex);
        imshow(TITLE_STRING, tau.GetDensitiesFrame());
        pthread_mutex_unlock(&tau.utility.outframe_mutex);
        
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
//                printf("%d\n", tau.count);
                printf("Tau averaged %fms for %d iterations\n", tau.avg*1000, tau.count);
                break;
        }
    }
    
    return 0;
}
