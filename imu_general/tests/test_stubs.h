//
//  test_stubs.c
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#ifndef test_stubs_h
#define test_stubs_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/time.h>

#define nil         0x00

#define CAMERA_OFFSET_ANGLE_X 0
#define CAMERA_OFFSET_ANGLE_Y 0
#define CAMERA_OFFSET_ANGLE_Z 0

#define CAMERA_ALPHA_H 120
#define CAMERA_ALPHA_W 150

#define CAMERA_WIDTH  1280
#define CAMERA_HEIGHT 800

/*
int open(const char * c, int i);
int close(int i);
int exit(int i);

int read(int a, unsigned char   *b, int c);
int write(int a, unsigned char*b, int c);

int ioctl(int a, int b, int c);
*/

void Print_Char(char);
void Print_Line(const char *);
void Print_String(const char *);
void Print_Double_Ascii(double);

double timestamp(void);

#ifdef __cplusplus
}
#endif

#endif