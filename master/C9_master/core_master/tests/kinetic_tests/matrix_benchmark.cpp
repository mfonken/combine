//
//  matrix_benchmark.cpp
//  
//
//  Created by Matthew Fonken on 6/21/17.
//
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include "../math_extern.h"
#include "../../ble-module/kinetic/kinetic_types.h"
#include "../../ble-module/kinetic/matrix.c"

using namespace std;

double a[3], b[3],c[3], d[3], r[3];
quaternion_t H_a, H_b, H_c, H_d, H_r;

void test1()
{
    Euler_To_Quaternion(a,&H_a);
    Euler_To_Quaternion(b,&H_b);
    Euler_To_Quaternion(c,&H_c);
    Quaternion_Combine(&H_a, &H_b, &H_c, &H_d);
}

void test2()
{
    mat3x3_t m_a, m_b, m_c, m_d, m_r;
    Euler_To_Rotation_Matrix(a, &m_a);
    Euler_To_Rotation_Matrix(b, &m_b);
    Multiply_Matrix_33x33(&m_a, &m_b, &m_d);
    Euler_To_Rotation_Matrix(c, &m_c);
    Multiply_Matrix_33x33(&m_d, &m_c, &m_r);
    Rotation_Matrix_To_Euler(&m_r, r);
    Euler_To_Quaternion(r,&H_r);
}

void runTest(int iterations)
{
    clock_t s1, e1, s2, e2;
    double d1, d2;
    double t1[iterations];
    double t2[iterations];
    
    
    for(int l = 0; l < iterations; l++ )
    {
        a[0] = rand() % 4*M_PI - 2*M_PI;
        a[1] = rand() % 4*M_PI - 2*M_PI;
        a[2] = rand() % 4*M_PI - 2*M_PI;
        b[0] = rand() % 4*M_PI - 2*M_PI;
        b[1] = rand() % 4*M_PI - 2*M_PI;
        b[2] = rand() % 4*M_PI - 2*M_PI;
        c[0] = rand() % 4*M_PI - 2*M_PI;
        c[1] = rand() % 4*M_PI - 2*M_PI;
        c[2] = rand() % 4*M_PI - 2*M_PI;
        
        s1 = clock();
        test1();
        e1 = clock();
        
        s2 = clock();
        test2();
        e2 = clock();
        
        d1 = ((double)(e1 - s1) / 1000000.0F ) * 1000000;
        t1[l] = d1;
        d2 = ((double)(e2 - s2) / 1000000.0F ) * 1000000;
        t2[l] = d2;
    }

    
    double a1 = 0, a2 = 0;
    for(int l = 0; l < iterations; l++ ) a1 += t1[l];
    a1 /= iterations;
    for(int l = 0; l < iterations; l++ ) a2 += t2[l];
    a2 /= iterations;
    
    cout << "Tested for " << iterations << " iterations:\n\tQ average time is: " << a1 << "ms\n\tR average time is: " << a2 << "ms\n";
}



int main(int argc, char *args[])
{
    double t;
    int iterations = 10;
    if(argc >= 1)
    {
        iterations = atoi(args[1]);
    }
    cout << "Performing benchmark for " << iterations << " iterations." << endl;
    runTest(iterations);
    
    
    return 0;
}
