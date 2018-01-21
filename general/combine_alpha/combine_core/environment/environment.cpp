//
//  environment.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "environment.hpp"

#define TILT_LINES

using namespace cv;

Environment::Environment( int argc, char * argv[] ) : utility(0,NULL)
{
    utility = ImageUtility( argc, argv );
}

int Environment::addThread( void *(*start_routine) (void *) )
{
    int id = pthread_create(&threads.list[0], NULL, start_routine, &data);
    if (id) {
        cout << "Error:unable to create thread: " << id << endl;
        exit(-1);
    }
    return id;
}

void Environment::start()
{
    printf("Starting Combine Core\n");
    
    printf("Initializing Image Utility.\n");
    
#ifdef ITERATIONS
    for(int o = 0; o < num_orders; o++)
    {
        int iterations = orders[o];
        double times[iterations];
        printf("Running for %d iterations.\n", iterations);
        for(int l=0;l<iterations;l++) {
#else
            printf("Running with User Control.\n");
            for(int l=0;l<1;)
            {
#endif
#ifdef TIME_FULL_LOOP
                gettimeofday( &start, NULL);
#endif
                /* Re-init frame and out images every loop */
                Mat frame;
                frame = utility.getNextFrame();
                
                /* Get Beacons */
                utility.getBeacons(frame);
                /***************/
                
#ifdef TIME_FULL_LOOP
                gettimeofday( &stop, NULL);
                printf("Total loop time is %f (s)\n", timeDiff(start, stop));
#endif
                
#ifdef SHOW_IMAGES
                utility.loop(waitKey(utility.isLive()?FRAME_DELAY_MS:60000));
#else
                utility.loop(' ');
#endif
#ifdef ITERATIONS
                long double average = 0;
                for(int l = 0; l < iterations; l++) average += times[l];
                average /= getBeaconsiterations;
                printf("L-%d A-%Lf\n", iterations, average);
            }
#else
        }
#endif
        printf("Ending Combine Core.\n");
    }

