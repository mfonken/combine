//
//  SysCtlr.h
//  
//
//  Created by Matthew Fonken on 12/17/16.
//
//

#ifndef SysCtlr_h
#define SysCtlr_h

#include <stdio.h>
#include <stdint.h>

#define SysCtlr_ADDR   0x70

#define INPUT_REGISTER  0
#define OUTPUT_REGISTER 1
#define TOGGLE_REGISTER 2
#define CONFIG_REGISTER 3

#define VREG_MODE       7 // Vsub switch global switching regulator
#define PWR_SWITCH      6 // Global power switch
#define OC_FLAG         5 // Over-current flag from
#define IMU_CS          4 // Magnometer chip select
/// TODO - add A/G CS
#define INT_CAM         3 // Interrupt from camera mcu
#define FRC_EN          2 // Force sensor circuit enable
/// TODO - configure for phototransistor
#define INT_M1          1 // Interrupt 1 from IMU
#define INT_T           0 // Interrupt from capactive touch sensor

/* Pin Direction Values */
#define INPUT           1
#define OUTPUT          0

/* Output Pin Values */
#define ON            	1
#define OFF             0

/* Pin Directions */
#define VREG_MODE_DIR   OUTPUT
#define PWR_SWITCH_DIR  OUTPUT
#define OC_FLAG_DIR     INPUT
#define IMU_CS_DIR      OUTPUT
#define INT_CAM_DIR     INPUT
#define FRC_EN_DIR      OUTPUT
#define INT_M1_DIR      INPUT
#define INT_T_DIR       INPUT

#define SYSCTL_PORT_DIR        (   \
    VREG_MODE_DIR  << VREG_MODE  | \
    PWR_SWITCH_DIR << PWR_SWITCH | \
    OC_FLAG_DIR    << OC_FLAG    | \
    IMU_CS_DIR     << IMU_CS     | \
    INT_CAM_DIR    << INT_CAM    | \
    FRC_EN_DIR     << FRC_EN     | \
    INT_M1_DIR     << INT_M1     | \
    INT_T_DIR      << INT_T		 )

/* Output Pin Defaults */
#define VREG_MODE_DEFAULT   ON
#define PWR_SWITCH_DEFAULT  OFF
#define IMU_CS_DEFAULT      ON
#define FRC_EN_DEFAULT      OFF

#define SYSCTL_OUTPUT_DEFAULT         \
    VREG_MODE_DEFAULT  << VREG_MODE  | \
    PWR_SWITCH_DEFAULT << PWR_SWITCH | \
    IMU_CS_DEFAULT     << IMU_CS     | \
    FRC_EN_DEFAULT     << FRC_EN

typedef struct
{
    uint8_t int_touch   :1;
    uint8_t int_m1      :1;
    uint8_t frc_en      :1;
    uint8_t int_cam     :1;
    uint8_t imu_cs      :1;
    uint8_t oc_flag     :1;
    uint8_t pwr_switch  :1;
    uint8_t vreg_mode   :1;
} SysCtlr_port_t;

void 	SYSCTL_Init( 				 void );
uint8_t SYSCTL_Get(  			 	 void );
void 	SYSCTL_Set( 			  	 uint8_t );
void 	SYSCTL_Toggle( 				 uint8_t );
void	SYSCTL_Enable_Magnometer( 	 void );
void 	SYSCTL_Disable_Magnometer( 	 void );
void 	SYSCTL_Enable_Force_Sensor(  void );
void	SYSCTL_Disable_Force_Sensor( void );
void	SYSCTL_Enable_Camera( 		 void );
void 	SYSCTL_Disable_Camera( 		 void );

#endif /* SysCtlr_h */
