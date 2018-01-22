//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "combine_master.h"
#include "environment_master.h"
#include "utility_master.h"

int main( int argc, char * argv[] )
{
    Combine combine;
    class SERCOM sercom(USB);
    Environment test( combine, sercom, 60 );
    
    test.start();
    return 0;
}
