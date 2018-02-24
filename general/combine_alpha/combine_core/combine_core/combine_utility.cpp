//
//  combine_utility.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "combine_utility.hpp"

#define FRAME_DELAY_MS 10

using namespace cv;

void * CombineUtility::main( void * data )
{
    printf("Starting Combine Core\n");
    for(int l=0;l<1;)
    {
//        utility.getBeacons(utility.getNextFrame());
//        utility.loop(waitKey(utility.isLive()?FRAME_DELAY_MS:60000));
    }
    printf("Ending Combine Core.\n");
    return NULL;
}

void * CombineUtility::performance( void * data )
{
    struct timeval start, stop;
    int num_orders = 3;
    int orders[] = {10, 100, 500};
    printf("Starting Combine Core Performance Test.\n");
    
    for(int o = 0; o < num_orders; o++)
    {
        int iterations = orders[o];
        double times[iterations];
        printf("Running for %d iterations.\n", iterations);
        for(int l=0;l<iterations;l++)
        {
            gettimeofday( &start, NULL);
            
            gettimeofday( &stop, NULL);
//            printf("Total loop time is %f (s)\n", timeDiff(start, stop));
            
            long double average = 0;
            for(int l = 0; l < iterations; l++) average += times[l];
            average /= iterations;
            printf("L-%d A-%Lf\n", iterations, average);
        }
    }
    printf("Ending Combine Core Performance Test.\n");
    return NULL;
}

