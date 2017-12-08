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

#include "utilities/sercom.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "open/test_setup.h"
#include "open/global_types.h"
#include "open/image_utility.hpp"
#include "open/open.hpp"
#include "kinetic/kinetic.h"

#define BT_FPS  30
#define MAX_BUFFER 100

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

void * BT_THREAD( void *data )
{
    const char bluetooth_port[] = "/dev/tty.Bluetooth-Incoming-Port";
    const char bluetooth_port_alt[] = "/dev/cu.Bluetooth-Incoming-Port";
    int bluetooth_filestream = -1;
    
    Init_SERCOM(&bluetooth_filestream, bluetooth_port, bluetooth_port_alt);
    
    printf("Waiting for start\n");
    int received = -1;
    while( received <= 0)
    {
        received = Test_SERCOM(bluetooth_filestream);
        printf("Received %d bytes\n", received);
        usleep(1000000);
    }
    
    printf("Sending handshake\n");
    char test[] = "ab\r\n";
    Write_SERCOM_Bytes(bluetooth_filestream, test, 4);
    
    printf("Bluetooth thread started\n");
    
    int bt_sleep = 1000000 / BT_FPS;
    int counter = 1;
    while(1)
    {
        char kin_packet[MAX_BUFFER];
        int l = sprintf(kin_packet, "f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,\r\n",  kin.rotation[0], kin.rotation[1], kin.rotation[2],  kin.position[0] * SCALE, kin.position[1] * SCALE, kin.position[2] * SCALE);
    
        Write_SERCOM_Bytes(bluetooth_filestream, kin_packet, l);
        
//        test[0] = (char)(int)(counter / 10) + 0x30;
//        test[1] = (char)(int)(counter % 10) + 0x30;
//        Write_SERCOM_Bytes(bluetooth_filestream, test, l);
        if(++counter > BT_FPS) counter = 1;
        usleep(bt_sleep);
    }
}

void * IMU_THREAD( void *data )
{
    printf("Initializing IMU.\n");
    IMU_Init( &lsm );

    printf("Initializing Kinetic Utility.\n");
    Kinetic_Init( &lsm, &kin );

    while(1)
    {
        Kinetic_Update_Rotation( &lsm, &kin );
        kin.rotation[2] += M_PI/2;
        Kinetic_Update_Position( &lsm, &kin, bea );
    }
}

int main( int argc, char * argv[] )
{
    printf("Starting Combine Core\n");

    pthread_t threads[NUM_THREADS];
    
    printf("Starting BT thread.\n");
    int t1, t2;
    t1 = pthread_create(&threads[0], NULL, &BT_THREAD, NULL);
    if (t1) {
        cout << "Error:unable to create BT thread," << t1 << endl;
        exit(-1);
    }
    
#ifdef IMU_DEBUG
    printf("Starting IMU thread.\n");
    t2 = pthread_create(&threads[1], NULL, &IMU_THREAD, NULL);
    if (t2) {
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
                
//                if(gkps.size() > 2)      threshold += THRESH_STEP;
//                else if(gkps.size() < 2) threshold -= THRESH_STEP;
                
                /*************************/
#ifdef ITERATIONS
//                times[l] = t[0]+t[1]+t[2];
#endif
#ifdef SHOW_IMAGES
                putText(out, "A", Point(bea[1].x, bea[1].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,255), 3);
                putText(out, "B", Point(bea[0].x, bea[0].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,55), 3);
                imshow("Open Tracker", out);
#endif
                
                double a,b,c;
                a = kin.rotation[0];
                b = kin.rotation[1];
                c = kin.rotation[2];
                printf("[R] %.4f  [P] %.4f  [Y] %.4f (º) | ", a * RAD_TO_DEG, b * RAD_TO_DEG, c * RAD_TO_DEG - 90);
                
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
