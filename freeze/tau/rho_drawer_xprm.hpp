//
//  rho_drawer_xprm.hpp
//  pcrxprm
//
//  Created by Matthew Fonken on 2/5/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef rho_drawer_xprm_hpp
#define rho_drawer_xprm_hpp

#include "rho_master.h"
#include "opencv_master.h"

#define DETECTION_ELEMENT_RADIUS    4

#define DETECTION_MAP_FRAME_OWIDTH  500
#define DETECTION_MAP_FRAME_OHEIGHT 500
#define DETECTION_MAP_INSET         15
#define DETECTION_MAP_FRAME_IWIDTH  DETECTION_MAP_FRAME_OWIDTH-DETECTION_MAP_INSET+DETECTION_ELEMENT_RADIUS
#define DETECTION_MAP_FRAME_IHEIGHT DETECTION_MAP_FRAME_OHEIGHT-DETECTION_MAP_INSET+DETECTION_ELEMENT_RADIUS

#define MAX_DETECTION_MAP_DENSITTY_VALUE 50
#define MAX_DETECTION_MAP_THRESH_VALUE (1 << 8)

#define DETECTION_MAP_INTERVAL  10

using namespace cv;

class RhoDrawer
{
    uint16_t counter = 0;
    Mat detection_map_frame;
public:
    RhoDrawer();

    void DrawDetectionMap( detection_map_t * );
    
    Mat& GetDetectionMapFrame() {return detection_map_frame;}
};


#endif /* rho_drawer_xprm_hpp */
