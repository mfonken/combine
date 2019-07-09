//
//  OV9712_calc.h
//  OV9712_autotest
//
//  Created by Matthew Fonken on 7/7/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef OV9712_calc_h
#define OV9712_calc_h

#include <stdio.h>
#include <math.h>

//#define OV9712_1280x800_CONFIG
#define USE_RGGB_G_SKIP

/// TODO: Get actual CORE_RATE
#define CORE_RATE                   PlatformFunctions.Clock.SysClockFreq()

#define SUBSAMPLE_APPLICATION       3
#define PERCENT_ACTIVE_APPLICATION  0.34
#define DEFAULT_FRAME_APPLICATION   10//24

#ifdef USE_RGGB_G_SKIP
#define SUBSAMPLE_BASE              2
#else
#define SUBSAMPLE_BASE              1
#endif

#define DEFAULT_SUBSAMPLE           ( (double)SUBSAMPLE_BASE * (double)SUBSAMPLE_APPLICATION )
#define DEFAULT_PERCENT_ACTIVE      PERCENT_ACTIVE_APPLICATION
#define DEFAULT_FRAME_RATE          DEFAULT_FRAME_APPLICATION

#define MAX_FRAME_RATE              30
#define MAX_PCLK_RATE               42000000
#define BASE_FRAME_SCALE            ( (double)MAX_PCLK_RATE / (double)MAX_FRAME_RATE )
#define PCLK(X)                     ( (double)X * BASE_FRAME_SCALE )

#ifdef OV9712_1280x800_CONFIG
#define FRAME_WIDTH_BASE            1280
#define FRAME_HEIGHT                800

/* See https://datasheet.lcsc.com/szlcsc/OmniVision-Technologies-OV09712-V28A-1D_C11171.pdf */
#define VSYNC_HIGH_TP               33584
#define VSYNC_LOW_TP                1364080
#define VSYNC_TOTAL_TP              1397664
#define HREF_HIGH_TP                1280
#define HREF_LOW_TP                 408
#define HREF_TOTAL_TP               1688
#define FRAME_TO_ROW_START_TP       13832
#define ROW_END_TO_FRAME_END_TP     256
#define ROW_END_TO_ROW_START_TP     47672
#define TP_TO_PCLK                  1

#else
#define FRAME_WIDTH_BASE            640
#define FRAME_HEIGHT                400

#define VSYNC_HIGH_TP               9944
#define VSYNC_LOW_TP                688888
#define VSYNC_TOTAL_TP              698832
#define HREF_HIGH_TP                1280
#define HREF_LOW_TP                 408
#define HREF_TOTAL_TP               1688
#define FRAME_TO_ROW_START_TP       13840
#define ROW_END_TO_FRAME_END_TP     256
#define ROW_END_TO_ROW_START_TP     24040
#define TP_TO_PCLK                  2

#endif

#define FRAME_WIDTH(S)              ( (double)FRAME_WIDTH_BASE / (double)S )

#define VSYNC_HIGH                  VSYNC_HIGH_TP           * TP_TO_PCLK
#define VSYNC_LOW                   VSYNC_LOW_TP            * TP_TO_PCLK
#define VSYNC_TOTAL                 VSYNC_TOTAL_TP          * TP_TO_PCLK
#define HREF_HIGH                   HREF_HIGH_TP            * TP_TO_PCLK
#define HREF_LOW                    HREF_LOW_TP             * TP_TO_PCLK
#define HREF_TOTAL                  HREF_TOTAL_TP           * TP_TO_PCLK
#define FRAME_TO_ROW_START          FRAME_TO_ROW_START_TP   * TP_TO_PCLK
#define ROW_END_TO_FRAME_END        ROW_END_TO_FRAME_END_TP * TP_TO_PCLK
#define ROW_END_TO_ROW_START        ROW_END_TO_ROW_START_TP * TP_TO_PCLK

#define FRAME_DUR_SEC               ( (double)VSYNC_TOTAL / PCLK(_frameRate) )
#define ROW_DUR_SEC                 ( (double)HREF_TOTAL / PCLK(_frameRate) )
#define MID_FRAME_DUR_SEC           ( (double)ROW_END_TO_ROW_START / PCLK(_frameRate) )
#define MID_ROW_DUR_SEC             ( (double)HREF_LOW / PCLK(_frameRate) )
#define MID_ROWS_DUR_SEC            ( (double)MID_ROW_DUR_SEC * ( FRAME_HEIGHT - 1 ) )

#define TOTAL_PIXELS                ( (double)FRAME_HEIGHT * (double)FRAME_WIDTH(_subSample) )
#define ACTIVE_PIXELS               ( (uint32_t)( (double)TOTAL_PIXELS * _percentActive ) )
#define INACTIVE_PIXELS             ( (uint32_t)( (double)TOTAL_PIXELS * ( 1 - _percentActive ) ) )

/* CYCLES */
#define CAPTURE_ACTIVE_CYCLES       11
#define CAPTURE_INACTIVE_CYCLES     6
#define PROCESS_ALL_CYCLES          4
#define PROCESS_PIXEL_CYCLES        9
#define PROCESS_ROW_CYCLES          9

#define ACTIVE_CAPTURE_DUR_SEC      ( (double)CAPTURE_ACTIVE_CYCLES / (double)CORE_RATE )
#define INACTIVE_CAPTURE_DUR_SEC    ( (double)CAPTURE_INACTIVE_CYCLES / (double)CORE_RATE )
#define TOTAL_CAPTURE_DUR_SEC       ( (double)ACTIVE_CAPTURE_DUR_SEC + (double)INACTIVE_CAPTURE_DUR_SEC )

#define ACTIVE_CAPTURES_DUR_SEC     ( (double)ACTIVE_CAPTURE_DUR_SEC * (double)ACTIVE_PIXELS )
#define INACTIVE_CAPTURES_DUR_SEC   ( (double)INACTIVE_CAPTURE_DUR_SEC * (double)INACTIVE_PIXELS )
#define TOTAL_CAPTURES_DUR_SEC      ( (double)ACTIVE_CAPTURES_DUR_SEC + (double)INACTIVE_CAPTURES_DUR_SEC )

#define AVG_ROW_CAPTURE_DUR_SEC     ( (double)TOTAL_CAPTURES_DUR_SEC / (double)FRAME_HEIGHT )
#define PERCENT_REAL_TIME           ( (double)MID_ROWS_DUR_SEC / (double)TOTAL_CAPTURES_DUR_SEC )

#define ELEMENTS_TO_PROCESS         ( TOTAL_PIXELS + FRAME_HEIGHT )
#define PROCESS_AVG_CYCLES          ( ( (double)PROCESS_ALL_CYCLES * (double)ELEMENTS_TO_PROCESS ) \
                                      + ( (double)PROCESS_PIXEL_CYCLES * (double)TOTAL_PIXELS ) \
                                      + ( (double)FRAME_HEIGHT * (double)PROCESS_ROW_CYCLES ) )
#define PROCESS_AVG_DUR_SEC         ( (double)PROCESS_AVG_CYCLES / (double)CORE_RATE )
#define TOTAL_PROCESS_DUR_SEC       ( (double)TOTAL_CAPTURES_DUR_SEC + (double)PROCESS_AVG_DUR_SEC )
#define MID_FRAME_SLACK             ( (double)MID_FRAME_DUR_SEC + (double)MID_ROWS_DUR_SEC - (double)TOTAL_PROCESS_DUR_SEC )
#define FRAME_BURNS                 fabs( (double)MID_FRAME_SLACK / (double)FRAME_DUR_SEC )
#define ACTUAL_FRAME_RATE           ( MID_FRAME_SLACK >= 0. ? _frameRate \
                                      : _frameRate / (double)( (uint32_t)(FRAME_BURNS) + 2 ) )

#define THRESH_BUFFER_MAX_LENGTH    55000

#define BUFFER_END_PADDING          2
#define CAPTURE_BUFFER_LENGTH       (uint32_t)( FRAME_WIDTH_BASE + BUFFER_END_PADDING )
#define THRESH_BUFFER_LENGTH        (uint32_t)( (uint32_t)( (double)FRAME_HEIGHT * (double)FRAME_WIDTH(DEFAULT_SUBSAMPLE) * PERCENT_ACTIVE_APPLICATION ) + FRAME_HEIGHT + BUFFER_END_PADDING )
#define DENSITY_MAP_X_LENGTH        (uint32_t)( FRAME_HEIGHT + BUFFER_END_PADDING )
#define DENSITY_MAP_Y_LENGTH        (uint32_t)( FRAME_WIDTH(DEFAULT_SUBSAMPLE) + BUFFER_END_PADDING )

#define CAMERA_WIDTH_F              FRAME_WIDTH_BASE
#define CAMERA_HEIGHT_F             FRAME_HEIGHT
#define CAMERA_WIDTH_MSB            ( ( CAMERA_WIDTH_F  >> 3 ) & 0xff )
#define CAMERA_WIDTH_LSB            ( CAMERA_WIDTH_F  & 0x07 )
#define CAMERA_HEIGHT_MSB           ( ( CAMERA_HEIGHT_F >> 2 ) & 0xff )
#define CAMERA_HEIGHT_LSB           ( CAMERA_HEIGHT_F & 0x03 )
#define REG57_V                     (uint8_t)((CAMERA_WIDTH_LSB << 2) | CAMERA_HEIGHT_LSB)
#define REG58_V                     (uint8_t)CAMERA_HEIGHT_MSB
#define REG59_V                     (uint8_t)CAMERA_WIDTH_MSB

#define CAMERA_WIDTH_R              ( ( ( REG57_V >> 3 ) & 0x07 ) | (REG59_V << 3))
#define CAMERA_HEIGHT_R             ( ( REG57_V & 0x03 ) | (REG58_V << 2))

#define REG5C_DEFAULT               0x59 //6a
#define REG5D_DEFAULT               0xc4 //f4
#define REG11_DEFAULT               0x00

#define PLL_PRE_DIVIDER             ( ( REG5C_DEFAULT >> 5 ) & 0x03 ) // REG5C[6:5]
#define PLL_MULTIPLIER              ( REG5C_DEFAULT & 0x1f )          // REG5C[4:0]
#define PLL_DIVIDER                 ( ( REG5D_DEFAULT >> 2 ) & 0x03 ) // REG5D[3:2]
#define CLK_DIVIDER                 REG11_DEFAULT                     // REG11[5:0]

#define SYSCLK                      10000000 /// Design clock order
#define XCLK(X)                     (uint32_t)( ( ( 2. * (double)SYSCLK * (double)PLL_PRE_DIVIDER * (double)( PLL_DIVIDER + 1 ) * (double)( CLK_DIVIDER + 1 ) ) ) / (double)( 32 - PLL_MULTIPLIER ) )
#define OCLK(X)                     (uint32_t)( ( (double)XCLK(X) / (double)PLL_PRE_DIVIDER ) * (double)( 32 - PLL_MULTIPLIER ) )

static uint8_t _subSample           = DEFAULT_SUBSAMPLE;
static double _percentActive        = DEFAULT_PERCENT_ACTIVE;
static double _frameRate            = DEFAULT_FRAME_RATE;

static void SetSubsample( uint8_t subSample )
{
    _subSample = subSample;
}

static void SetPercentActive( double percentActive )
{
    if( percentActive > PERCENT_ACTIVE_APPLICATION )
        _percentActive = PERCENT_ACTIVE_APPLICATION;
    else
        _percentActive = percentActive;
}

static void SetFrameRate( double frameRate )
{
    _frameRate = frameRate;
}

#endif /* OV9712_calc_h */
