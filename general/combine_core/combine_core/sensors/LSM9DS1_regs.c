/***********************************************************************************************//**
* \file   LSM9DS1_regs.c
* \brief  LSM9DS1 registers initialization
***************************************************************************************************
*      Author: Matthew Fonken
**************************************************************************************************/

#include "LSM9DS1_regs.h"

/***************************************************************************************************
Local Variables
**************************************************************************************************/

/***************************************************************************************************
Local Functions
**************************************************************************************************/

/**************************************************************************//**
* \brief Get register value from LSM9DS1
* \param[out] Return value from register
* \param[in] reg Register to access
*****************************************************************************/
uint8_t LSM9DS1_GetRegister( uint8_t reg )
{
  return I2C_Read_Reg( LSM9DS1_IMU_ADDR, reg);
}

/**************************************************************************//**
* \brief Set register value on LSM9DS1
* \param[in] reg Register to access
* \param[in] val Value to set
*****************************************************************************/
void LSM9DS1_SetRegister( uint8_t reg, uint8_t val )
{
  uint8_t i2c_write_data[2];
  i2c_write_data[0] = reg;
  i2c_write_data[1] = val;
  I2C_Write( LSM9DS1_IMU_ADDR, i2c_write_data, 2 );
}

/**************************************************************************//**
* \brief Get register value from LSM9DS1 magnometer
* \param[out] Return value from register
* \param[in] reg Register to access
*****************************************************************************/
uint8_t LSM9DS1_GetMagRegister( uint8_t reg )
{
  return I2C_Read_Reg( LSM9DS1_MAG_ADDR, reg );
}

/**************************************************************************//**
* \brief Set register value on LSM9DS1 magnometer
* \param[in] reg Register to access
* \param[in] val Value to set
*****************************************************************************/
void LSM9DS1_SetMagRegister( uint8_t reg, uint8_t val )
{
  uint8_t i2c_write_data[2];
  i2c_write_data[0] = reg;
  i2c_write_data[1] = val;
  I2C_Write( LSM9DS1_MAG_ADDR, i2c_write_data, 2 );
}

/**************************************************************************//**
* \brief Initialize LSM9DS1 with default values
*****************************************************************************/
void LSM9DS1_defaultInit( LSM9DS1_cfg_t * lsm9 )
{
  uint8_t act_ths_default             = ACT_THS_DEFAULT;
  uint8_t act_dur_default             = ACT_DUR_DEFAULT;
  uint8_t int1_ctrl_default           = INT1_CTRL_DEFAULT;
  uint8_t int2_ctrl_default           = INT2_CTRL_DEFAULT;
  uint8_t ctrl_reg8_default           = CTRL_REG8_DEFAULT;
  uint8_t ctrl_reg9_default           = CTRL_REG9_DEFAULT;
  uint8_t ctrl_reg10_default          = CTRL_REG10_DEFAULT;
  lsm9->general.activity_threshold    = *( LSM9DS1_act_ths_t    *)&act_ths_default;
  lsm9->general.inactivity_duration   = *( LSM9DS1_act_dur_t    *)&act_dur_default;
  lsm9->general.int1_control          = *( LSM9DS1_int1_ctrl_t  *)&int1_ctrl_default;
  lsm9->general.int2_control          = *( LSM9DS1_int2_ctrl_t  *)&int2_ctrl_default;
  lsm9->general.system_control        = *( LSM9DS1_ctrl_reg8_t  *)&ctrl_reg8_default;
  lsm9->general.system_enables        = *( LSM9DS1_ctrl_reg9_t  *)&ctrl_reg9_default;
  lsm9->general.self_test             = *( LSM9DS1_ctrl_reg10_t *)&ctrl_reg10_default;

  uint8_t fifo_ctrl_default           = FIFO_CTRL_DEFAULT;
  uint8_t fifo_src_default            = FIFO_SRC_DEFAULT;
  lsm9->fifo.control                  = *( LSM9DS1_fifo_ctrl_t  *)&fifo_ctrl_default;
  lsm9->fifo.status_control           = *( LSM9DS1_fifo_src_t   *)&fifo_src_default;

  uint8_t int_gen_ths_x_xl_default    = INT_GEN_THS_X_XL_DEFAULT;
  uint8_t int_gen_ths_y_xl_default    = INT_GEN_THS_Y_XL_DEFAULT;
  uint8_t int_gen_ths_z_xl_default    = INT_GEN_THS_Z_XL_DEFAULT;
  uint8_t int_gen_cfg_xl_default      = INT_GEN_CFG_XL_DEFAULT;
  uint8_t int_gen_dur_xl_default      = INT_GEN_DUR_XL_DEFAULT;
  uint8_t ctrl_reg6_xl_default        = CTRL_REG6_XL_DEFAULT;
  uint8_t ctrl_reg7_xl_default        = CTRL_REG7_XL_DEFAULT;
  uint8_t ctrl_reg5_xl_default        = CTRL_REG5_XL_DEFAULT;
  lsm9->a.interrupt_threshold_x   	= *( LSM9DS1_int_gen_ths_x_xl_t *)&int_gen_ths_x_xl_default;
  lsm9->a.interrupt_threshold_y   	= *( LSM9DS1_int_gen_ths_y_xl_t *)&int_gen_ths_y_xl_default;
  lsm9->a.interrupt_threshold_z   	= *( LSM9DS1_int_gen_ths_z_xl_t *)&int_gen_ths_z_xl_default;
  lsm9->a.interrupt_configuration 	= *( LSM9DS1_int_gen_cfg_xl_t   *)&int_gen_cfg_xl_default;
  lsm9->a.interrupt_duration      	= *( LSM9DS1_int_gen_dur_xl_t   *)&int_gen_dur_xl_default;
  lsm9->a.bw_odr_control          	= *( LSM9DS1_ctrl_reg6_xl_t     *)&ctrl_reg6_xl_default;
  lsm9->a.filter_control          	= *( LSM9DS1_ctrl_reg7_xl_t     *)&ctrl_reg7_xl_default;
  lsm9->a.enable                  	= *( LSM9DS1_ctrl_reg5_t        *)&ctrl_reg5_xl_default;

  uint8_t ctrl_reg1_g_default         = CTRL_REG1_G_DEFAULT;
  uint8_t ctrl_reg2_g_default         = CTRL_REG2_G_DEFAULT;
  uint8_t ctrl_reg3_g_default         = CTRL_REG3_G_DEFAULT;
  uint16_t int_gen_ths_x_g_default    = ( INT_GEN_THS_XH_G_DEFAULT << 8 ) && INT_GEN_THS_XL_G_DEFAULT;
  uint16_t int_gen_ths_y_g_default    = ( INT_GEN_THS_YH_G_DEFAULT << 8 ) && INT_GEN_THS_YL_G_DEFAULT;
  uint16_t int_gen_ths_z_g_default    = ( INT_GEN_THS_ZH_G_DEFAULT << 8 ) && INT_GEN_THS_ZL_G_DEFAULT;
  uint8_t orient_cfg_g_default        = ORIENT_CFG_G_DEFAULT;
  uint8_t int_gen_cfg_g_default       = INT_GEN_CFG_G_DEFAULT;
  uint8_t int_gen_dur_g_default       = INT_GEN_DUR_G_DEFAULT;
  uint8_t ctrl_reg4_default           = CTRL_REG4_DEFAULT;
  lsm9->g.ctrl1                    	= *( LSM9DS1_ctrl_reg1_g_t     *)&ctrl_reg1_g_default;
  lsm9->g.ctrl2                    	= *( LSM9DS1_ctrl_reg2_g_t     *)&ctrl_reg2_g_default;
  lsm9->g.ctrl3                    	= *( LSM9DS1_ctrl_reg3_g_t     *)&ctrl_reg3_g_default;
  lsm9->g.interrupt_threshold_x    	= *( LSM9DS1_int_gen_ths_x_g_t *)&int_gen_ths_x_g_default;
  lsm9->g.interrupt_threshold_y    	= *( LSM9DS1_int_gen_ths_y_g_t *)&int_gen_ths_y_g_default;
  lsm9->g.interrupt_threshold_z    	= *( LSM9DS1_int_gen_ths_z_g_t *)&int_gen_ths_z_g_default;
  lsm9->g.orientation              	= *( LSM9DS1_orient_cfg_g_t    *)&orient_cfg_g_default;
  lsm9->g.interrupt_configuration  	= *( LSM9DS1_int_gen_cfg_g_t   *)&int_gen_cfg_g_default;
  lsm9->g.interrupt_duration       	= *( LSM9DS1_int_gen_dur_g_t   *)&int_gen_dur_g_default;
  lsm9->g.enable                   	= *( LSM9DS1_ctrl_reg4_t       *)&ctrl_reg4_default;

  uint16_t offset_x_reg_m_default     = ( OFFSET_X_REG_H_M_DEFAULT << 8 ) && OFFSET_X_REG_L_M_DEFAULT;
  uint16_t offset_y_reg_m_default     = ( OFFSET_Y_REG_H_M_DEFAULT << 8 ) && OFFSET_Y_REG_L_M_DEFAULT;
  uint16_t offset_z_reg_m_default     = ( OFFSET_Z_REG_H_M_DEFAULT << 8 ) && OFFSET_Z_REG_L_M_DEFAULT;
  uint8_t ctrl_reg1_m_default         = CTRL_REG1_M_DEFAULT;
  uint8_t ctrl_reg2_m_default         = CTRL_REG2_M_DEFAULT;
  uint8_t ctrl_reg3_m_default         = CTRL_REG3_M_DEFAULT;
  uint8_t ctrl_reg4_m_default         = CTRL_REG4_M_DEFAULT;
  uint8_t ctrl_reg5_m_default         = CTRL_REG5_M_DEFAULT;
  uint8_t int_cfg_m_default           = INT_CFG_M_DEFAULT;
  uint16_t int_ths_m_default          = ( INT_THS_H_M_DEFAULT << 8 ) && INT_THS_L_M_DEFAULT;
  lsm9->m.x_offset                  	= *( LSM9DS1_offset_x_reg_m_t *)&offset_x_reg_m_default;
  lsm9->m.y_offset                  	= *( LSM9DS1_offset_y_reg_m_t *)&offset_y_reg_m_default;
  lsm9->m.z_offset                  	= *( LSM9DS1_offset_z_reg_m_t *)&offset_z_reg_m_default;
  lsm9->m.odr_control               	= *( LSM9DS1_ctrl_reg1_m_t    *)&ctrl_reg1_m_default;
  lsm9->m.fs_control                	= *( LSM9DS1_ctrl_reg2_m_t    *)&ctrl_reg2_m_default;
  lsm9->m.configuration_control     	= *( LSM9DS1_ctrl_reg3_m_t    *)&ctrl_reg3_m_default;
  lsm9->m.operative_mode            	= *( LSM9DS1_ctrl_reg4_m_t    *)&ctrl_reg4_m_default;
  lsm9->m.block_data_update_control 	= *( LSM9DS1_ctrl_reg5_m_t    *)&ctrl_reg5_m_default;
  lsm9->m.interrupt_configuration   	= *( LSM9DS1_int_cfg_m_t      *)&int_cfg_m_default;
  lsm9->m.interrupt_threshold       	= *( LSM9DS1_int_ths_t        *)&int_ths_m_default;

  LSM9DS1_updateRegisters( lsm9 );
}

/**************************************************************************//**
* \brief Update LSM9DS1 registers with current global configuration
*****************************************************************************/
void LSM9DS1_updateRegisters( LSM9DS1_cfg_t * lsm9 )
{
  /* Set General Registers */
  LSM9DS1_SetRegister( ACT_THS,           *( uint8_t *)&lsm9->general.activity_threshold);
  LSM9DS1_SetRegister( ACT_DUR,           *( uint8_t *)&lsm9->general.inactivity_duration);
  LSM9DS1_SetRegister( INT1_CTRL,         *( uint8_t *)&lsm9->general.int1_control);
  LSM9DS1_SetRegister( INT2_CTRL,         *( uint8_t *)&lsm9->general.int2_control);
  LSM9DS1_SetRegister( CTRL_REG8,         *( uint8_t *)&lsm9->general.system_control);
  LSM9DS1_SetRegister( CTRL_REG9,         *( uint8_t *)&lsm9->general.system_enables);
  LSM9DS1_SetRegister( CTRL_REG10,        *( uint8_t *)&lsm9->general.self_test);

  /* Set FIFO Registers */
  LSM9DS1_SetRegister( FIFO_CTRL,         *( uint8_t *)&lsm9->fifo.control);
  LSM9DS1_SetRegister( FIFO_SRC,          *( uint8_t *)&lsm9->fifo.status_control);

  /* Set Accelerometer Registers */
  LSM9DS1_SetRegister( INT_GEN_THS_X_XL,  *( uint8_t *)&lsm9->a.interrupt_threshold_x);
  LSM9DS1_SetRegister( INT_GEN_THS_Y_XL,  *( uint8_t *)&lsm9->a.interrupt_threshold_y);
  LSM9DS1_SetRegister( INT_GEN_THS_Z_XL,  *( uint8_t *)&lsm9->a.interrupt_threshold_z);
  LSM9DS1_SetRegister( INT_GEN_CFG_XL,    *( uint8_t *)&lsm9->a.interrupt_configuration);
  LSM9DS1_SetRegister( INT_GEN_DUR_XL,    *( uint8_t *)&lsm9->a.interrupt_duration);
  LSM9DS1_SetRegister( CTRL_REG6_XL,      *( uint8_t *)&lsm9->a.bw_odr_control);
  LSM9DS1_SetRegister( CTRL_REG7_XL,      *( uint8_t *)&lsm9->a.filter_control);
  LSM9DS1_SetRegister( CTRL_REG5_XL,      *( uint8_t *)&lsm9->a.enable);

  /* Set Gyroscope Registers */
  LSM9DS1_SetRegister( CTRL_REG1_G,       *( uint8_t *)&lsm9->g.ctrl1);
  LSM9DS1_SetRegister( CTRL_REG2_G,       *( uint8_t *)&lsm9->g.ctrl2);
  LSM9DS1_SetRegister( CTRL_REG3_G,       *( uint8_t *)&lsm9->g.ctrl3);
  LSM9DS1_SetRegister( INT_GEN_THS_XH_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_x << 8 );
  LSM9DS1_SetRegister( INT_GEN_THS_XL_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_x);
  LSM9DS1_SetRegister( INT_GEN_THS_YH_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_y << 8 );
  LSM9DS1_SetRegister( INT_GEN_THS_YL_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_y);
  LSM9DS1_SetRegister( INT_GEN_THS_ZH_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_z << 8 );
  LSM9DS1_SetRegister( INT_GEN_THS_ZL_G,  *( uint8_t *)&lsm9->g.interrupt_threshold_z);
  LSM9DS1_SetRegister( ORIENT_CFG_G,      *( uint8_t *)&lsm9->g.orientation);
  LSM9DS1_SetRegister( INT_GEN_CFG_G,     *( uint8_t *)&lsm9->g.interrupt_configuration);
  LSM9DS1_SetRegister( INT_GEN_DUR_G,     *( uint8_t *)&lsm9->g.interrupt_duration);
  LSM9DS1_SetRegister( CTRL_REG4,         *( uint8_t *)&lsm9->g.enable);

  /* Set Magnometer Registers */
  LSM9DS1_SetMagRegister( OFFSET_X_REG_H_M,  *( uint8_t *)&lsm9->m.x_offset << 8 );
  LSM9DS1_SetMagRegister( OFFSET_X_REG_L_M,  *( uint8_t *)&lsm9->m.x_offset );
  LSM9DS1_SetMagRegister( OFFSET_Y_REG_H_M,  *( uint8_t *)&lsm9->m.y_offset << 8 );
  LSM9DS1_SetMagRegister( OFFSET_Y_REG_L_M,  *( uint8_t *)&lsm9->m.y_offset );
  LSM9DS1_SetMagRegister( OFFSET_Z_REG_H_M,  *( uint8_t *)&lsm9->m.z_offset << 8 );
  LSM9DS1_SetMagRegister( OFFSET_Z_REG_L_M,  *( uint8_t *)&lsm9->m.z_offset );
  LSM9DS1_SetMagRegister( CTRL_REG1_M,       *( uint8_t *)&lsm9->m.odr_control );
  LSM9DS1_SetMagRegister( CTRL_REG2_M,       *( uint8_t *)&lsm9->m.fs_control );
  LSM9DS1_SetMagRegister( CTRL_REG3_M,       *( uint8_t *)&lsm9->m.configuration_control );
  LSM9DS1_SetMagRegister( CTRL_REG4_M,       *( uint8_t *)&lsm9->m.operative_mode );
  LSM9DS1_SetMagRegister( CTRL_REG5_M,       *( uint8_t *)&lsm9->m.block_data_update_control );
  LSM9DS1_SetMagRegister( INT_CFG_M,         *( uint8_t *)&lsm9->m.interrupt_configuration );
  LSM9DS1_SetMagRegister( INT_THS_H_M,       *( uint8_t *)&lsm9->m.interrupt_threshold << 8 );
  LSM9DS1_SetMagRegister( INT_THS_L_M,       *( uint8_t *)&lsm9->m.interrupt_threshold );
}
