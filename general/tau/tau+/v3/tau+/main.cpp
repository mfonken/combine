//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#include "test_setup.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sys/time.h>

#include "global_types.h"
#include "tau_test.hpp"
#include "statem.h"
#include "tau.h"
#include "rho.h"

#include "image_utility.hpp"

using namespace cv;
using namespace std;

int main( int argc, const char * argv[] )
{
    image_test util(argc, argv);
    
    int width = util.getWidth();
    int height = util.getHeight();
    initTauA( width, height );

    pixel_base_t **pixels;
    initImg(&pixels, width, height);
    
    density_map_pair_t          d;
    peak_list_pair_t            p;
    probability_list_pair_t     r;
    prediction_pair_t           e;
    
    system_t sys;
    initSystem(&sys);
    
    int draw = 4;
    
    for(int l=0;l<1;) {
        
        Mat frame, out(height, width, CV_8UC3, Scalar(0,0,0));
        frame = util.getNextFrame();
        
        MatToCimage( width, height, frame, pixels );
        cimageToBandW( width, height, pixels, out );

        double t[2];
        performTauA(t, pixels, &e);
        
        struct timeval start,stop;
        gettimeofday(&start, NULL);
        updateSystem( &sys, &e );
#ifdef STATEM_DEBUG
        printBayesianMap(&sys.probabilities);
#endif

        updateState( &sys );
        gettimeofday(&stop, NULL);
        
        double a[draw];
        a[0] = 0.0;
        a[1] = t[0];
        a[2] = t[0]+t[1];
        a[3] = a[2] + timeDiff(start, stop);
        
        
        string text = "FPS:" + to_string(1/a[2]);
        cv::putText(out, text, Point(3, 18), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 1, 8);
        
        getRhoData(&d, &p, &r);
        drawTau(out, &d, &p, &r, &e);
        
        drawTiming(a, draw);
        util.loop(waitKey(util.isLive()?FRAME_DELAY_MS:60000));
    }
    return 0;
}
