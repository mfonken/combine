//
//  sercom.hpp
//  combine_core
//
//  Created by Matthew Fonken on 12/7/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef sercom_h
#define sercom_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void Init_SERCOM( int * filestream, const char * port, const char * port_alt);
void Write_SERCOM_Byte( int filestream, char b );
void Write_SERCOM_Bytes( int filestream, const char * a, int l );
int Read_SERCOM_Bytes( int filestream, char * a, int l );
int Read_SERCOM_Byte( int filestream, char * b );
int Test_SERCOM( int filestream );
    
#ifdef __cplusplus
}
#endif
        
#endif /* sercom_hpp */
