//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "combine_master.h"
#include "environment_master.h"
#include "A.hpp"

#define COMBINE_FPS 30
#define COMBINE_DEL 1000/COMBINE_FPS

const char * FILENAME = "/Users/matthewfonken/Desktop/out.txt";

int main( int argc, char * argv[] )
{
    Combine combine("Combine");
    SerialWriter comm(SFILE, FILENAME);
    Environment env(&combine, &comm, MAX_FPS);

    
    env.start();
    usleep(1000000);
    env.pause();
    
    Mat frame;
    while(1)
    {
        imshow("Live Camera", combine.utility.outframe);
        char r = cv::waitKey(10);
        
        if( r == ' ' )
        {
            if(env.status != LIVE)
                env.resume();
            else
                env.pause();
        }
    }
    return 0;
}
