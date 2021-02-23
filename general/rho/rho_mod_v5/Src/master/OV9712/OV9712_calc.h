/************************************************************************
 *  File: OV9712_calc.h
 *  Group: OV9712 Firmware
 ***********************************************************************/

#ifndef OV9712_calc_h
#define OV9712_calc_h

/************************************************************************
 *                             Includes                                 *
 ***********************************************************************/
#include <stdio.h>
#include <math.h>

/************************************************************************
 *                             Settings                                 *
 ***********************************************************************/
//#define OV9712_1280x800_CONFIG
//#define USE_RGGB_G_SKIP

/************************************************************************
 *                      Application Configuration                       *
 ***********************************************************************/
/// TODO: Get actual CORE_RATE
#define CORE_RATE                   PlatformFunctions.Clock.SysClockFreq()

#define SUBSAMPLE_APPLICATION       1
#define PERCENT_ACTIVE_APPLICATION  0.20
#define DEFAULT_FRAME_APPLICATION   20

#define CAPTURE_DIV                 1

/************************************************************************
 *                        Derived Configuration                         *
 ***********************************************************************/
#ifdef USE_RGGB_G_SKIP
#define SUBSAMPLE_BASE              2
#else
#define SUBSAMPLE_BASE              1
#endif

#define DEFAULT_SUBSAMPLE           ( (double)SUBSAMPLE_BASE * (double)SUBSAMPLE_APPLICATION )
#define DEFAULT_PERCENT_ACTIVE      PERCENT_ACTIVE_APPLICATION
#define DEFAULT_FRAME_RATE          DEFAULT_FRAME_APPLICATION

#define MAX_FRAME_RATE              15
#define MAX_PCLK_RATE               42000000
#define BASE_FRAME_SCALE            ( (double)MAX_PCLK_RATE / (double)MAX_FRAME_RATE )
#define PCLK(X)                     ( (double)X * BASE_FRAME_SCALE )

/************************************************************************
 *                      OV9712 Datasheet Values                         *
 * See https://datasheet.lcsc.com/szlcsc/OmniVision-Technologies-OV09712-V28A-1D_C11171.pdf *
 ***********************************************************************/
#define DEFAULT_HOFFSET             0x131
#define DEFAULT_VOFFSET             0x6

/* Color bar settings */
#define ENABLE_COLOR_BAR            false
#define COLOR_BAR_SOLID             0x0
#define COLOR_BAR_FADE              0x1
#define COLOR_BAR                   0x2
#define COLOR_BAR_STYLE             COLOR_BAR

#ifdef OV9712_1280x800_CONFIG
#define FRAME_WIDTH_BASE_PRE_DIV    1280
#define FRAME_HEIGHT_PRE_DIV        800
#define FRAME_WIDTH_BASE            ( CAPTURE_DIV == 0 ? FRAME_WIDTH_BASE_PRE_DIV : ( FRAME_WIDTH_BASE_PRE_DIV >> CAPTURE_DIV ) )
#define FRAME_HEIGHT                ( CAPTURE_DIV == 0 ? FRAME_HEIGHT_PRE_DIV : ( FRAME_HEIGHT_PRE_DIV >> CAPTURE_DIV ) )
#define FRAME_HSTART                ( ( ( FRAME_WIDTH_BASE_PRE_DIV >> 1 ) - ( FRAME_WIDTH_BASE >> 1 ) ) + DEFAULT_HOFFSET )
#define FRAME_VSTART                ( ( ( FRAME_HEIGHT_PRE_DIV >> 1 ) - ( FRAME_HEIGHT     >> 1 ) ) + DEFAULT_VOFFSET )

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
#define FRAME_WIDTH_BASE_PRE_DIV    640
#define FRAME_HEIGHT_PRE_DIV        400
#define FRAME_WIDTH_BASE            ( CAPTURE_DIV == 0 ? FRAME_WIDTH_BASE_PRE_DIV : ( FRAME_WIDTH_BASE_PRE_DIV >> CAPTURE_DIV ) )
#define FRAME_HEIGHT                ( CAPTURE_DIV == 0 ? FRAME_HEIGHT_PRE_DIV : ( FRAME_HEIGHT_PRE_DIV >> CAPTURE_DIV ) )
#define FRAME_HSTART                ( ( ( FRAME_WIDTH_BASE_PRE_DIV >> 1 ) - ( FRAME_WIDTH_BASE >> 1 ) ) + DEFAULT_HOFFSET )
#define FRAME_VSTART                ( ( ( FRAME_HEIGHT_PRE_DIV >> 1 ) - ( FRAME_HEIGHT     >> 1 ) ) + DEFAULT_VOFFSET )

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

/************************************************************************
 *                        Derived OV9712 Values                         *
 ***********************************************************************/
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

/************************************************************************
 *                              Rho Cycles                              *
 ***********************************************************************/
#define CAPTURE_ACTIVE_CYCLES       11
#define CAPTURE_INACTIVE_CYCLES     6
#define PROCESS_ALL_CYCLES          4
#define PROCESS_PIXEL_CYCLES        9
#define PROCESS_ROW_CYCLES          9

/************************************************************************
 *                      Derived Rho Values                              *
 ***********************************************************************/
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

/************************************************************************
 *                      Buffer Parameters                               *
 ***********************************************************************/
#define THRESH_BUFFER_MAX_LENGTH    55000

#define BUFFER_END_PADDING          2
#define CAPTURE_BUFFER_LENGTH       (uint32_t)( FRAME_WIDTH_BASE + BUFFER_END_PADDING )
#define THRESH_BUFFER_LENGTH        (uint32_t)( (uint32_t)( (double)FRAME_HEIGHT * (double)FRAME_WIDTH(DEFAULT_SUBSAMPLE) * PERCENT_ACTIVE_APPLICATION ) + FRAME_HEIGHT + BUFFER_END_PADDING )
#define DENSITY_MAP_X_LENGTH        (uint32_t)( FRAME_HEIGHT + BUFFER_END_PADDING )
#define DENSITY_MAP_Y_LENGTH        (uint32_t)( FRAME_WIDTH(DEFAULT_SUBSAMPLE) + BUFFER_END_PADDING )

/************************************************************************
 *                 OV9712 Registers & Parameters                        *
 ***********************************************************************/
#define CAMERA_WIDTH_F              FRAME_WIDTH_BASE
#define CAMERA_HEIGHT_F             FRAME_HEIGHT

#define CAMERA_DSP_WIDTH_F          CAMERA_WIDTH_F
#define CAMERA_DSP_HEIGHT_F         CAMERA_HEIGHT_F
#define CAMERA_DSP_WIDTH_MSB        ( ( CAMERA_DSP_WIDTH_F  >> 3 ) & 0xff )
#define CAMERA_DSP_WIDTH_LSB        ( CAMERA_DSP_WIDTH_F  & 0x07 )
#define CAMERA_DSP_HEIGHT_MSB       ( ( CAMERA_DSP_HEIGHT_F >> 2 ) & 0xff )
#define CAMERA_DSP_HEIGHT_LSB       ( CAMERA_DSP_HEIGHT_F & 0x03 )
#define REG57_V                     (uint8_t)( ( CAMERA_DSP_WIDTH_LSB << 2 ) | CAMERA_DSP_HEIGHT_LSB )
#define REG58_V                     (uint8_t)CAMERA_DSP_HEIGHT_MSB
#define REG59_V                     (uint8_t)CAMERA_DSP_WIDTH_MSB

#define CAMERA_SENSOR_HORZ_PAD      16
#define CAMERA_SENSOR_WIDTH         ( CAMERA_WIDTH_F + CAMERA_SENSOR_HORZ_PAD )
#define CAMERA_SENSOR_HEIGHT        CAMERA_HEIGHT_F
#define CAMERA_SENSOR_AHSIZE_LSB    ( CAMERA_SENSOR_WIDTH & 0x07 )
#define CAMERA_SENSOR_AHSIZE_MSB    ( ( CAMERA_SENSOR_WIDTH  >> 3 ) & 0xff )
#define CAMERA_SENSOR_AVSIZE_LSB    ( CAMERA_SENSOR_HEIGHT & 0x03 )
#define CAMERA_SENSOR_AVSIZE_MSB    ( ( CAMERA_SENSOR_HEIGHT >> 2 ) & 0xff )
#define CAMERA_SENSOR_HSTART_LSB    ( FRAME_HSTART & 0x07 )
#define CAMERA_SENSOR_HSTART_MSB    ( ( FRAME_HSTART >> 3 ) & 0xff )
#define CAMERA_SENSOR_VSTART_LSB    ( FRAME_VSTART & 0x03 )
#define CAMERA_SENSOR_VSTART_MSB    ( ( FRAME_VSTART >> 2 ) & 0xff )
#define REG32_V                     ( ( CAMERA_SENSOR_AHSIZE_LSB << 3 ) | CAMERA_SENSOR_HSTART_LSB )
#define REG03_V                     ( ( CAMERA_SENSOR_AVSIZE_LSB << 2 ) | CAMERA_SENSOR_VSTART_LSB )
#define HSTART_V                    CAMERA_SENSOR_HSTART_MSB
#define VSTART_V                    CAMERA_SENSOR_VSTART_MSB
#define AHSIZE_V                    CAMERA_SENSOR_AHSIZE_MSB
#define AVSIZE_V                    CAMERA_SENSOR_AVSIZE_MSB

#define DSP_CTRL_1_SMPH_MEAN_EN     ( CAPTURE_DIV > 0 ? 1 : 0)
#define DSP_CTRL_1_V                ( ( DSP_CTRL_1_SMPH_MEAN_EN << 7 ) | ( ENABLE_COLOR_BAR  << 3 ) | ( ENABLE_COLOR_BAR ? COLOR_BAR_STYLE & 0x3 : 0 ) )

#define CAMERA_WIDTH_R              ( ( ( REG57_V >> 3 ) & 0x07 ) | (REG59_V << 3))
#define CAMERA_HEIGHT_R             ( ( REG57_V & 0x03 ) | (REG58_V << 2))

//#define CALCULATE_CLOCK_FROM_REGISTER_DEFAULTS
#ifdef CALCULATE_CLOCK_FROM_REGISTER_DEFAULTS
#define BIT_INDEX_LOOKUP(X)         ( ( X & 0x02 ) ? ( ( X & 0x01 ) ? 0x04 : 0x02 ) : 0x01 )
#define REG5C_DEFAULT               0x6a                                     /* default is 0x59 */
#define REG5D_DEFAULT               0xc4                                     /* default is 0xc4 */
#define REG11_DEFAULT               0x00                                     /* default is 0x00 */
#define PLL_PRE_DIVIDER             BIT_INDEX_LOOKUP( ( ( REG5C_DEFAULT >> 5 ) & 0x03 ) )
#define PLL_MULTIPLIER              ( REG5C_DEFAULT & 0x1f )
#define PLL_DIVIDER                 ( ( REG5D_DEFAULT >> 2 ) & 0x03 )
#define CLK_DIVIDER                 REG11_DEFAULT
#else
#define REVERSE_BIT_LOOKUP(X)       ( ( X == 0x04 ) ? 0x03 : ( ( X == 0x02 ) ? 0x02 : 0x01 ) )
#define PLL_PRE_DIVIDER             0x04 /* REG5C[6:5]: 1= /1, 2= /2, or 3= /4, default is 0x02 */
#define PLL_MULTIPLIER              0x19 /* REG5C[4:0]: 32- 0-31,               default is 0x19 */
#define PLL_DIVIDER                 0x01 /* REG5D[3:2]: 0-3 +1,                 default is 0x01 */
#define CLK_DIVIDER                 0x00 /* REG11[5:0]: 0-63,                   default is 0x00 */
#define PLL_BYPASS                  (uint8_t)true /* true(1)/false(0)           default is true */
#define DRIVE_CAPABILITY            (uint8_t)0x03 /* 0-3 +1,                    default is 0x00 */
#define REG5C_V                     (uint8_t)( ( ( REVERSE_BIT_LOOKUP(PLL_PRE_DIVIDER) & 0x03 ) << 5 ) | ( PLL_MULTIPLIER & 0x1f ) )
#define REG5D_V                     (uint8_t)( ( 1 << 7 ) | ( ( PLL_BYPASS & 0x01 ) << 6 ) | ( ( DRIVE_CAPABILITY & 0x03 ) << 4 ) | ( ( PLL_DIVIDER & 0x03 ) << 2 ) )
#endif

#define MAX_XCLK_RATE               27000000
#define BASE_SYS_SCALE              ( (double)MAX_XCLK_RATE / (double)MAX_FRAME_RATE )
#define XCLK(X)                     (uint32_t)( (double)X * BASE_SYS_SCALE )
#define CLK2(X)                     (uint32_t)( PLL_BYPASS ? 0 : ( (double)XCLK(X) / (double)PLL_PRE_DIVIDER ) * (double)( 32 - PLL_MULTIPLIER ) )
#define SYSCLK(X)                   (uint32_t)( PLL_BYPASS ? (double)XCLK(X) : CLK2(X) / ( (double)( PLL_DIVIDER + 1) * ( 2. * (double)( CLK_DIVIDER + 1 ) ) ) )

/************************************************************************
 *                       Dynamic Calculation                            *
 ***********************************************************************/
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
