//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "combine_master.h"
#include "environment_master.h"
#include "utility_master.h"

#define COMBINE_FPS 30
#define COMBINE_DEL 1000/COMBINE_FPS

const char * FILENAME = "/Users/matthewfonken/Desktop/out.txt";

int main( int argc, char * argv[] )
{
    Combine combine;
    class SERCOM comm(SFILE, FILENAME);
    
    Mat frame;
    while(1)
    {
        combine.trigger();
        comm.write(combine.serialize());
//        imshow("Live Camera", combine.utility.outframe);
        cv::waitKey(1);
    }
    return 0;
}
