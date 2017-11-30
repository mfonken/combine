//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <sys/time.h>
#include <pthread.h>

#include "tau+/test_setup.h"
#include "tau+/global_types.h"
#include "tau+/tau_test.hpp"
#include "tau+/statem.h"
#include "tau+/tau.h"
#include "tau+/image_utility.hpp"
#include "kinetic/kinetic.h"

#define RAD_TO_DEG (180 / M_PI)
#define NUM_THREADS 1
#define SCALE 100

using namespace cv;
using namespace std;

int orders[] = { 10, 100, 500 };//1000, 10000, 100000 };
int num_orders = sizeof(orders)/sizeof(orders[0]);
double t[3];

#ifdef TIME_FULL_LOOP
struct timeval start,stop;
#endif

cartesian2_t bea[2];

LSM9DS1_t lsm;
kinetic_t kin;
tau_t tau;

void * IMU_THREAD( void *data ) //LSM9DS1_t * lsm, kinetic_t * kin )
{
    printf("Initializing IMU.\n");
    IMU_Init( &lsm );
    
    printf("Initializing Kinetic Utility.\n");
    Kinetic_Init( &lsm, &kin );
    
    int roll, pitch, yaw;
    while(1)
    {
        Kinetic_Update_Rotation( &lsm, &kin );
        kin.rotation[2] += M_PI/2;
        roll    = (int)( kin.rotation[0] * RAD_TO_DEG );
        pitch   = (int)( kin.rotation[1] * RAD_TO_DEG );
        yaw     = (int)( kin.rotation[2] * RAD_TO_DEG );
//        printf("[R] %4d\t [P] %4d\t [Y] %4d\n", roll, pitch, yaw);
        
        Kinetic_Update_Position( &lsm, &kin, bea );
    }
}

int main( int argc, char * argv[] )
{
    printf("Starting Combine Core\n");
    
#ifdef IMU_DEBUG
    pthread_t threads[NUM_THREADS];
    printf("Starting IMU thread.\n");
    int t1 = pthread_create(&threads[0], NULL, &IMU_THREAD, NULL);
    if (t1) {
        cout << "Error:unable to create IMU thread," << t1 << endl;
        exit(-1);
    }
#endif
    int threshold = THRESHOLD;
    
    printf("Initializing Image Utility.\n");
    image_test util( argc, argv);
    int width = util.getWidth();
    int height = util.getHeight();
    pixel_base_t pixels[width*height];
    
    printf("Initializing Tau Utility.\n");
    initTauA(&tau, width, height );
    
#ifdef ITERATIONS
    for(int o = 0; o < num_orders; o++)
    {
        int iterations = orders[o];
        double times[iterations];
        printf("Running for %d iterations\n", iterations);
        for(int l=0;l<iterations;l++) {
#else
            printf("Running with User Control\n");
            for(int l=0;l<1;)
            {
#endif
                
#ifdef TIME_FULL_LOOP
                gettimeofday( &start, NULL);
#endif
                /* Re-init frame and out images every loop */
                Mat frame, out(height, width, CV_8UC3, Scalar(0,0,0));
                frame = util.getNextFrame();
//                imshow("Frame", frame);
                
                /* Convert image to filtered pixel array and back to black and white image */
                MatToCimage( width, height, frame, pixels, threshold );
                
                /* Run Tau */
                performTauA(&tau, t, pixels);
                
                /* Update threshold */
                updateThreshold(&threshold, stateNumber(tau.sys.state));
                
                /* Reconstruct Position */

                if(tau.sys.state == STABLE_DOUBLE)// && 0)
                {
                    double a,b,c;
//                    a = kin.rotation[0];
//                    b = kin.rotation[1];
//                    c = kin.rotation[2];
//                    printf("[R] %.4f  [P] %.4f  [Y] %.4f (º)\n", a * RAD_TO_DEG, b * RAD_TO_DEG, c * RAD_TO_DEG - 90);
                    
                    a = kin.position[0];
                    b = kin.position[1];
                    c = kin.position[2];
                    printf("[X] %.4f  [Y] %.4f  [Z] %.4f (%s)\n", a * SCALE, b * SCALE, c * SCALE, UNITS);
                }
                /*************************/
                
#ifdef MAIN_DEBUG
                printf("Threshold for state %s is %d. - (%d)\n", stateString(sys.state), threshold, n );
#endif
                
#ifdef STATEM_DEBUG
                printBayesianMap(&tau.sys.probabilities, tau.sys.state);
#endif
                
#ifdef ITERATIONS
                times[l] = t[0]+t[1]+t[2];
#endif
#ifdef SHOW_IMAGES
                double a[] = { 0.0, t[0], t[0] + t[1], t[0] + t[1] + t[2]};
                drawTiming(a, sizeof(a)/sizeof(a[0]));
                cimageToBandW( width, height, pixels, out );
                cv::putText(out, "FPS:" + to_string(1/a[3]), Point(3, 18), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 1, 8);
                drawTau(out, &tau.rho.density_map_pair, &tau.rho.peak_list_pair, &tau.predictions);
#endif
                
#ifdef TIME_FULL_LOOP
                gettimeofday( &stop, NULL);
                printf("Total loop time is %f (s)\n", timeDiff(start, stop));
#endif
                
#ifdef SHOW_IMAGES
                util.loop(waitKey(util.isLive()?FRAME_DELAY_MS:60000));
#else
                util.loop(' ');
#endif

            
#ifdef ITERATIONS
            long double average = 0;
            for(int l = 0; l < iterations; l++) average += times[l];
            average /= iterations;
            printf("L-%d A-%Lf\n", iterations, average);
        }
#else
    }
#endif
//    pthread_exit(NULL);
        return 0;
}

