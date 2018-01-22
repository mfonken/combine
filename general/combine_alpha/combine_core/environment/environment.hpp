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

#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "environment_master.h"
#include "sercom_wrapper.hpp"

#define MAX_THREADS 3
#define MAX_RATE 200
#define OUT_FPS  60
#define OUT_UDL  1000000 / OUT_FPS

using namespace std;

class Test
{
public:
    virtual void  init( void ) { };
    virtual void  trigger( void ) { };
    virtual string serialize( void ) { return NULL; };
};

class ThreadList
{
public:
    pthread_t list[MAX_THREADS];
    int       rate[MAX_THREADS];
    int       id[MAX_THREADS];
    int       num;
};

typedef struct
{
    int id,
        rate;
    class SERCOM sercom;
    Test test;
} event_t;

class Environment
{
public:
    ThreadList  threads;
    pthread_mutex_t lock;
    
    event_t *events;
    int num_events;

    bool live;

    Environment( Test, class SERCOM, int );
    ~Environment();
    void start();
};

#endif /* environment_hpp */
