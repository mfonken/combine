//
//  environment.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/20/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "environment.hpp"

Environment::Environment( Test test, SERCOM sercom, int rate )
{
    live = false;
    threads.num = 0;
    if (pthread_mutex_init(&lock, NULL) != 0) printf("\n mutex init failed\n");
    event_t e = {num_events, rate, sercom, test};
    e.test.init();
//    events[num_events] = malloc(sizeof(e));
    events[num_events++] = e;
}

Environment::~Environment()
{
    pthread_mutex_destroy(&lock);
}

void Environment::start()
{
    int counter = 0;
    live = true;
    
    while(live)
    {
        for(int i = 0; i < num_events; i++ )
        {
            event_t e = events[i];
            if(counter % e.rate == 0)
            {
                e.test.trigger();
                e.sercom.write(e.test.serialize());
            }
        }
        if(++counter >= MAX_RATE) counter = 0;
    }
}

