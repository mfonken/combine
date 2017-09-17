//
//  test_stubs.c
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include <sys/time.h>

#define nil         0x00

/*
int open(const char * c, int i){return nil;}
int close(int i){return nil;}
int exit(int i){return nil;}

int read(int a, unsigned char   *b, int c){return nil;}
int write(int a, unsigned char*b, int c){return nil;}

int ioctl(int a, int b, int c){return nil;}
*/

void Print_Char(char c) {printf("%c",c);}
void Print_Line(const char * l) {printf("%s\r\n", l);}
void Print_String(const char * s) {printf("%s",s);}
void Print_Double_Ascii(double d) {printf("%.2f",d);}


double timestamp(void)
{
	struct timeval stamp;
	gettimeofday(&stamp, NULL);
	return stamp.tv_sec + stamp.tv_usec/1000000.0;
}
