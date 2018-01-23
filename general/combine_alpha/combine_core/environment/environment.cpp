//
//  environment.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "environment.hpp"

Environment::Environment()
{
    if (pthread_mutex_init(&lock, NULL) != 0) printf("\n mutex init failed\n");
    events.mutex = &lock;
    status = INITIALIZED;
}

Environment::Environment( Combine test, class SERCOM sercom, int rate )
{
    if (pthread_mutex_init(&lock, NULL) != 0) printf("\n mutex init failed\n");
    
    addTest(test, sercom, rate);
    status = INITIALIZED;
}

void Environment::addTest( Combine test, class SERCOM sercom, int rate )
{
    Event e = {events.length, rate, sercom, &events.threads[events.length], &lock, test};
    e.test.init();
    if(events.addEvent(e) == 0) events.addEvent(e);
}

Environment::~Environment()
{
    pthread_mutex_destroy(&lock);
}

void Environment::start()
{
    usleep(1000000);
    resume();
}

void Environment::pause()
{
    pthread_mutex_unlock(&lock);
    status = PAUSED;
}

void Environment::resume()
{
    pthread_mutex_lock(&lock);
    for(int i = 0; i < events.length; i++ )
    {
        Event e = events.getEvent(i);
        pthread_create(&events.threads[i], NULL, &e.eventThread, (void*)&e);
    }
    status = LIVE;
}

void * Event::eventThread( void * data )
{
    Event e = *(Event*)data;
    if( e.mutex == NULL /*|| &e.test == NULL*/) return NULL;
    int sl = 1000000/e.rate;
    while( pthread_mutex_trylock(e.mutex) )
    {
        e.test.trigger();
        e.test.serialize();
        usleep(sl);
    }
    pthread_mutex_unlock(e.mutex);
    return NULL;
}

EventList::EventList()
{
    mutex = NULL;
    length = 0;
}

int EventList::addEvent(Event e)
{
    if( e.mutex == NULL /*|| e.test == NULL*/) return 0;
    
    int id = length;
    if(id == MAX_EVENTS) return -1;
    
    list[id] = e;
    
    if(!e.rate) e.rate = 1;
    length++;
    
    return 1;
}

Event EventList::getEvent(int i)
{
    return list[i];
}

