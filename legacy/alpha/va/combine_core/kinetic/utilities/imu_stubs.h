//
//  imu_stubs.hpp
//  combine_core
//
//  Created by Matthew Fonken on 11/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef imu_stubs_h
#define imu_stubs_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
    
void tokenifyPacket( char * a, int l, int n, char s, double * d);
char** str_split(char* a_str, const char a_delim);
    
#ifdef __cplusplus
}
#endif
        
#endif /* imu_stubs_h */
