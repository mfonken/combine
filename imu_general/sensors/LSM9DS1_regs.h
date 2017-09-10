/***********************************************************************************************//**
 * \file   LSM9DS1_regs.h
 * \brief  LSM9DS1 registers header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef LSM9DS1_REGS_h
#define LSM9DS1_REGS_h

#include <stdint.h>

#include "../utilities/i2c_sp.h"

#define LSM9DS1_IMU_ADDR        0xd4
#define LSM9DS1_MAG_ADDR        0x38

#define LSM9DS1_IMU_ADC_MAX		32768.0

/***************************************************************************************************
 Accel and Gyro Registers
 **************************************************************************************************/
#define ACT_THS                 0x04
#define ACT_DUR                 0x05
#define INT_GEN_CFG_XL          0x06
#define INT_GEN_THS_X_XL        0x07
#define INT_GEN_THS_Y_XL        0x08
#define INT_GEN_THS_Z_XL        0x09
#define INT_GEN_DUR_XL          0x0a
#define REFERENCE_G             0x0b
#define INT1_CTRL               0x0c
#define INT2_CTRL               0x0d
#define WHO_AM_I                0x0f
#define CTRL_REG1_G             0x10
#define CTRL_REG2_G             0x11
#define CTRL_REG3_G             0x12
#define ORIENT_CFG_G            0x13
#define INT_GEN_SRC_G           0x14
#define OUT_TEMP_L              0x15
#define OUT_TEMP_H              0x16
#define STATUS_REG_1            0x17
#define OUT_X_L_G               0x18
#define OUT_X_H_G               0x19
#define OUT_Y_L_G               0x1a
#define OUT_Y_H_G               0x1b
#define OUT_Z_L_G               0x1c
#define OUT_Z_H_G               0x1d
#define CTRL_REG4               0x1e
#define CTRL_REG5_XL            0x1f
#define CTRL_REG6_XL            0x20
#define CTRL_REG7_XL            0x21
#define CTRL_REG8               0x22
#define CTRL_REG9               0x23
#define CTRL_REG10              0x24
#define INT_GEN_SRC_XL          0x26
#define STATUS_REG_2            0x27
#define OUT_X_L_XL              0x28
#define OUT_X_H_XL              0x29
#define OUT_Y_L_XL              0x2a
#define OUT_Y_H_XL              0x2b
#define OUT_Z_L_XL              0x2c
#define OUT_Z_H_XL              0x2d
#define FIFO_CTRL               0x2e
#define FIFO_SRC                0x2f
#define INT_GEN_CFG_G           0x30
#define INT_GEN_THS_XH_G        0x31
#define INT_GEN_THS_XL_G        0x32
#define INT_GEN_THS_YH_G        0x33
#define INT_GEN_THS_YL_G        0x34
#define INT_GEN_THS_ZH_G        0x35
#define INT_GEN_THS_ZL_G        0x36
#define INT_GEN_DUR_G           0x37
/***************************************************************************************************
 Magnometer Registers
 **************************************************************************************************/
#define OFFSET_X_REG_L_M        0x05
#define OFFSET_X_REG_H_M        0x06
#define OFFSET_Y_REG_L_M        0x07
#define OFFSET_Y_REG_H_M        0x08
#define OFFSET_Z_REG_L_M        0x09
#define OFFSET_Z_REG_H_M        0x0a
#define WHO_AM_I_M              0x0f
#define CTRL_REG1_M             0x20
#define CTRL_REG2_M             0x21
#define CTRL_REG3_M             0x22
#define CTRL_REG4_M             0x23
#define CTRL_REG5_M             0x24
#define STATUS_REG_M            0x27
#define OUT_X_L_M               0x28
#define OUT_X_H_M               0x29
#define OUT_Y_L_M               0x2a
#define OUT_Y_H_M               0x2b
#define OUT_Z_L_M               0x2c
#define OUT_Z_H_M               0x2d
#define INT_CFG_M               0x30
#define INT_SRC_M               0x31
#define INT_THS_L_M             0x32
#define INT_THS_H_M             0x33

#define XL_OUT					OUT_X_L_XL
#define G_OUT					OUT_X_L_G
#define M_OUT					OUT_X_L_M

/* Universal Register Commands */
#define ENABLE          1
#define DISABLE         0
#define LOW             1
#define HIGH            0

/***************************************************************************************************
 IMU Types and Definitions
 **************************************************************************************************/

/* Activity Threshold Register */
typedef struct
{
    uint8_t threshold       :7;                 /**< Activity threshold         */
    uint8_t sleep_on        :1;                 /**< Gyroscope operating mode   */
} LSM9DS1_act_ths_t;

/* Inactivity Duration Register */
typedef struct
{
    uint8_t threshold       :8;                 /**< Inactivity duration        */
} LSM9DS1_act_dur_t;

/* Linear Acceleration Interrupt Generator Configuration Register */
typedef struct
{
    uint8_t xlie_xl         :1;
    uint8_t xhie_xl         :1;
    uint8_t ylie_xl         :1;
    uint8_t yhie_xl         :1;
    uint8_t zlie_xl         :1;
    uint8_t zhie_xl         :1;
    uint8_t d_6d            :1;
    uint8_t aoi_xl          :1;                 /**< AND/OR int combination     */
} LSM9DS1_int_gen_cfg_xl_t;

/* Linear Acceleration Interrupt X Threshold Register */
typedef struct
{
    uint8_t threshold       :7;                 /**< Xaxis interrupt threshold  */
} LSM9DS1_int_gen_ths_x_xl_t;

/* Linear Acceleration Interrupt Y Threshold Register */
typedef struct
{
    uint8_t threshold       :7;                 /**< Yaxis interrupt threshold  */
} LSM9DS1_int_gen_ths_y_xl_t;

/* Linear Acceleration Interrupt Z Threshold Register */
typedef struct
{
    uint8_t threshold       :7;                 /**< Zaxis interrupt threshold  */
} LSM9DS1_int_gen_ths_z_xl_t;

/* Linear Acceleration Interrupt Duration Register */
typedef struct
{
    uint8_t dur_xl          :7;                 /**< Enter/exit interrupt value */
    uint8_t wait_xl         :1;                 /**< Duration counter           */
} LSM9DS1_int_gen_dur_xl_t;

/* Angular Rate Sensor Reference Register */
typedef struct
{
    uint8_t ref_g           :8;                 /**< Gyro HPF reference value   */
} LSM9DS1_reference_g_t;

/* INT1_A/G Control Register */
typedef struct
{                                               /**<  On Pin INT1_A/G           */
    uint8_t int1_drdy_xl    :1;                 /**< Accel data ready           */
    uint8_t int1_drdy_g     :1;                 /**< Gyro data ready            */
    uint8_t int1_boot       :1;                 /**< Boot status available      */
    uint8_t int1_fth        :1;                 /**< FIFO threshold interrupt   */
    uint8_t int1_ovr        :1;                 /**< Overrun interrupt enable   */
    uint8_t int1_fss5       :1;                 /**< FSS5 interrupt enable      */
    uint8_t int_ig_xl       :1;                 /**< Accel interrupt enable     */
    uint8_t int1_ig_g       :1;                 /**< Gyro interrupt enable      */
} LSM9DS1_int1_ctrl_t;

/* INT2_A/G Control Register */
typedef struct
{                                               /**<  On Pin INT2_A/G           */
    uint8_t int2_drdy_xl    :1;                 /**< Accel data ready           */
    uint8_t int2_drdy_g     :1;                 /**< Gyro data ready            */
    uint8_t int2_drdy_temp  :1;                 /**< Boot status available      */
    uint8_t int2_fth        :1;                 /**< FIFO threshold interrupt   */
    uint8_t int2_ovr        :1;                 /**< Overrun interrupt enable   */
    uint8_t int2_fss5       :1;                 /**< FSS5 interrupt enable      */
    uint8_t RESERVED        :1;
    uint8_t int2_inact      :1;                 /**< Inactivity interrupt       */
} LSM9DS1_int2_ctrl_t;

/* Angular Rate Sensor Control Register 1 */
typedef struct
{
    uint8_t bw_g            :2;                 /**< Gyro bandwidth selection   */
    uint8_t RESERVED        :1;
    uint8_t fs_g            :2;                 /**< Gyro full-scale selection  */
    uint8_t odr_g           :3;                 /**< Gyro data rate selection   */
} LSM9DS1_ctrl_reg1_g_t;

/* Angular Rate Sensor Control Register 2 */
typedef struct
{
    uint8_t out_sel         :2;                 /**< Out selection              */
    uint8_t int_sel         :2;                 /**< INT selection              */
    uint8_t RESERVED        :4;                 /**< */
} LSM9DS1_ctrl_reg2_g_t;

/* Angular Rate Sensor Control Register 3 */
typedef struct
{
    uint8_t hpcf_g          :4;                 /**< Gyro hpf cutoff selection  */
    uint8_t RESERVED        :2;
    uint8_t hp_en           :1;                 /**< High-pass filter enable    */
    uint8_t lp_mode         :1;                 /**< Low-power mode enable      */
} LSM9DS1_ctrl_reg3_g_t;

/* Angular Rate Sensor Sign and Orientation Register */
typedef struct
{
    uint8_t orient          :3;                 /**< Orientation selection      */
    uint8_t signx_g         :1;                 /**< Pitch axis (X) sign        */
    uint8_t signy_g         :1;                 /**< Roll axis (Y) sign         */
    uint8_t signz_g         :1;                 /**< Yaw axis (Z) sign          */
    uint8_t RESERVED        :2;
} LSM9DS1_orient_cfg_g_t;

/* Angular Rate Sensor Interrupt Source Register */
typedef struct
{
    uint8_t xl_g            :1;                 /**< Pitch (X) low              */
    uint8_t xh_g            :1;                 /**< Pitch (X) high             */
    uint8_t yl_g            :1;                 /**< Roll (Y) low               */
    uint8_t yh_g            :1;                 /**< Roll (Y) high              */
    uint8_t zl_g            :1;                 /**< Yaw (Z) low                */
    uint8_t zh_g            :1;                 /**< Yaw (Z) high               */
    uint8_t ia_g            :1;                 /**< Interrupt active           */
    uint8_t RESERVED        :1;
} LSM9DS1_int_gen_src_g_t;

/* Temperature Data Output Register */
typedef struct
{
    uint16_t temp_l         :8;                 /**< Tempurature low            */
    uint16_t temp_h         :8;                 /**< Tempurature high           */
} LSM9DS1_out_temp_t;

/* Status Register 1 */
typedef struct
{
    uint16_t xlda           :1;                 /**< Accel has data             */
    uint16_t gda            :1;                 /**< Gyro has data              */
    uint16_t tda            :1;                 /**< Temperature has data       */
    uint16_t boot_status    :1;                 /**< Boot running flag signal   */
    uint16_t inact          :1;                 /**< Inactivity interrupt       */
    uint16_t ig_g           :1;                 /**< Gyro interrupt output      */
    uint16_t ig_xl          :1;                 /**< Accel interrupt output     */
    uint8_t RESERVED        :1;
} LSM9DS1_status_reg_1_t;

/* Angular Rate Sensor Pitch */
typedef struct
{
    uint16_t out_x_g_l      :8;                 /**< Gyro pitch out low         */
    uint16_t out_x_g_h      :8;                 /**< Gyro pitch out high        */
} LSM9DS1_out_x_g_t;

/* Angular Rate Sensor Roll */
typedef struct
{
    uint16_t out_y_g_l      :8;                 /**< Gyro roll out low          */
    uint16_t out_y_g_h      :8;                 /**< Gyro roll out high         */
} LSM9DS1_out_y_g_t;

/* Angular Rate Sensor Yaw */
typedef struct
{
    uint16_t out_z_g_l      :8;                 /**< Gyro yaw out low           */
    uint16_t out_z_g_h      :8;                 /**< Gyro yaw out high          */
} LSM9DS1_out_z_g_t;

/* Control Register 4 */
typedef struct
{
    uint8_t o_4d_xl1        :1;                 /**< 4D option enable           */
    uint8_t lir_xl1         :1;                 /**< Latched interrupt          */
    uint8_t RESERVED1       :1;
    uint8_t xen_g           :1;                 /**< Pitch output enable        */
    uint8_t yen_g           :1;                 /**< Roll output enable         */
    uint8_t zen_g           :1;                 /**< Yaw output enable          */
    uint8_t RESERVED2       :2;
} LSM9DS1_ctrl_reg4_t;

/* Control Register 5 */
typedef struct
{
    uint8_t RESERVED1       :3;
    uint8_t xen_xl          :1;                 /**< Accel X output enable      */
    uint8_t yen_xl          :1;                 /**< Accel Y output enable      */
    uint8_t zen_xl          :1;                 /**< Accel Z output enable      */
    uint8_t dec_            :2;                 /**< Decimation selection       */
} LSM9DS1_ctrl_reg5_t;

/* Linear Acceleration Control Register 6 */
typedef struct
{
    uint8_t bw_xl           :2;                 /**< Anti-aliasing bandwidth    */
    uint8_t bw_scal_odr     :1;                 /**< Accel bandwidth selection  */
    uint8_t fs_xl           :2;                 /**< Accel full-scale selection */
    uint8_t odr_xl          :3;                 /**< Accel ODR and power mode   */
} LSM9DS1_ctrl_reg6_xl_t;

/* Linear Acceleration Control Register 7 */
typedef struct
{
    uint8_t hpis            :1;                 /**< */
    uint8_t RESERVED1       :1;                 /**< */
    uint8_t fds             :1;                 /**< */
    uint8_t RESERVED2       :2;                 /**< */
    uint8_t dcf             :2;                 /**< */
    uint8_t hr              :1;                 /**< */
} LSM9DS1_ctrl_reg7_xl_t;

/* Control Register 8 */
typedef struct
{
    uint8_t sw_reset        :1;                 /**< */
    uint8_t ble             :1;                 /**< */
    uint8_t if_add_inc      :1;                 /**< */
    uint8_t sim             :1;                 /**< */
    uint8_t pp_od           :1;                 /**< */
    uint8_t h_lactive       :1;                 /**< */
    uint8_t bdu             :1;                 /**< */
    uint8_t boot            :1;                 /**< */
} LSM9DS1_ctrl_reg8_t;

/* Control Register 9 */
typedef struct
{
    uint8_t stop_on_fth     :1;                 /**< */
    uint8_t fifo_en         :1;                 /**< */
    uint8_t i2c_disable     :1;                 /**< */
    uint8_t drdy_mask_bit   :1;                 /**< */
    uint8_t fifo_temp_en    :1;                 /**< */
    uint8_t RESERVED1       :1;                 /**< */
    uint8_t sleep_g         :1;                 /**< */
    uint8_t RESERVED2       :1;                 /**< */
} LSM9DS1_ctrl_reg9_t;

/* Control Register 10 */
typedef struct
{
    uint8_t st_xl           :1;                 /**< */
    uint8_t RESERVED1       :1;                 /**< */
    uint8_t st_g            :1;                 /**< */
    uint8_t RESERVED2       :5;                 /**< */
} LSM9DS1_ctrl_reg10_t;

/* Linear Acceleration Interrupt Source Register 10 */
typedef struct
{
    uint8_t xl_xl           :1;                 /**< */
    uint8_t xh_xl           :1;                 /**< */
    uint8_t yl_xl           :1;                 /**< */
    uint8_t yh_xl           :1;                 /**< */
    uint8_t zl_xl           :1;                 /**< */
    uint8_t zh_xl           :1;                 /**< */
    uint8_t ia_xl           :1;                 /**< */
    uint8_t RESERVED        :1;                 /**< */
} LSM9DS1_int_gen_src_xl_t;

/* Status Register 2 */
typedef struct
{
    uint8_t xlda            :1;                 /**< */
    uint8_t gda             :1;                 /**< */
    uint8_t tda             :1;                 /**< */
    uint8_t boot_status     :1;                 /**< */
    uint8_t inact           :1;                 /**< */
    uint8_t ig_g            :1;                 /**< */
    uint8_t ig_xl           :1;                 /**< */
    uint8_t RESERVED        :1;                 /**< */
} LSM9DS1_status_reg_2_t;

/* Linear Acceleration Sensor X */
typedef struct
{
    uint16_t out_x_xl_l     :8;                 /**< */
    uint16_t out_x_xl_h     :8;                 /**< */
} LSM9DS1_out_x_xl_t;

/* Linear Acceleration Sensor Y */
typedef struct
{
    uint16_t out_y_xl_l     :8;                 /**< */
    uint16_t out_y_xl_h     :8;                 /**< */
} LSM9DS1_out_y_xl_t;

/* Linear Acceleration Sensor Z */
typedef struct
{
    uint16_t out_z_xl_l     :8;                 /**< */
    uint16_t out_z_xl_h     :8;                 /**< */
} LSM9DS1_out_z_xl_t;

/* FIFO Control Register */
typedef struct
{
    uint8_t fmode           :3;                 /**< */
    uint8_t fth             :5;                 /**< */
} LSM9DS1_fifo_ctrl_t;

/* FIFO Status Control Register */
typedef struct
{
    uint8_t fss             :6;                 /**< */
    uint8_t ovrn            :1;                 /**< */
    uint8_t fth             :1;                 /**< */
} LSM9DS1_fifo_src_t;

/* Angular Rate Sensor Interrupt Generator Register */
typedef struct
{
    uint8_t xlie_g          :1;                 /**< */
    uint8_t xhie_g          :1;                 /**< */
    uint8_t ylie_g          :1;                 /**< */
    uint8_t yhie_g          :1;                 /**< */
    uint8_t zlie_g          :1;                 /**< */
    uint8_t zhie_g          :1;                 /**< */
    uint8_t lir_g           :1;                 /**< */
    uint8_t aoi_g           :1;                 /**< */
} LSM9DS1_int_gen_cfg_g_t;

/* Angular Rate Sensor Interrupt X Threshold */
typedef struct
{
    uint16_t ths_g_x_l      :8;                 /**< */
    uint16_t ths_g_x_h      :7;                 /**< */
    uint16_t dcrm_g         :1;                 /**< */
} LSM9DS1_int_gen_ths_x_g_t;


/* Angular Rate Sensor Interrupt Y Threshold */
typedef struct
{
    uint16_t ths_g_y_l      :8;                 /**< */
    uint16_t ths_g_y_h      :7;                 /**< */
} LSM9DS1_int_gen_ths_y_g_t;


/* Angular Rate Sensor Interrupt Z Threshold */
typedef struct
{
    uint16_t ths_g_z_l      :8;                 /**< */
    uint16_t ths_g_z_h      :7;                 /**< */
} LSM9DS1_int_gen_ths_z_g_t;


/* Angular Rate Sensor Interrupt Duration Register */
typedef struct
{
    uint8_t dur_g           :7;                 /**< */
    uint8_t wait_g          :1;                 /**< */
} LSM9DS1_int_gen_dur_g_t;

/***************************************************************************************************
 Magnometer Types and Definitions
 **************************************************************************************************/

/* Magnometer X Offset */
typedef struct
{
    uint16_t ofxm_l         :8;                 /**< */
    uint16_t ofxm_h         :8;                 /**< */
} LSM9DS1_offset_x_reg_m_t;

/* Magnometer Y Offset */
typedef struct
{
    uint16_t ofym_l         :8;                 /**< */
    uint16_t ofym_h         :8;                 /**< */
} LSM9DS1_offset_y_reg_m_t;

/* Magnometer Z Offset */
typedef struct
{
    uint16_t ofzm_l         :8;                 /**< */
    uint16_t ofzm_h         :8;                 /**< */
} LSM9DS1_offset_z_reg_m_t;

/* Magnometer Control Register 1 */
typedef struct
{
    uint8_t st              :1;                 /**< */
    uint8_t fast_odr        :1;                 /**< */
    uint8_t odr             :3;                 /**< */
    uint8_t om              :2;                 /**< */
    uint8_t temp_comp       :1;                 /**< */
} LSM9DS1_ctrl_reg1_m_t;

/* Magnometer Control Register 2 */
typedef struct
{
    uint8_t RESERVED1       :2;                 /**< */
    uint8_t soft_rst        :1;                 /**< */
    uint8_t reboot          :1;                 /**< */
    uint8_t RESERVED2       :1;                 /**< */
    uint8_t fs              :2;                 /**< */
    uint8_t RESERVED3       :1;                 /**< */
} LSM9DS1_ctrl_reg2_m_t;

/* Magnometer Control Register 3 */
typedef struct
{
    uint8_t md              :2;                 /**< */
    uint8_t sim             :1;                 /**< */
    uint8_t RESERVED1       :2;                 /**< */
    uint8_t lp              :1;                 /**< */
    uint8_t RESERVED2       :1;                 /**< */
    uint8_t i2c_disable     :1;                 /**< */
} LSM9DS1_ctrl_reg3_m_t;

/* Magnometer Control Register 4 */
typedef struct
{
    uint8_t RESERVED1       :1;                 /**< */
    uint8_t ble             :2;                 /**< */
    uint8_t omz             :2;                 /**< */
    uint8_t RESERVED2       :4;                 /**< */
} LSM9DS1_ctrl_reg4_m_t;

/* Magnometer Control Register 5 */
typedef struct
{
    uint8_t RESERVED1       :6;                 /**< */
    uint8_t bdu             :1;                 /**< */
    uint8_t fast_read       :1;                 /**< */
} LSM9DS1_ctrl_reg5_m_t;

/* Magnometer Status Register */
typedef struct
{
    uint8_t xda             :1;                 /**< */
    uint8_t yda             :1;                 /**< */
    uint8_t zda             :1;                 /**< */
    uint8_t zyxda           :1;                 /**< */
    uint8_t xorn            :1;                 /**< */
    uint8_t yorn            :1;                 /**< */
    uint8_t zorn            :1;                 /**< */
    uint8_t zyxorn          :1;                 /**< */
} LSM9DS1_status_reg_m_t;

/* Magnometer X Output Register */
typedef struct
{
    uint16_t out_x_l_m      :8;                 /**< */
    uint16_t out_x_h_m      :8;                 /**< */
} LSM9DS1_out_x_m_t;

/* Magnometer Y Output Register */
typedef struct
{
    uint16_t out_y_l_m      :8;                 /**< */
    uint16_t out_y_h_m      :8;                 /**< */
} LSM9DS1_out_y_m_t;

/* Magnometer Z Output Register */
typedef struct
{
    uint16_t out_z_l_m      :8;                 /**< */
    uint16_t out_z_h_m      :8;                 /**< */
} LSM9DS1_out_z_m_t;

/* Magnometer Source Control Register */
typedef struct
{
    uint8_t ien             :1;                 /**< */
    uint8_t iel             :1;                 /**< */
    uint8_t iea             :1;                 /**< */
    uint8_t RESERVED        :2;                 /**< */
    uint8_t zien            :1;                 /**< */
    uint8_t yien            :1;                 /**< */
    uint8_t xien            :1;                 /**< */
} LSM9DS1_int_cfg_m_t;

/* Magnometer Source Control Register */
typedef struct
{
    uint8_t int_            :1;                 /**< */
    uint8_t mroi            :1;                 /**< */
    uint8_t nth_z           :1;                 /**< */
    uint8_t nth_y           :1;                 /**< */
    uint8_t nth_x           :1;                 /**< */
    uint8_t pth_z           :1;                 /**< */
    uint8_t pth_y           :1;                 /**< */
    uint8_t pth_x           :1;                 /**< */
} LSM9DS1_int_src_m_t;

/* Magnometer Control Register 5 */
typedef struct
{
    uint16_t int_ths_l      :8;                 /**< */
    uint16_t int_ths_h      :8;                 /**< */
} LSM9DS1_int_ths_t;

/***************************************************************************************************
 Gyroscope Types and Definitions
 **************************************************************************************************/

/* Gyro Full-scale Selection */
enum LSM9DS1_gyro_fs
{
    GYRO_FS_245DPS                  = 0x00, /**< 245dps                         */
	GYRO_FS_500DPS                 	= 0x01, /**< 500dps                         */
	GYRO_FS_2000DPS                 = 0x03, /**< 2000dps					    */
};

/* Gyro odr and Cutoff Frequency Configuration Setting (After LPF1) */
enum LSM9DS1_gyro_odr
{
    GYRO_ODR_PWDN                   = 0x00, /**< Power down                     */
    GYRO_ODR_14_9HZ                 = 0x01, /**< 14.9Hz ODR with 5Hz cutoff     */
    GYRO_ODR_59_5HZ                 = 0x02, /**< 59.5Hz ODR with 19kHz cutoff   */
    GYRO_ODR_119HZ                  = 0x03, /**< 119Hz ODR with 38kHz cutoff    */
    GYRO_ODR_238HZ                  = 0x04, /**< 238Hz ODR with 76Hz cutoff     */
    GYRO_ODR_476HZ                  = 0x05, /**< 476Hz ODR with 100Hz cutoff    */
    GYRO_ODR_952HZ                  = 0x06, /**< 952Hz ODR with 100Hz cutoff    */
    GYRO_ODR_N_A                    = 0x07, /**< Not Available                  */
};

/***************************************************************************************************
 Accelerometer Types and Definitions
 **************************************************************************************************/
/* Accelerometer Full-scale Selection */
enum LSM9DS1_accel_fs
{
    XL_FS_2G	                  	= 0x00, /**< 2g		                        */
    XL_FS_4G	                  	= 0x02, /**< 4g		                        */
    XL_FS_8G	                  	= 0x03, /**< 8g		                        */
    XL_FS_16G	                  	= 0x01, /**< 16g	                        */
};

/* Accelerometer odr */
enum LSM9DS1_xl_odr
{
    XL_ODR_PWDN                     = 0x00, /**< Power down                     */
    XL_ODR_10HZ                     = 0x01, /**<  10Hz ODR                      */
    XL_ODR_50HZ                     = 0x02, /**<  50Hz ODR                      */
    XL_ODR_119HZ                    = 0x03, /**<  119Hz ODR                     */
    XL_ODR_238HZ                    = 0x04, /**<  238Hz ODR                     */
    XL_ODR_476HZ                    = 0x05, /**<  476Hz ODR                     */
    XL_ODR_952HZ                    = 0x06, /**<  952Hz ODR                     */
    XL_ODR_N_A                      = 0x07, /**<  Not Available                 */
};

/***************************************************************************************************
 Magnometer Types and Definitions
 **************************************************************************************************/

/* Magnometer Full-scale Selection */
enum LSM9DS1_mag_fs
{
    MAG_FS_4GAUSS                  	= 0x00, /**< +/-4 Gauss                     */
	MAG_FS_8GAUSS                 	= 0x01, /**< +/-8 Gauss                     */
	MAG_FS_12GAUSS                 	= 0x02, /**< +/-16 Gauss                    */
	MAG_FS_16GAUSS                 	= 0x03, /**< +/-32 Gauss                    */
};

/* Magnometer X and Y Operative Modes */
enum LSM9DS1_m_axis_om
{
    MAG_LOW_POWER                   = 0x00, /**< Low-power mode                 */
    MAG_MEDIUM_PERFORMANCE          = 0x01, /**< Medium-performance mode        */
    MAG_HIGH_PERFORMANCE            = 0x02, /**< High-performance mode          */
    MAG_ULTRA_HIGH_PERFORMANCE      = 0x03, /**< Ultra-high performance mode    */
};

/* Magnometer ODR Modes */
enum LSM9DS1_m_odr
{
    MAG_ODR_0_625HZ                 = 0x00, /**< 0.625Hz ODR                    */
    MAG_ODR_1_25HZ                  = 0x01, /**< 1.25Hz ODR                     */
    MAG_ODR_2_5HZ                   = 0x02, /**< 2,5Hz ODR                      */
    MAG_ODR_5HZ                     = 0x03, /**< 5Hz ODR                        */
    MAG_ODR_10HZ                    = 0x04, /**< 10Hz ODR                       */
    MAG_ODR_20HZ                    = 0x05, /**< 20Hz ODR                       */
    MAG_ODR_40HZ                    = 0x06, /**< 40Hz ODR                       */
    MAG_ODR_80HZ                    = 0x07, /**< 80Hz ODR                       */
};

/* Magnometer System Operating Modes */
enum LSM9DS1_m_om
{
    MAG_CONTINUOUS                  = 0x00, /**< Continuous-conversion mode     */
    MAG_SINGLE                      = 0x01, /**< Single-conversion mode         */
    MAG_POWER_DOWN                  = 0x03, /**< Power-down mode                */
};

/***************************************************************************************************
 FIFO Types and Definitions
 **************************************************************************************************/

/* FIFO Modes */
enum LSM9DS1_fifo_odr
{
    FIFO_BYPASS                     = 0x00, /**< Bypass FIFO (Off)              */
    FIFO_STOP_WHEN_FULL             = 0x01, /**< Stop collecting data when full */
    FIFO_CONTINUOUS_UNTIL_NTRIG     = 0x03, /**< Continuous until no trigger    */
    FIFO_BYPASS_UNTIL_NTRIG         = 0x04, /**< Bypass fifo until no trigger   */
    FIFO_CONTINUOUS                 = 0x06, /**< Continuous                     */
};

/***************************************************************************************************
 Universal Types and Definitions
 **************************************************************************************************/
/* General Configuration Type - 10 Bytes */
typedef struct
{
    LSM9DS1_act_ths_t             activity_threshold;       /**< */
    LSM9DS1_act_dur_t             inactivity_duration;      /**< */
    LSM9DS1_int1_ctrl_t           int1_control;
    LSM9DS1_int2_ctrl_t           int2_control;
    LSM9DS1_status_reg_1_t        status;
    LSM9DS1_status_reg_2_t        status_2;
    LSM9DS1_ctrl_reg8_t           system_control;
    LSM9DS1_ctrl_reg9_t           system_enables;
    LSM9DS1_ctrl_reg10_t          self_test;
} LSM9DS1_gen_t;

/* Accelerometer Configuration Type - 10 Bytes */
typedef struct
{
    LSM9DS1_int_gen_ths_x_xl_t    interrupt_threshold_x;        /**< */
    LSM9DS1_int_gen_ths_y_xl_t    interrupt_threshold_y;         /**< */
    LSM9DS1_int_gen_ths_z_xl_t    interrupt_threshold_z;     /**< */
    LSM9DS1_int_gen_cfg_xl_t      interrupt_configuration;
    LSM9DS1_int_gen_dur_xl_t      interrupt_duration;
    LSM9DS1_int_gen_src_xl_t      interrupt_source;
    LSM9DS1_ctrl_reg5_t           enable;
    LSM9DS1_ctrl_reg6_xl_t        bw_odr_control;
    LSM9DS1_ctrl_reg7_xl_t        filter_control;
    uint8_t                       PADDING;
} LSM9DS1_xl_t;

/* Gyro Configuration Type - 16 Bytes */
typedef struct
{
    LSM9DS1_ctrl_reg1_g_t         ctrl1;
    LSM9DS1_ctrl_reg2_g_t         ctrl2;        /**< */
    LSM9DS1_ctrl_reg3_g_t         ctrl3;         /**< */
    LSM9DS1_orient_cfg_g_t        orientation;     /**< */
    LSM9DS1_int_gen_src_g_t       interrupt_sources;
    LSM9DS1_ctrl_reg4_t           enable;
    LSM9DS1_int_gen_cfg_g_t       interrupt_configuration;
    LSM9DS1_int_gen_ths_x_g_t     interrupt_threshold_x;        /**< */
    LSM9DS1_int_gen_ths_y_g_t     interrupt_threshold_y;         /**< */
    LSM9DS1_int_gen_ths_z_g_t     interrupt_threshold_z;     /**< */
    LSM9DS1_int_gen_dur_g_t       interrupt_duration;
} LSM9DS1_g_t;

/* Magnometer Configuration Type - 18 Bytes */
typedef struct
{
    LSM9DS1_offset_x_reg_m_t      x_offset;
    LSM9DS1_offset_y_reg_m_t      y_offset;
    LSM9DS1_offset_z_reg_m_t      z_offset;
    LSM9DS1_ctrl_reg1_m_t         odr_control;
    LSM9DS1_ctrl_reg2_m_t         fs_control;
    LSM9DS1_ctrl_reg3_m_t         configuration_control;
    LSM9DS1_ctrl_reg4_m_t         operative_mode;
    LSM9DS1_ctrl_reg5_m_t         block_data_update_control;
    LSM9DS1_status_reg_m_t        status;
    LSM9DS1_int_cfg_m_t           interrupt_configuration;
    LSM9DS1_int_src_m_t           interrupt_source;
    LSM9DS1_int_ths_t             interrupt_threshold;
} LSM9DS1_m_t;

/* FIFO Configuration Type - 2 Bytes */
typedef struct
{
    LSM9DS1_fifo_ctrl_t           control;
    LSM9DS1_fifo_src_t            status_control;
} LSM9DS1_fifo_t;

/* Global Configuration Type - 56 Bytes */
typedef struct
{
    LSM9DS1_gen_t               general;
    LSM9DS1_xl_t                a;
    LSM9DS1_g_t                 g;
    LSM9DS1_m_t                 m;
    LSM9DS1_fifo_t              fifo;
} LSM9DS1_cfg_t;


/* IMU Default Configurations */
#define GYRO_FS_DEFAULT 		GYRO_FS_245DPS
#define XL_FS_DEFAULT 			XL_FS_2G
#define MAG_FS_DEFAULT			MAG_FS_8GAUSS

#define GYRO_RES_DEFAULT 		( double )( GYRO_FS_DEFAULT / LSM9DS1_IMU_ADC_MAX )
#define XL_RES_DEFAULT 			( double )( XL_FS_DEFAULT   / LSM9DS1_IMU_ADC_MAX )
#define MAG_RES_DEFAULT			( double )( MAG_FS_DEFAULT  / LSM9DS1_IMU_ADC_MAX )

/* Gyro Enable */
#define O_4D_EN_LOC             0
#define LATCHED_INT_LOC         1
#define GYRO_X_EN_LOC           3
#define GYRO_Y_EN_LOC           4
#define GYRO_Z_EN_LOC           5

#define O_4D_EN                 DISABLE
#define LATCHED_INT             DISABLE
#define GYRO_X_EN               ENABLE
#define GYRO_Y_EN               ENABLE
#define GYRO_Z_EN               ENABLE

#define GYRO_EN    ( O_4D_EN     << O_4D_EN_LOC     | \
                     LATCHED_INT << LATCHED_INT_LOC | \
                     GYRO_X_EN   << GYRO_X_EN_LOC   | \
                     GYRO_Y_EN   << GYRO_Y_EN_LOC   | \
                     GYRO_Z_EN   << GYRO_Z_EN_LOC   )

/* Gyro Control Register */
#define ODR_G_LOC	            5
#define FS_G_LOC				3
#define BW_G_LOC    	       	0

#define ODR_G	            	GYRO_ODR_952HZ
#define FS_G					GYRO_FS_DEFAULT
#define BW_G    	       		0

#define GYRO_CTRL_REG   ( ODR_G << ODR_G_LOC | \
						   FS_G << FS_G_LOC  | \
						   BW_G << BW_G_LOC  )

/* Accel Enable */
#define ACCEL_X_EN_LOC          3
#define ACCEL_Y_EN_LOC          4
#define ACCEL_Z_EN_LOC          5

#define ACCEL_X_EN              ENABLE
#define ACCEL_Y_EN              ENABLE
#define ACCEL_Z_EN              ENABLE

#define ACCEL_EN   ( ACCEL_X_EN << ACCEL_X_EN_LOC | \
                     ACCEL_Y_EN << ACCEL_Y_EN_LOC | \
                     ACCEL_Z_EN << ACCEL_Z_EN_LOC )

/* Gyro Control Register */
#define ODR_XL_LOC	            5
#define FS_XL_LOC				3
#define BW_XL_LOC    	       	0

#define ODR_XL	            	XL_ODR_952HZ
#define FS_XL					XL_FS_DEFAULT
#define BW_XL    	       		0

#define XL_CTRL_REG  ( ODR_XL << ODR_XL_LOC | \
						FS_XL << FS_XL_LOC  | \
						BW_XL << BW_XL_LOC  )

/* Default Values */
#define ACT_THS_DEFAULT                 0x00
#define ACT_DUR_DEFAULT                 0x00
#define INT_GEN_CFG_XL_DEFAULT          0x00
#define INT_GEN_THS_X_XL_DEFAULT        0x00
#define INT_GEN_THS_Y_XL_DEFAULT        0x00
#define INT_GEN_THS_Z_XL_DEFAULT        0x00
#define INT_GEN_DUR_XL_DEFAULT          0x00
#define REFERENCE_G_DEFAULT             0x00
#define INT1_CTRL_DEFAULT               0x00
#define INT2_CTRL_DEFAULT               0x00
#define WHO_AM_I_DEFAULT                0x68
#define CTRL_REG1_G_DEFAULT             GYRO_CTRL_REG
#define CTRL_REG2_G_DEFAULT             0x00
#define CTRL_REG3_G_DEFAULT             0x00
#define ORIENT_CFG_G_DEFAULT            0x00
#define CTRL_REG4_DEFAULT               GYRO_EN
#define CTRL_REG5_XL_DEFAULT            ACCEL_EN
#define CTRL_REG6_XL_DEFAULT            XL_CTRL_REG
#define CTRL_REG7_XL_DEFAULT            0x00
#define CTRL_REG8_DEFAULT               0x04
#define CTRL_REG9_DEFAULT               0x00
#define CTRL_REG10_DEFAULT              0x00
#define FIFO_CTRL_DEFAULT               0x00
#define FIFO_SRC_DEFAULT                0x00
#define INT_GEN_CFG_G_DEFAULT           0x00
#define INT_GEN_THS_XH_G_DEFAULT        0x00
#define INT_GEN_THS_XL_G_DEFAULT        0x00
#define INT_GEN_THS_YH_G_DEFAULT        0x00
#define INT_GEN_THS_YL_G_DEFAULT        0x00
#define INT_GEN_THS_ZH_G_DEFAULT        0x00
#define INT_GEN_THS_ZL_G_DEFAULT        0x00
#define INT_GEN_DUR_G_DEFAULT           0x00


/* Magnometer Default Configurations */

/* ODR/Operative Mode Control */
#define MAG_SELF_TEST_LOC               0
#define MAG_FAST_ODR_LOC                1
#define MAG_ODR_LOC                     2
#define MAG_OM_LOC                      5

#define MAG_SELF_TEST           DISABLE
#define MAG_FAST_ODR            DISABLE
#define MAG_ODR                 MAG_ODR_10HZ
#define MAG_OM                  MAG_HIGH_PERFORMANCE

#define MAG_ODR_OM_DEFAULT    ( MAG_SELF_TEST << MAG_SELF_TEST_LOC | \
                                MAG_FAST_ODR  << MAG_FAST_ODR_LOC  | \
                                MAG_ODR       << MAG_ODR_LOC       | \
                                MAG_OM        << MAG_OM_LOC        )

/* Magnometer Control (Full-scale) Register */
#define MAG_FS_LOC						5

#define MAG_CTRL_REG_DEFAULT MAG_FS_DEFAULT << MAG_FS_LOC

#define OFFSET_X_REG_L_M_DEFAULT        0x00
#define OFFSET_X_REG_H_M_DEFAULT        0x00
#define OFFSET_Y_REG_L_M_DEFAULT        0x00
#define OFFSET_Y_REG_H_M_DEFAULT        0x00
#define OFFSET_Z_REG_L_M_DEFAULT        0x00
#define OFFSET_Z_REG_H_M_DEFAULT        0x00
#define WHO_AM_I_M_DEFAULT              0x3d
#define CTRL_REG1_M_DEFAULT             MAG_ODR_OM_DEFAULT
#define CTRL_REG2_M_DEFAULT             MAG_CTRL_REG_DEFAULT
#define CTRL_REG3_M_DEFAULT             MAG_CONTINUOUS
#define CTRL_REG4_M_DEFAULT             0x00
#define CTRL_REG5_M_DEFAULT             0x00
#define INT_CFG_M_DEFAULT               0x00
#define INT_SRC_M_DEFAULT               0x00
#define INT_THS_L_M_DEFAULT             0x00
#define INT_THS_H_M_DEFAULT             0x00

/***************************************************************************************************
Local Functions
**************************************************************************************************/

/**************************************************************************//**
 * \brief Set register value on LSM9DS1
 * \param[in] reg Register to access
 * \param[in] val Value to set
 *****************************************************************************/
void LSM9DS1_SetRegister( uint8_t reg, uint8_t val );

/**************************************************************************//**
 * \brief Get register value from LSM9DS1
 * \param[out] Return value from register
 * \param[in] reg Register to access
 *****************************************************************************/
uint8_t LSM9DS1_GetRegister( uint8_t reg );

/**************************************************************************//**
 * \brief Set register value on LSM9DS1 Magnometer
 * \param[in] reg Register to access
 * \param[in] val Value to set
 *****************************************************************************/
void LSM9DS1_SetMagRegister( uint8_t reg, uint8_t val );

/**************************************************************************//**
 * \brief Get register value from LSM9DS1 Magnometer
 * \param[out] Return value from register
 * \param[in] reg Register to access
 *****************************************************************************/
uint8_t LSM9DS1_GetMagRegister( uint8_t reg );

/**************************************************************************//**
 * \brief Initialize LSM9DS1 with default values
 *****************************************************************************/
void LSM9DS1_defaultInit( LSM9DS1_cfg_t * );

/**************************************************************************//**
 * \brief Update LSM9DS1 registers with current global configuration
 *****************************************************************************/
void LSM9DS1_updateRegisters( LSM9DS1_cfg_t * );

#endif /* LSM9DS1_regs_h */
