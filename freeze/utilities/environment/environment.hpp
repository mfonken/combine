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
//#define LOCK2(m1, m2) Lock _tmp_lock1(m1); Lock _tmp_lock2(m2);

using namespace std;

static inline long getTime( struct timeval tv )
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

class TestInterface
{
public:
    int id;
    const char * name;
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
    int id,
        rate;
    pthread_t       thread;
    pthread_mutex_t * mutex;
    SerialWriter    * sercom;
    TestInterface   * test;
    
    Event(int);
    Event( pthread_mutex_t*, TestInterface*, SerialWriter*, int );
    static void     * worker(void * data);
};

class EventList
{
private:
    int     index;
    Event * list[MAX_EVENTS];
public:
    
    EventList();
    int add( pthread_mutex_t*, TestInterface*, SerialWriter*, int );
    int validIndex(int, int);
    Event * get(int);
    int remove(int);
    int length();
};

class Environment
{
private:
    pthread_mutex_t lock;
    pthread_cond_t  condition;
    string name;
    
    void controlTick();
public:
    ENV_STATUS status;
    EventList events;
    pthread_t thread;
    
    Environment( string );
    Environment( string, TestInterface*, int );
    Environment( string, TestInterface*, SerialWriter*, int );
    ~Environment();
    
    void addTest( TestInterface*, int );
    void addTest( TestInterface*, SerialWriter*, int );
    
    void start();
    void pause();
    void resume();
};

#endif /* environment_hpp */
