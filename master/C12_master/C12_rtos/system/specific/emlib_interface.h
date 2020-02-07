//
//  emlib_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 2/1/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef emlib_interface_h
#define emlib_interface_h

typedef enum
{
    I2C_READ_REG_EVENT = 1,
    I2C_WRITE_REG_EVENT
} emlib_i2c_transfer_type_t, EMLIB_I2C_TRANSFER_TYPE_T;

typedef struct
{
emlib_i2c_transfer_type_t
    type:8;
uint8_t
    reg,
    addr,
    length,
    *buffer;
} emlib_i2c_event_t, EMLIB_I2C_EVENT_T;

typedef enum
{
    SPI_READ_REG_EVENT = 1,
} emlib_spi_transfer_type_t, EMLIB_SPI_TRANSFER_TYPE_T;

typedef struct
{
emlib_spi_transfer_type_t
    type:8;
uint8_t
    reg,
    addr,
    length,
    *buffer;
} emlib_spi_event_t, EMLIB_SPI_EVENT_T;

#ifndef __EMLIB__
typedef bool emlib_i2c_transfer_return_t;
typedef bool EMLIB_I2C_TRANSFER_RETURN_T;
typedef bool emlib_spi_transfer_return_t;
typedef bool EMLIB_SPI_TRANSFER_RETURN_T;

static void EMLIB_PAPIInterface_DCDC_Init( uint16_t mV ) {};

static void EMLIB_PAPIInterface_I2C_Init( emlib_i2c_event_t * data ) {};
static void EMLIB_PAPIInterface_I2C_Enable( emlib_i2c_event_t * data ) {};
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_ReadRegister( emlib_i2c_event_t * data ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Read( emlib_i2c_event_t * data ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_WriteRegister( emlib_i2c_event_t * data ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Write( emlib_i2c_event_t * data ) { return true; };

static void EMLIB_PAPIInterface_SPI_Init( emlib_spi_event_t * data ) {};
static void EMLIB_PAPIInterface_SPI_Enable( emlib_spi_event_t * data ) {};
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_ReadRegister( emlib_spi_event_t * data ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Read( emlib_spi_event_t * data ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_WriteRegister( emlib_spi_event_t * data ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Write( emlib_spi_event_t * data ) { return true; };
#else
typedef I2C_TransferReturn_TypeDef emlib_i2c_transfer_return_t;
typedef I2C_TransferReturn_TypeDef EMLIB_I2C_TRANSFER_RETURN_T;

typedef SPI_TransferReturn_TypeDef emlib_spi_transfer_return_t;
typedef SPI_TransferReturn_TypeDef EMLIB_SPI_TRANSFER_RETURN_T;

static void EMLIB_PAPIInterface_DCDC_Init( uint16_t mV )
{
    typedef struct
    {
            EMU_PowerConfig_TypeDef powerConfig;
            EMU_DcdcMode_TypeDef dcdcMode;
            uint16_t mVout;
            uint16_t em01LoadCurrent_mA;
            uint16_t em234LoadCurrent_uA;
            uint16_t maxCurrent_mA;
            EMU_DcdcAnaPeripheralPower_TypeDef anaPeripheralPower;
            EMU_DcdcLnReverseCurrentControl_TypeDef reverseCurrentControl;
            EMU_DcdcLnCompCtrl_TypeDef dcdcLnCompCtrl;
    } EMU_DCDCInit_TypeDef;
    
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    dcdcInit.mVout = mV;
    EMU_DCDCInit(&dcdcInit);
}

static void EMLIB_PAPIInterface_I2C_Init( emlib_i2c_event_t * data )
{
    
};

static void EMLIB_PAPIInterface_I2C_Enable( emlib_i2c_event_t * data )
{
    
};

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_ReadRegister( emlib_i2c_event_t * data )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;

    seq.addr  = data->addr;
    seq.flags = I2C_FLAG_WRITE_READ;
    /* Select command to issue */
    seq.buf[0].len    = data->length;
    /* Select location/length of data to be read */
    seq.buf[1].data = data->data;
    seq.buf[1].len  = data->length;

    ret = I2C_TransferInit(I2C0, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(I2C0);
    }
    return ret;
}

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Read( emlib_i2c_event_t * data )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;

    seq.addr  = data->addr;
    seq.flags = I2C_FLAG_READ;
    /* Select location/length of data to be read */
    seq.buf[0].data = data->buffer;
    seq.buf[0].len  = data->length;

    ret = I2C_TransferInit(I2C0, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(I2C0);
    }
    return ret;
};

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_WriteRegister( emlib_i2c_event_t * data )
{
    data->length = 1;
    data->buffer = data->reg;
    return EMLIB_PAPIInterface_I2C_Write(data);
}

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Write( emlib_i2c_event_t * data )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;
    //uint8_t                    i2c_read_data[1];

    seq.addr  = data->addr;
    seq.flags = I2C_FLAG_WRITE;
    
    /* Select command to issue */
    seq.buf[0].data   = data->buffer;
    seq.buf[0].len    = data->length;

    ret = I2C_TransferInit(I2C0, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(I2C0);
    }
    return ret;
};



static void EMLIB_PAPIInterface_SPI_Init( emlib_spi_event_t * data )
{
    
};

static void EMLIB_PAPIInterface_SPI_Enable( emlib_spi_event_t * data )
{
    
};

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_ReadRegister( emlib_spi_event_t * data )
{
    SPI_TransferSeq_TypeDef    seq;
    emlib_spi_transfer_return_t ret;

    seq.addr  = data->addr;
    seq.flags = SPI_FLAG_WRITE_READ;
    /* Select command to issue */
    seq.buf[0].len    = data->length;
    /* Select location/length of data to be read */
    seq.buf[1].data = data->data;
    seq.buf[1].len  = data->length;

    ret = SPI_TransferInit(SPI0, &seq);
    while (ret == spiTransferInProgress)
    {
        ret = SPI_Transfer(SPI0);
    }
    return ret;
}

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Read( emlib_spi_event_t * data )
{
    SPI_TransferSeq_TypeDef    seq;
    emlib_spi_transfer_return_t ret;

    seq.addr  = data->addr;
    seq.flags = SPI_FLAG_READ;
    /* Select location/length of data to be read */
    seq.buf[0].data = data->buffer;
    seq.buf[0].len  = data->length;

    ret = SPI_TransferInit(SPI0, &seq);
    while (ret == spiTransferInProgress)
    {
        ret = SPI_Transfer(SPI0);
    }
    return ret;
};

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_WriteRegister( emlib_spi_event_t * data )
{
    data->length = 1;
    data->buffer = data->reg;
    return EMLIB_PAPIInterface_SPI_Write(data);
}

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Write( emlib_spi_event_t * data )
{
    SPI_TransferSeq_TypeDef    seq;
    emlib_spi_transfer_return_t ret;
    //uint8_t                    spi_read_data[1];

    seq.addr  = data->addr;
    seq.flags = SPI_FLAG_WRITE;
    
    /* Select command to issue */
    seq.buf[0].data   = data->buffer;
    seq.buf[0].len    = data->length;

    ret = SPI_TransferInit(SPI0, &seq);
    while (ret == spiTransferInProgress)
    {
        ret = SPI_Transfer(SPI0);
    }
    return ret;
};

#endif

#endif /* emlib_interface_h */
