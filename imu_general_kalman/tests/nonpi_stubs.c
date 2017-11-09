#include "nonpi_stubs.h"

int open(const char * c, int i){return nil;}
int close(int i){return nil;}

int read(int a, unsigned char   *b, int c){return nil;}
int write(int a, unsigned char*b, int c){return nil;}
int i2c_smbus_read_byte_data(int a, int b){return nil;}

int ioctl(int a, int b, int c){return nil;}
