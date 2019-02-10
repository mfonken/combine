//
//  rho_drawer_xprm.cpp
//  pcrxprm
//
//  Created by Matthew Fonken on 2/5/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#include "rho_drawer_xprm.hpp"

//#define PRINT_DETECTION_MAP

#define NUM_COLORS 4
Vec3b rcolors[NUM_COLORS] =
{
    {255, 0, 0},
    {100, 150, 0},
    {0, 15, 255},
    {0, 150, 100},
},
rwhite = {255,255,255};

RhoDrawer::RhoDrawer()
: detection_map_frame(Size(DETECTION_MAP_FRAME_OWIDTH, DETECTION_MAP_FRAME_OHEIGHT), CV_8UC3, Scalar(0,0,0))
{}

void RhoDrawer::DrawDetectionMap( detection_map_t * map )
{
    if(++counter == DETECTION_MAP_INTERVAL)
    {
        detection_map_frame = {0};
        rectangle(detection_map_frame, Point(DETECTION_MAP_INSET,DETECTION_MAP_INSET), Point(DETECTION_MAP_FRAME_OWIDTH-DETECTION_MAP_INSET, DETECTION_MAP_FRAME_OHEIGHT-DETECTION_MAP_INSET), rwhite);
        counter = 0;
        if( !map->length ) return;
#ifdef PRINT_DETECTION_MAP
        printf("Detection Map:");
        if( !map->fill ) printf(" Empty\n");
        else printf("\n");
#endif
        uint8_t i = map->first, c = 0;
        for( ; c < map->fill; c++, i = c % map->length)
        {
            detection_element_t * e = &map->buffer[i];
#ifdef PRINT_DETECTION_MAP
            printf(" - [%2d](%3d, %3d, %d)\n", c, e->thresh, e->density, e->tracking_id);
#endif
            int x = ((double)e->density / MAX_DETECTION_MAP_DENSITTY_VALUE * DETECTION_MAP_FRAME_IWIDTH) + DETECTION_MAP_INSET;
            int y = (( 1 - (double)e->thresh / MAX_DETECTION_MAP_THRESH_VALUE) * DETECTION_MAP_FRAME_IHEIGHT) + DETECTION_MAP_INSET*2;
            
            Vec3b color = rwhite;
            if( e->tracking_id < NUM_COLORS) color = rcolors[e->tracking_id];
            circle(detection_map_frame, Point(x,y), DETECTION_ELEMENT_RADIUS, color, FILLED);
        }
    }
}
