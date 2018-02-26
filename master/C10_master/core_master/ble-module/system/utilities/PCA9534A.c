//
//  SysCtlr.c
//  
//
//  Created by Matthew Fonken on 12/17/16.
//
//

#include "PCA9534A.h"

#include "i2c_sp.h"

uint8_t curr_port;

void SYSCTL_Init( void )
{
    uint8_t i2c_write_data[2] = { CONFIG_REGISTER, SYSCTL_PORT_DIR };
    I2C_Write( SysCtlr_ADDR, i2c_write_data, 2 );

    SYSCTL_Set( SYSCTL_OUTPUT_DEFAULT );
}

uint8_t SYSCTL_Get( void )
{
	uint8_t i2c_read_data[1];
	I2C_Read_Reg( SysCtlr_ADDR, INPUT_REGISTER, i2c_read_data, 1 );
    return i2c_read_data[0];
}

void SYSCTL_Set( uint8_t port )
{
    uint8_t i2c_write_data[2] = { OUTPUT_REGISTER, *( uint8_t * )&port };
    I2C_Write( SysCtlr_ADDR, i2c_write_data, 2 );
    curr_port = port;
}

void SYSCTL_Toggle( uint8_t port )
{
    uint8_t i2c_write_data[2] = { TOGGLE_REGISTER, *( uint8_t * )&port };
    I2C_Write( SysCtlr_ADDR, i2c_write_data, 2 );
}

void SYSCTL_Enable_Magnometer( void )
{
	curr_port |=  ( 1 << IMU_CS );
	SYSCTL_Set( curr_port );
}
void SYSCTL_Disable_Magnometer( void )
{
	curr_port &= ~( 1 << IMU_CS );
	SYSCTL_Set( curr_port );
}
void SYSCTL_Enable_Force_Sensor( void )
{
	curr_port |=  ( 1 << FRC_EN );
	SYSCTL_Set( curr_port );
}
void SYSCTL_Disable_Force_Sensor( void )
{
	curr_port &= ~( 1 << FRC_EN );
	SYSCTL_Set( curr_port );
}
void SYSCTL_Enable_Camera( void )
{
	curr_port |=  ( 1 << VREG_MODE );
	SYSCTL_Set( curr_port );
}
void SYSCTL_Disable_Camera( void )
{
	curr_port &= ~( 1 << VREG_MODE );
	SYSCTL_Set( curr_port );
}
