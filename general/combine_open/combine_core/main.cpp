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

#include "open/test_setup.h"
#include "open/global_types.h"
#include "open/image_utility.hpp"
#include "open/open.hpp"
#include "kinetic/kinetic.h"

#define RAD_TO_DEG (180 / M_PI)
#define NUM_THREADS 1
#define SCALE 100
#define DRAW_SCALE 7000

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
open_t    tra;

void drawPosition(double x, double y, double z)
{
    int width = 800, act_width = 600, height = 600, step = 50;
    Mat P(height, width, CV_8UC3, Scalar(0,0,0));
    
    const Vec3b white(255,255,255);
    /* Draw Grid */
    for(int x = 0; x <= act_width; x+=step) for(int y = 0; y < act_width; y++) P.at<Vec3b>(y,x) = white;
    for(int y = 0; y < height; y+=step) for(int x = 0; x < act_width; x++) P.at<Vec3b>(y,x) = white;
    
    int HORZ_INSET = 100;
    
    
    /* Draw Beacons */
    Point A(HORZ_INSET, 200), B(HORZ_INSET, height - 200 );
    int cir_rad = 5;
    const Vec3b cir_col(0,0,255);
    circle(P, A, cir_rad, cir_col, 2, 8, 0);
    circle(P, B, cir_rad, cir_col, 2, 8, 0);
    
    line(P, Point(act_width, 0), Point(act_width, height), white, 3, 8, 0);
    for(int y = 0; y < height; y+=step*2) for(int x = act_width; x < width; x++) P.at<Vec3b>(y,x) = white;
    
    /* Draw Given (X,Y) */
    Point C(y * DRAW_SCALE + HORZ_INSET, height/2 + x * DRAW_SCALE);
    circle(P, C, 10, Vec3b(255,100,0), 2, 8, 0);
    
    /* Draw Given Z */
    double thisZ = height/2 - z * DRAW_SCALE;
    line(P, Point(act_width, thisZ), Point(width, thisZ), Vec3b(255,100,0), 3, 10, 0);
    
    imshow("Position", P);
}

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
        printf("[R] %4d\t [P] %4d\t [Y] %4d\n", roll, pitch, yaw);

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

    printf("Initializing Image Utility.\n");
    image_test util( argc, argv);
    int width = util.getWidth();
    int height = util.getHeight();
    
    int threshold = 254;
    
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
#ifdef OPENCV_THRESHOLD
                Mat threshed_frame;
                cv::threshold(frame, threshed_frame, threshold, 255, THRESH_BINARY);
                frame = threshed_frame;
#endif
//                imshow("Frame", frame);

                std::vector<KeyPoint> kps;
                kps = tra.detect(frame, out);

                
                
#define TEXT_OFFSET_X -26
#define TEXT_OFFSET_Y  18
#define DETECT_BORDER_OFFSET 10
                vector<KeyPoint> gkps;
                for(int i = 0; i < kps.size(); i++)
                {
                    Point pt(kps.at(i).pt);
                    if(   pt.x > DETECT_BORDER_OFFSET && pt.x < ( width - DETECT_BORDER_OFFSET)
                       && pt.y > DETECT_BORDER_OFFSET && pt.y < ( height - DETECT_BORDER_OFFSET))
                    {
                        Point opt(pt.x + TEXT_OFFSET_X, pt.y + TEXT_OFFSET_Y);
                        putText(out, "+", opt, FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Vec3b(0,0,255), 3, 8);
                        gkps.push_back(kps.at(i));
                    }
                }
                
//                printf("Found %lu good keypoints\n", gkps.size());
                if(gkps.size() >= 2)
                {
                    if(gkps.at(1).pt.x < gkps.at(0).pt.x)
                    {
                        bea[0].x = gkps.at(1).pt.x;
                        bea[0].y = gkps.at(1).pt.y;
                        bea[1].x = gkps.at(0).pt.x;
                        bea[1].y = gkps.at(0).pt.y;
                    }
                    else
                    {
                        bea[0].x = gkps.at(0).pt.x;
                        bea[0].y = gkps.at(0).pt.y;
                        bea[1].x = gkps.at(1).pt.x;
                        bea[1].y = gkps.at(1).pt.y;
                    }
                }
                
                if(gkps.size() > 2)      threshold += THRESH_STEP;
//                else if(gkps.size() < 2) threshold -= THRESH_STEP;
                
                /*************************/
#ifdef ITERATIONS
//                times[l] = t[0]+t[1]+t[2];
#endif
#ifdef SHOW_IMAGES
                imshow("Open Tracker", out);
#endif

                
                double a,b,c;
//                    a = kin.rotation[0];
//                    b = kin.rotation[1];
//                    c = kin.rotation[2];
//                    printf("[R] %.4f  [P] %.4f  [Y] %.4f (º)\n", a * RAD_TO_DEG, b * RAD_TO_DEG, c * RAD_TO_DEG - 90);
                
                a = kin.position[0];
                b = kin.position[1];
                c = kin.position[2];
                printf("[X] %.4f  [Y] %.4f  [Z] %.4f (%s)\n", a * SCALE, b * SCALE, c * SCALE, UNITS);
                
                drawPosition(a,b,c);

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

    return 0;
}
