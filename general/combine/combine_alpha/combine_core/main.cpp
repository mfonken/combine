//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

//#include "A.hpp"

#include "combine_master.h"
#include "environment_master.h"

const char * FILENAME = "/Users/matthewfonken/Desktop/out.txt";

int main( int argc, char * argv[] )
{
#ifdef HAS_CAMERA
    ImageUtility utility("ImageUtility");
#else
    ImageUtility utility("ImageUtility", "frames/small", 26, FNL_RESIZE_W, FNL_RESIZE_H);
#endif
    Combine combine("Combine", &utility);
    SerialWriter comm(SFILE, FILENAME);
    
    Environment env(&utility, MAX_FPS);
//    env.addTest(&combine, &comm, MAX_FPS);
    
    env.start();
    usleep(1000000);
    env.pause();
    
    Mat frame;
    while(1)
    {
        imshow("Live Camera", utility.outframe);
        switch(waitKey(10))
        {
            case ' ':
                if(env.status != LIVE) env.resume();
                else env.pause();
                break;
            case 's':
                env.pause();
                usleep(10000);
                utility.tra.avg = 0;
                utility.tra.count = 0;
                env.start();
                usleep(10000000);
                env.pause();
                printf("Open Detect averaged %fms for %d iterations\n", utility.tra.avg*1000, utility.tra.count);
                break;
            default:
                break;
        }
    }
    return 0;
}
