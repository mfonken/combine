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

#include "combine.hpp"
#include "environment_master.h"
#include "sercom_wrapper.hpp"

#define MAX_THREADS 3
#define MAX_RATE 200
#define OUT_FPS  60
#define OUT_UDL  1000000 / OUT_FPS

#define MAX_EVENTS 3

using namespace std;

class TestInterface
{
public:
    virtual void  init( void ) = 0;
    virtual void  trigger( void ) = 0;
    virtual string serialize( void ) = 0;
};

struct Event
{
public:
    int id,
        rate;
    class SERCOM sercom;
    pthread_t * thread;
    pthread_mutex_t * mutex;
    Combine test;
    
    static void *eventThread(void * data);
};

class EventList
{
private:
    Event   list[MAX_EVENTS];
    
public:
    int     length;
    pthread_t threads[MAX_EVENTS];
    pthread_mutex_t * mutex;
    
    EventList();
    int     addEvent(Event);
    Event   getEvent(int);
};

typedef enum
{
    INITIALIZED = 0,
    LIVE,
    PAUSED,
    ERROR
} ENV_STATUS;

class Environment
{
    ENV_STATUS status;
    pthread_mutex_t lock;
    pthread_cond_t condition;
    
//    void * internalControl( void * );
    
    void controlTick();
public:
    EventList events;
    pthread_t thread;
    
    Environment();
    Environment( Combine, class SERCOM, int );
    ~Environment();
    
    void addTest( Combine, class SERCOM, int );
    
    void start();
    void pause();
    void resume();
};

#endif /* environment_hpp */
