//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <sys/time.h>

#include "tau+/test_setup.h"
#include "tau+/global_types.h"
#include "tau+/tau_test.hpp"
#include "tau+/statem.h"
#include "tau+/tau.h"
#include "tau+/image_utility.hpp"
#include "sensors/LSM9DS1_types.h"
#include "sensors/LSM9DS1.h"
#include "kinetic/kinetic.h"

using namespace cv;
using namespace std;

int orders[] = { 10, 100, 500 };//1000, 10000, 100000 };
int num_orders = sizeof(orders)/sizeof(orders[0]);
double t[3];

#ifdef TIME_FULL_LOOP
struct timeval start,stop;
#endif

tau_t tau;

int main( int argc, const char * argv[] )
{
    printf("Starting.\n");
    int threshold = THRESHOLD;
    
    printf("Initializing Image Utility.\n");
    image_test util(argc, argv);
    int width = util.getWidth();
    int height = util.getHeight();
    pixel_base_t pixels[width*height];
    
    printf("Initializing Tau Utility.\n");
    initTauA(&tau, width, height );
    
    printf("Initializing IMU.\n");
    LSM9DS1_t lsm;
    IMU_Init( &lsm );
    
    printf("Initializing Kinetic Utility.\n");
    kinetic_t kin;
    Kinetic_Init( &lsm, &kin );

    
#ifdef ITERATIONS
    for(int o = 0; o < num_orders; o++)
    {
    int iterations = orders[o];
    double times[iterations];
    printf("Running for %d iterations\n", iterations);
    for(int l=0;l<iterations;l++) {
#else
    printf("Running with User Control\n");
    for(int l=0;l<1;) {
#endif
        
#ifdef TIME_FULL_LOOP
        gettimeofday( &start, NULL);
#endif
        
        /* Re-init frame and out images every loop */
        Mat frame, out(height, width, CV_8UC3, Scalar(0,0,0));
        frame = util.getNextFrame();
        imshow("Frame", frame);
        
        /* Convert image to filtered pixel array and back to black and white image */
        MatToCimage( width, height, frame, pixels, threshold );

        /* Run Tau */
        performTauA(&tau, t, pixels);
        
        /* Update threshold */
        updateThreshold(&threshold, stateNumber(tau.sys.state));
        
        cartesian2_t bea[2];
        bea[0].x = tau.predictions.x.primary;
        bea[0].y = tau.predictions.y.primary;
        bea[1].x = tau.predictions.x.primary;
        bea[1].y = tau.predictions.y.primary;
        
        Kinetic_Update_Rotation( &lsm, &kin );
        Kinetic_Update_Position( &lsm, &kin, bea );
        
        /*** Final coordinates ***/
        int final_x, final_y, final_z;
        final_x = kin.truePositionFilter[0].value;
        final_y = kin.truePositionFilter[1].value;
        final_z = kin.truePositionFilter[2].value;
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
    }
        
#ifdef ITERATIONS
    long double average = 0;
        for(int l = 0; l < iterations; l++) average += times[l];
        average /= iterations;
        printf("L-%d A-%Lf\n", iterations, average);
    }
#endif
        
    return 0;
}
