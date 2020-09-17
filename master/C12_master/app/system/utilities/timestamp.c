//
//  timestamp.c
//  
//
//  Created by Matthew Fonken on 7/16/20.
//

#include <timestamp.h>

#if __OS__ == MICRIUM && defined(__MICRIUM__)
double TIMESTAMP(void)
{
    return (double)OS.Timestamp() / 1000.0;
}
#else
#include <sys/time.h>
double TIMESTAMP(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}
#endif

double SECONDSSINCE( double check )
{
    return TIMESTAMP() - check;
}

bool ISTIMEDOUT( double check, double time_out )
{
    double diff = SECONDSSINCE(check);
//    printf("delta%f c%f t%f\n", diff, check, time_out);
    return (diff > time_out);
}

time_functions Time =
{
    .Stamp = TIMESTAMP,
    .SecondsSince = SECONDSSINCE,
    .IsTimedOut = ISTIMEDOUT
};
