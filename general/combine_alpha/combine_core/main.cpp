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
    ImageUtility utility("ImageUtil");
    Combine combine("Combine", &utility);
    SerialWriter comm(SFILE, FILENAME);
    
    Environment env(&combine, &comm, MAX_FPS);
    env.addTest(&utility, MAX_FPS);
    
    env.start();
    usleep(1000000);
    env.pause();
    
    Mat frame;
    while(1)
    {
        imshow("Live Camera", utility.outframe);
        if( cv::waitKey(10) == ' ' )
        {
            if(env.status != LIVE)
                env.resume();
            else
                env.pause();
        }
    }
    return 0;
}
