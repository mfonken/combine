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

#include <string>
#include <stdlib.h>
#include <stdint.h>

#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "serial_wrapper.hpp"

#include "unilog.h"

#define ENV_DEBUG

#ifndef LOG
#define LOG(L,...) printf(__VA_ARGS__)
#endif

#ifdef ENV_DEBUG
#define LOG_ENV(L,...) LOG(L, "<Environment> " __VA_ARGS__)
#else
#define LOG_ENV(...)
#endif

#define MAX_THREADS 3
#define MAX_FPS     60
#define OUT_FPS     60
#define OUT_UDL     1000000 / OUT_FPS

#define MAX_EVENTS 6

#define LOCK(m) Lock _tmp_lock(m);
#define TIMELOCK(m) TimeLock _tmp_timelock(m);

using namespace std;

static inline long getusec( struct timeval tv )
{
    gettimeofday(&tv, NULL);
    return long(tv.tv_usec + tv.tv_sec*1000000);
}

class Lock
{
    pthread_mutex_t * _mutex;
public:
    Lock(pthread_mutex_t * mutex)
    {
        _mutex = mutex;
        pthread_mutex_lock(_mutex);
    }
    ~Lock()
    {
        pthread_mutex_unlock(_mutex);
    }
};

class TimeLock
{
    struct timeval _time;
    long _end_time;
    useconds_t _interval_us;
public:
    TimeLock(double lifespan_s, useconds_t interval_us = 10000)
    {
        _interval_us = interval_us;
        _end_time = getusec(_time) + (long)(lifespan_s * 1000000.0);
    }
    ~TimeLock()
    {
        while( getusec(_time) < _end_time )
            usleep(_interval_us);
    }
};

class TestInterface
{
public:
    int id;
    char * name;
    pthread_mutex_t mutex;
    virtual void init( void ) = 0;
    virtual void trigger( void ) = 0;
    virtual string serialize( void ) = 0;
};

typedef enum
{
    INITIALIZED = 0,
    LIVE,
    PAUSED,
    ERROR
} ENV_STATUS;

class Event
{
public:
    int id;
    double rate;
    int ticks;
    int c;
    pthread_t       thread;
    pthread_mutex_t * mutex;
    SerialWriter    * sercom;
    TestInterface   * test;
    
    Event(int);
    Event( pthread_mutex_t*, TestInterface*, SerialWriter*, double );
    static void *worker(void * data);
    bool tick();
};

class EventList
{
private:
    int     num;
    Event * list[MAX_EVENTS];
public:
    EventList();
    int add( pthread_mutex_t*, TestInterface*, SerialWriter*, double );
    int validIndex(int, int);
    Event * get(int);
    int remove(int);
    int length();
    void tick();
};

class Environment
{
private:
    pthread_cond_t  condition;
    string name;
    
    int interval;
    
    static void * startWorker(void * env) { ((Environment *)env)->worker(); return NULL; }

    void worker();
    void controlTick();
    
    double  ticks_per_sec;
public:
    pthread_mutex_t lock;
    int ticks;
    ENV_STATUS status;
    EventList events;
    pthread_t thread;
    
    Environment( string );
    Environment( string, TestInterface*, double );
    Environment( string, TestInterface*, SerialWriter*, double );
    ~Environment();
    
    void addTest( TestInterface*, double );
    void addTest( TestInterface*, SerialWriter*, double );
    
    void start();
    void pause();
    void resume();
    
    int resetTicks();
};

#endif /* environment_hpp */
