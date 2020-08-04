//
//  emlib_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 2/1/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef emlib_interface_h
#define emlib_interface_h

#ifdef __EMLIB__
//#define DCDC_SERVICE
#define I2C_SERVICE
#define USART_SERVICE
#endif /* __EMLIB__ */



#ifdef __EMLIB__
#define ASSERT EFM_ASSERT

#ifdef I2C_SERVICE
#include <em_i2c.h>
#endif

#ifdef USART_SERVICE
#include <em_usart.h>
#endif

#else
#include <assert.h>
#define EFM_ASSERT assert
typedef void I2C_TypeDef;
typedef void SPI_TypeDef;
typedef void USART_TypeDef;
typedef uint8_t GPIO_Port_TypeDef;
#define I2C1 1
#define I2C2 2
#define SPI1 1
#define SPI2 2
#define USART1 1
#define USART2 2
#endif

typedef USART_TypeDef SPI_TypeDef;

typedef enum
{
    I2C_READ_REG_EVENT = 0,
    I2C_WRITE_REG_EVENT
} emlib_i2c_transfer_type_t, EMLIB_I2C_TRANSFER_TYPE_T;

typedef enum
{
    EMLIB_GPIO_STATE_LOW = 0,
    EMLIB_GPIO_STATE_HIGH
} EMLIB_GPIO_STATE_T, emlib_gpio_state_t;

typedef enum
{
    EMLIB_GPIO_NCS_ACTIVE = 0,
    EMLIB_GPIO_NCS_INACTIVE,
} EMLIB_GPIO_NCS_T, emlib_gpio_ncs_t;

typedef struct
{
    uint8_t
        pin:8;
    GPIO_Port_TypeDef
        port:7;
    emlib_gpio_state_t
        state:1;
} EMLIB_GPIO_T, emlib_gpio_t;

typedef struct
{
uint8_t
    protocol,
    address;
I2C_TypeDef *
    device;
void *
    buffer;
} emlib_i2c_host_t, EMLIB_I2C_HOST_T;

typedef struct
{
emlib_i2c_host_t *
    host;
uint8_t
    command,
    reg,
    length;
    union _data
    {
        uint8_t
            byte;
        void *
            buffer;
    } data;
} emlib_i2c_event_t, EMLIB_I2C_EVENT_T;

typedef struct
{
uint8_t
    protocol;
emlib_gpio_t
    gpio;
USART_TypeDef *
    device;
void *
    buffer;
} emlib_spi_host_t, EMLIB_SPI_HOST_T;

typedef struct
{
emlib_spi_host_t *
    host;
uint8_t
    reg,
    length;
	union 
	{
		uint8_t
			byte;
		void *
			buffer;
	} data;
} emlib_spi_event_t, EMLIB_SPI_EVENT_T;

#ifndef __EMLIB__
typedef bool emlib_i2c_transfer_return_t;
typedef bool EMLIB_I2C_TRANSFER_RETURN_T;
typedef bool emlib_spi_transfer_return_t;
typedef bool EMLIB_SPI_TRANSFER_RETURN_T;

#ifdef DCDC_SERVICE
static void EMLIB_PAPIInterface_DCDC_Init( uint16_t mV ) {};
#endif
static bool EMLIB_PAPIInterface_I2C_Init( emlib_i2c_event_t * event ) { return true; };
static void EMLIB_PAPIInterface_I2C_Enable( emlib_i2c_event_t * event ) {};
static void EMLIB_PAPIInterface_I2C_Disable( emlib_i2c_event_t * event ) {};
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_ReadRegister( emlib_i2c_event_t * event ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Read( emlib_i2c_event_t * event ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_WriteRegister( emlib_i2c_event_t * event ) { return true; };
static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Write( emlib_i2c_event_t * event ) { return true; };

static bool EMLIB_PAPIInterface_SPI_Init( emlib_spi_event_t * event ) { return true; };
static void EMLIB_PAPIInterface_SPI_Enable( emlib_spi_event_t * event ) {};
static void EMLIB_PAPIInterface_SPI_Disable( emlib_spi_event_t * event ) {};
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_ReadRegister( emlib_spi_event_t * event ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Read( emlib_spi_event_t * event ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_WriteRegister( emlib_spi_event_t * event ) { return true; };
static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Write( emlib_spi_event_t * event ) { return true; };
#else
typedef I2C_TransferReturn_TypeDef emlib_i2c_transfer_return_t;
typedef I2C_TransferReturn_TypeDef EMLIB_I2C_TRANSFER_RETURN_T;

typedef uint32_t emlib_spi_transfer_return_t;
typedef uint32_t EMLIB_SPI_TRANSFER_RETURN_T;
//typedef USART_TransferReturn_TypeDef emlib_spi_transfer_return_t;
//typedef USART_TransferReturn_TypeDef EMLIB_SPI_TRANSFER_RETURN_T;

#ifdef DCDC_SERVICE
static void EMLIB_PAPIInterface_DCDC_Init( uint16_t mV )
{
//    typedef struct
//    {
//            EMU_PowerConfig_TypeDef powerConfig;
//            EMU_DcdcMode_TypeDef dcdcMode;
//            uint16_t mVout;
//            uint16_t em01LoadCurrent_mA;
//            uint16_t em234LoadCurrent_uA;
//            uint16_t maxCurrent_mA;
//            EMU_DcdcAnaPeripheralPower_TypeDef anaPeripheralPower;
//            EMU_DcdcLnReverseCurrentControl_TypeDef reverseCurrentControl;
//            EMU_DcdcLnCompCtrl_TypeDef dcdcLnCompCtrl;
//    } EMU_DCDCInit_TypeDef;
    
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    dcdcInit.mVout = mV;
    EMU_DCDCInit(&dcdcInit);
}
#endif

static void EMLIB_PAPIInterface_GPIO_Set( emlib_gpio_t gpio, emlib_gpio_state_t state )
{
    switch( state )
    {
		case EMLIB_GPIO_STATE_LOW:
            GPIO_PinOutClear( gpio.port, gpio.pin );
            break;
        case EMLIB_GPIO_STATE_HIGH:
            GPIO_PinOutSet( gpio.port, gpio.pin );
            break;
        default:
            break;
    }
}

#define DEFAULT_I2C_CLOCKHLR        i2cClockHLRStandard
#define DEFAULT_I2C_ENABLE_ON_INIT  true
#define DEFAULT_I2C_FREQUENCY       100
#define DEFAULT_I2C_MASTER_ON_INIT  true
#define DEFAULT_I2C_REF_FREQUENCY

static bool EMLIB_PAPIInterface_I2C_Init( emlib_i2c_event_t * event )
{
    I2C_Init_TypeDef initType = I2C_INIT_DEFAULT;
    I2C_Init( event->host.device, &initType );
    return true;
};

static void EMLIB_PAPIInterface_I2C_Enable( emlib_i2c_event_t * event )
{
    I2C_Enable( event->host.device, true );
};

static void EMLIB_PAPIInterface_I2C_Disable( emlib_i2c_event_t * event )
{
    I2C_Enable( event->host.device, false );
};

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_ReadRegister( emlib_i2c_event_t * event )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;

    seq.addr  = event->host->address;
    seq.flags = I2C_FLAG_WRITE_READ;
    /* Select command to issue */
    seq.buf[0].len    = event->length;
    /* Select location/length of data to be read */
    seq.buf[1].data = event->data;
    seq.buf[1].len  = event->length;

    ret = I2C_TransferInit(event->host.device, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(event->host.device);
    }
    return ret;
}

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Read( emlib_i2c_event_t * event )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;

    seq.addr  = event->host->address;
    seq.flags = I2C_FLAG_READ;
    
    /* Select location/length of data to be read */
    seq.buf[0].data = event->data.buffer;
    seq.buf[0].len  = event->length;

    ret = I2C_TransferInit(I2C0, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(I2C0);
    }
    return ret;
};

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_WriteRegister( emlib_i2c_event_t * event )
{
    event->length = 1;
    event->reg = event->reg;
    return EMLIB_PAPIInterface_I2C_Write(event->data.byte);
}

static emlib_i2c_transfer_return_t EMLIB_PAPIInterface_I2C_Write( emlib_i2c_event_t * event )
{
    I2C_TransferSeq_TypeDef    seq;
    emlib_i2c_transfer_return_t ret;
    //uint8_t                    i2c_read_data[1];

    seq.addr  = event->host->address;
    seq.flags = I2C_FLAG_WRITE;
    
    /* Select command to issue */
    seq.buf[0].data   = event->data.buffer;
    seq.buf[0].len    = event->length;

    ret = I2C_TransferInit(event->host.device, &seq);
    while (ret == i2cTransferInProgress)
    {
        ret = I2C_Transfer(event->host.device);
    }
    return ret;
};

static bool EMLIB_PAPIInterface_SPI_Init( emlib_spi_event_t * event )
{
    USART_InitSync_TypeDef initType = USART_INITASYNC_DEFAULT;
    USART_InitSync( event->host.device, &initType );
    return true;
};

static void EMLIB_PAPIInterface_SPI_Enable( emlib_spi_event_t * event )
{
    USART_Enable( event->host.device, usartEnable);
};

static void EMLIB_PAPIInterface_SPI_Disable( emlib_spi_event_t * event )
{
    USART_Enable( event->host.device, usartDisable);
};

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_ReadRegister( emlib_spi_event_t * event )
{
    EMLIB_PAPIInterface_GPIO_Set( event->host->gpio, EMLIB_GPIO_NCS_ACTIVE );
    event->length = 1;
    event->data.byte = USART_SpiTransfer( event->host.device, event->reg );
    EMLIB_PAPIInterface_GPIO_Set( event->host->gpio, EMLIB_GPIO_NCS_INACTIVE );
    return true;
}

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Read( emlib_spi_event_t * event )
{
///TODO: Finish implementing
//    emlib_spi_transfer_return_t ret;
//    uint8_t data = USART_RxDataGet( event->host.device );
//    event->length = 1;
//    event->buffer = data;
    return true;
};

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_WriteRegister( emlib_spi_event_t * event )
{
    EMLIB_PAPIInterface_GPIO_Set( event->host->gpio, EMLIB_GPIO_NCS_ACTIVE );
    event->length = 1;
    USART_SpiTransfer( event->host.device, event->reg );
    USART_SpiTransfer( event->host.device, event->data );
    EMLIB_PAPIInterface_GPIO_Set( event->host->gpio, EMLIB_GPIO_NCS_INACTIVE );
    return true;
}

static emlib_spi_transfer_return_t EMLIB_PAPIInterface_SPI_Write( emlib_spi_event_t * event )
{
//    SPI_TransferSeq_TypeDef    seq;
//    emlib_spi_transfer_return_t ret;
//
//    return ret;
};

#endif /* __EMLIB__ */

#endif /* emlib_interface_h */
