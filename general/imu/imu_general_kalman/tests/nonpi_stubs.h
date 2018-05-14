//
//  test_stubs.c
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#ifndef nonpi_stubs_h
#define nonpi_stubs_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define nil         0x00
#define I2C_SLAVE   nil
#define O_RDWR      nil

int open(const char * c, int i);
int close(int i);

int read(int a, unsigned char   *b, int c);
int write(int a, unsigned char*b, int c);
int i2c_smbus_read_byte_data(int a, int b);

int ioctl(int a, int b, int c);

#ifdef __cplusplus
}
#endif

#endif
