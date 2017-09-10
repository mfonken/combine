//
//  test_stubs.c
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>


#define nil         0x00
#define O_RDWR      nil
#define I2C_SLAVE   nil

int open(const char * c, int i){return nil;}
int close(int i){return nil;}
int exit(int i){return nil;}

int read(int a, unsigned char   *b, int c){return nil;}
int write(int a, unsigned char*b, int c){return nil;}

int ioctl(int a, int b, int c){return nil;}
