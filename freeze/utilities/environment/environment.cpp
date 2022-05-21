//
//  environment.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "environment.hpp"

Environment::Environment( string name ) : Environment(name, NULL, NULL, 0) {}

Environment::Environment( string name, TestInterface * test, double rate ) : Environment(name, test, NULL, rate) {}

Environment::Environment( string name, TestInterface * test, SerialWriter * sercom, double rate )
: ticks_per_sec(1)
{
    this->name = name;
    LOG_ENV(DEBUG_2, "Initializing Test Environment %s.\n", name.c_str());
    if (pthread_mutex_init(&lock, NULL) != 0) {LOG_ENV(DEBUG_2, "\n mutex init failed\n");}
    if(test != NULL) addTest(test, rate);
    status = INITIALIZED;
}

void Environment::addTest( TestInterface * test, double rate)
{
    if(events.add( &lock, test, NULL, rate ))
        test->init();
}

void Environment::addTest( TestInterface* test, SerialWriter* sercom, double rate)
{
    if(events.add( &lock, test, sercom, rate ))
        test->init();
}

Environment::~Environment()
{
    pthread_mutex_destroy(&lock);
}

void Environment::start()
{
    resume();
}

void Environment::pause()
{
    LOG_ENV(DEBUG_2, "Pausing environment\n");
    pthread_mutex_unlock(&lock);
    status = PAUSED;
}

void Environment::resume()
{
    LOG_ENV(DEBUG_2, "Resuming environment\n");
    
    resetTicks();
    pthread_mutex_lock(&lock);
    pthread_create(&thread, NULL, &Environment::startWorker, this);
    
    status = LIVE;
}

int Environment::resetTicks()
{
    if(ticks_per_sec <= 0.0)
        ticks_per_sec = 1.0;
    if(ticks_per_sec < 1.0)
        ticks_per_sec = (int)( 1.0 / ticks_per_sec );
    for( int i = 0; i < events.length(); i++ )
    {
        double r = (double)events.get(i)->rate;
        double div = ticks_per_sec / r;
        if( div != (double)(int)div )
            ticks_per_sec *= r;
    }
    
//    printf("%.1f:\n", ticks_per_sec);
    for( int i = 0; i < events.length(); i++ )
    {
        Event * e = events.get(i);
        e->ticks = ticks_per_sec / e->rate;
//        printf("%d - %d\n", i, e->ticks);
    }
    return ticks_per_sec;
}

void Environment::worker()
{
    double interval_s = 1.0 / ticks_per_sec;
    
    while( pthread_mutex_trylock(&lock) )
    { TIMELOCK(interval_s)
        events.tick();
        ticks++;
//        printf("tick\n");
    }
}

Event::Event( int id )
{
    this->id = id;
}

Event::Event( pthread_mutex_t * mutex, TestInterface * test, SerialWriter * sercom, double rate )
{
    this->id = 0;
    this->mutex = mutex;
    this->test = test;
    this->sercom = sercom;
    this->rate = rate;
}

void * Event::worker( void * data )
{
    Event e = *(Event*)data;
    const char * n = e.test->name;
    if( e.mutex == NULL)
    {
        LOG_ENV(DEBUG_2, "ALERT: Event %s has no mutex!\n", n);
        return NULL;
    }
    
    if( e.rate <= 0)
    {
        LOG_ENV(DEBUG_2, "ALERT: Event %s has invalid rate.\n", n);
        return NULL;
    }
//    int sl = 1000000/e.rate;
    
//    struct timeval time;
//    long curr_time, end_time;
//    while( pthread_mutex_trylock(e.mutex) )
    {
//        end_time = getTime(time) + sl;
    
        e.test->trigger();
        if(e.sercom != NULL)
            e.sercom->write(e.test->serialize());
        
        e.id++;
        
//        while( (curr_time = getTime(time)) < end_time );
    }
//    pthread_mutex_unlock(e.mutex);
//    LOG_ENV(DEBUG_2, "Event %s has triggered %d times\n", e.test->name, e.id);
    return NULL;
}

bool Event::tick()
{
    if(++c == ticks)
    {
        c = 0;
        return true;
    }
    return false;
}

EventList::EventList()
{
    this->num = 0;
    
    for(int i = 0; i < MAX_EVENTS; i++)
    {
        list[i] = (Event *)malloc(sizeof(Event));
    }
}

int EventList::add( pthread_mutex_t* mutex, TestInterface* test, SerialWriter* sercom, double rate )
{
    if( !validIndex(this->num, MAX_EVENTS-1) ) return 0;
    Event * e = get(this->num++);
    if(e == NULL) return 0;
    e->id = this->num-1;
    e->mutex = mutex;
    e->test = test;
    e->sercom = sercom;
    e->rate = rate;
    e->c = 0;
    
    return 1;
}

int EventList::validIndex( int i, int m )
{
    if( i >= m || i < 0 ) return 0;
    return 1;
}

Event * EventList::get(int i)
{
    if( !validIndex(i, num) ) return NULL;
    return list[i];
}
int EventList::remove(int i)
{
    if( !validIndex(i, num)  ) return 0;
    for(; i < num-1; i++)
        list[i] = list[i+1];
    num--;
    return 1;
}

int EventList::length()
{
    return num;
}

void EventList::tick()
{
    for( int i = 0; i < length(); i++ )
    {
        Event * e = get(i);
        if(e->tick())
        {
            e->worker((void*)e);
//            pthread_create(&e->thread, NULL, &e->worker, (void*)e);
            LOG_ENV(DEBUG_0, "Event %d\n", i);
        }
    }
    LOG_ENV(DEBUG_0, "\n");
}
