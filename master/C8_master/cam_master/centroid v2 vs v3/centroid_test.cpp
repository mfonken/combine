#include <iostream>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <sstream>
#include "centroidv2.h"
#include "centroidv3.h"
#include <sys/time.h>

#define MIN_M  150

#define OUTPUT

const char* INPUT_FILENAME = "images/kitten3.bmp";
const char* OUTPUT_FILENAME = "images/kitten_unfisheyed.bmp";

using namespace std;

void performTest( const char* input_file, const char* output_file, int iterations)
{
    
    double timev2[iterations];
    double timev3[iterations];
    int width = 0;
    int height = 0;
    clock_t t1, t2;
    double diff;
    
    ifstream infile;
    ofstream outfile;
    size_t size = 0;
#ifdef OUTPUT
    cout << "Attempting to open " << INPUT_FILENAME << endl;
#endif
    
    infile.open( input_file, ios::in | ios::binary | ios::ate );
    outfile.open( output_file, ios::out | ios::binary | ios::ate );
    char* data = 0;
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    
    infile.seekg(0);
    infile.seekg(18);
    char width_c[4];
    infile.read(width_c, 4);
    char height_c[4];
    infile.read(height_c, 4);
    std::stringstream wd;
    for(int i = 3 ; i >= 0; i--) wd << hex << ((int)width_c[i] & 0xff);
    wd >> width;
    std::stringstream ht;
    for(int i = 3 ; i >= 0; i--) ht << hex << 0xff - ((int)height_c[i] & 0xff);
    ht >> height;
    height++;
#ifdef OUTPUT
    cout << "Length is " << length << " with dimensions " << width << "x" << height << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
#endif
    
    infile.seekg(54);
    char image[height][width][4];
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            char pixel[4];
            infile.read(pixel, 4);
            
            if(pixel[0] < 0 && pixel[1] < 0 && pixel[2] < 0)
            {
                image[y][x][0] = -1;
                image[y][x][1] = 0;
                image[y][x][2] = 0;
            }
            else
            {
                image[y][x][0] = pixel[0];
                image[y][x][1] = pixel[1];
                image[y][x][2] = pixel[2];
            }
            
            image[y][x][3] = pixel[3];
        }
    }
    
    for(int i = 0; i < iterations; i++)
    {
        for(int v = 0; v < 2; v++)
        {
            if(v)
                resetCentroids();
            else
                resetBlobs();
            infile.seekg(54);
            t1 = clock();
            int num_centroids = 0;
            for(int y = 0; y < height; y++)
            {
                char p[width];
                for(int x = 0; x < width; x++)
                {
                    if( image[y][x][0] < 0)// && image[y][x][1] > 0 && image[y][x][2] > 0)
                    {
                        p[x] = 1;
                    }
                    else
                    {
                        p[x] = 0;
                    }
                }
                if(v)
                    getCentroidsv3(p, width, y);
                else
                    getCentroidsv2(p, width, y);
            }
            
            t2 = clock();
            if(v)
                num_centroids = processCentroids();
            
            diff = ((double)(t2 - t1) / 1000000.0F ) * 1000;
            if(v)
                timev3[i] = diff;
            else
                timev2[i] = diff;
        }
    }
    
    double avgv2 = 0;
    double avgv3 = 0;
    for(int i = 0; i < iterations; i++)
    {
        cmav2(timev2[i], &avgv2, i);
        cmav2(timev3[i], &avgv3, i);
    }
    
    cout << "Centroids v2 performed in an average of " << avgv2 << "ms" << endl;
    cout << "Centroids v3 performed in an average of " << avgv3 << "ms" << endl;
    double d = 0;
    if( avgv3 > avgv2 )
    {
        cout << "\tv2";
        d = avgv3 - avgv2;
    }
    else
    {
        cout << "\tv3";
        d = avgv2 - avgv3;
    }
    cout << " is the fastest on file " << INPUT_FILENAME << " by " << d << "ms." << endl;
}

int main(int argc, char *args[])
{
    double t;
    int iterations = 10;
    if(argc >= 2)
    {
        
        INPUT_FILENAME  = args[1];
        OUTPUT_FILENAME = "images/output_image.bmp";
        if(argc >= 3)
        {
            iterations = atoi(args[2]);
        }
    }
    cout << "Performing test on v2 vs v3 for " << iterations << " iterations." << endl;
    performTest(INPUT_FILENAME, OUTPUT_FILENAME, iterations);
    
    
    return 0;
}
