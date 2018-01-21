//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "environment_master.h"

int main( int argc, char * argv[] )
{
    Environment test( argc, argv );
    
    test.addThread(DATA_WR_THREAD);
    
#ifdef IMU_DEBUG
    test.addThread(IMU_THREAD);
#endif
    
    test.start();
    return 0;
}
