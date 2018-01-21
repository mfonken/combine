//
//  environment.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef environment_hpp
#define environment_hpp

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "open_master.h"
#include "kinetic_master.h"

#define MAX_THREADS 3
#define MAX_BUFFER 256
#define OUT_FPS  60
#define OUT_UDL  1000000 / OUT_FPS

#ifdef TIME_FULL_LOOP
struct timeval start,stop;
#endif

class ThreadList
{
public:
    pthread_t list[MAX_THREADS];
    int       num;
};

typedef struct _environment_t
{
    kpoint_t  bea[2];
    imu_t     bno;
    kinetic_t kin;
    int     width,
            height;
} environment_t;

class Environment
{
public:
    environment_t data;
    
    ThreadList threads;
    ImageUtility utility;
    
    Environment( int, char *[] );
    int addThread(void *(*start_routine) (void *));
    void start();
};


void * IMU_THREAD( void *data );

#endif /* environment_hpp */
