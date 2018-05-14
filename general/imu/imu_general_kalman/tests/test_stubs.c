#include "test_stubs.h"

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
