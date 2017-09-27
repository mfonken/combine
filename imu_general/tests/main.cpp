//
//  imu_general_test.cpp
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include <string>
#include "imu_general_test.cpp"

std::string msg;

void TEST_SETUP()
{
    imu_general_setup( msg );
}

void TEST_LOOP()
{
    imu_general_main( msg );
}
           
int main( int argc, char * args[] )
{
    TEST_SETUP();
    
    while(true)
    {
        TEST_LOOP();
    }
    return 0;
}
