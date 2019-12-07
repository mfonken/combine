//
//  rho_drawer_xprm.hpp
//  pcrxprm
//
//  Created by Matthew Fonken on 2/5/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef rho_drawer_xprm_hpp
#define rho_drawer_xprm_hpp

//#ifdef RHO_DRAWER
#ifdef __PSM__

#include "test_config.h"
#include "rho_master.h"
#include "opencv_master.h"
#include <string.h>

#define DETECTION_ELEMENT_RADIUS    4

#define DETECTION_MAP_FRAME_OWIDTH  800
#define DETECTION_MAP_FRAME_OHEIGHT DETECTION_MAP_FRAME_OWIDTH
#define DETECTION_MAP_INSET         0//15
#define DETECTION_MAP_FRAME_IWIDTH  DETECTION_MAP_FRAME_OWIDTH-DETECTION_MAP_INSET+DETECTION_ELEMENT_RADIUS
#define DETECTION_MAP_FRAME_IHEIGHT DETECTION_MAP_FRAME_OHEIGHT-DETECTION_MAP_INSET+DETECTION_ELEMENT_RADIUS

#define MAX_DETECTION_MAP_DENSITTY_VALUE 750
#define MAX_DETECTION_MAP_THRESH_VALUE (1 << 8)

#define DETECTION_MAP_INTERVAL  1

#define COV_SCALE 1

#define NUM_LABELS_TO_SHOW 3

using namespace cv;

template <typename T>
static std::string pto_string(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

#ifdef __PSM__
class RhoDrawer
{
    uint16_t counter = 0;
    Mat detection_map_frame;
    psm_t * psm;
public:
    RhoDrawer(psm_t *);

    void DrawDetectionMap( detection_map_t *, uint8_t thresh = 0 );
    void PostProcess( psm_t * );
    
    Mat& GetDetectionMapFrame() {return detection_map_frame;}
};
//#endif
#endif
#endif
#endif /* rho_drawer_xprm_hpp */
