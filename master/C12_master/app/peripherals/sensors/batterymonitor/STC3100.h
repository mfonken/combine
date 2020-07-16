/* File: STC3100.h
 * Author: Matthew Fonken
 * Source: https://www.mouser.com/datasheet/2/389/STC3100-974293.pdf
 * Description:
    The STC3100 monitors the critical parameters of a single-cell Li-Ion battery (voltage, temperature and current) and includes hardware functions to implement a gas gauge for battery charge monitoring, based on a programmable 12- to 14-bit A/D converter. With a typical 30 milliOhms external sense resistor, the battery current can be up to 2.5 A and the accumulator system provides a capacity up to +/-7000 mAh with a resolution of 0.2 mAh. The device is programmable through the I2C interface.
 */

#ifndef STC3100_h
#define STC3100_h

#include "peripheralbridge.h"

#define STC3100_ADDR                0x70  // STC3100 I2C address
#define STC3100_ID                  0x10  // STC3100 type ID
#define STC3100_CRC_LENGTH_BIT      8     // Byte length of CRC
#define STC3100_TIMEBASE            32768 // Hz
#define STC3100_MV_RANGE            80    // +/-mV

#define STC3100_REG_LEN             1   // Byte length of register
#define STC3100_WORD_LEN            2   // Byte length of word

/* Control registers 0 to 23 */
#define STC3100_REG_MODE            0   // R/W Mode register
#define STC3100_REG_CTRL            1   // R/W Control and status register
#define STC3100_REG_CHARGE_LOW      2   // R Gas gauge charge data, bits 0-7
#define STC3100_REG_CHARGE_HIGH     3   // R Gas gauge charge data, bits 8-15
#define STC3100_REG_COUNTER_LOW     4   // R Number of conversions, bits 0-7
#define STC3100_REG_COUNTER_HIGH    5   // R Number of conversions, bits 8-15
#define STC3100_REG_CURRENT_LOW     6   // R Battery current value, bits 0-7
#define STC3100_REG_CURRENT_HIGH    7   // R Battery current value, bits 8-15
#define STC3100_REG_VOLTAGE_LOW     8   // R Battery voltage value, bits 0-7
#define STC3100_REG_VOLTAGE_HIGH    9   // R Battery voltage value, bits 8-15
#define STC3100_REG_TEMPERATURE_LOW 10  // R Temperature value, bits 0-7
#define STC3100_REG_TEMPERATURE_HIGH 11 // R Temperature value, bits 8-15

/* Device ID registers 24 to 31 */
//#define STC3100_REG_ID_LEN          8   // ID length in bytes
#define STC3100_REG_ID0             24  // R Part type ID = 10h
#define STC3100_REG_ID1             25  // R Unique part ID, bits 0-7
#define STC3100_REG_ID2             26  // R Unique part ID, bits 8-15
#define STC3100_REG_ID3             27  // R Unique part ID, bits 16-23
#define STC3100_REG_ID4             28  // R Unique part ID, bits 24-31
#define STC3100_REG_ID5             29  // R Unique part ID, bits 32-39
#define STC3100_REG_ID6             30  // R Unique part ID, bits 40-47
#define STC3100_REG_ID7             31  // R Device ID CRC

/* From - https://github.com/st-sw/STC3100_GenericDriver/blob/master/src/stc3100.c */
#define STC3100_OK 0
#define SENSERESISTOR  33

/* --- constants ---------------------------------------------------------- */
#define CurrentFactor  (48210/SENSERESISTOR)
// LSB=11.77uV/R= ~48210/R/4096 - convert to mA

#define ChargeCountFactor  (27443/SENSERESISTOR)
// LSB=6.7uVh/R ~27443/R/4096 - converter to mAh

#define VoltageFactor  9994
// LSB=2.44mV ~9994/4096 - convert to mV

#define TemperatureFactor  5120
// LSB=0.125°C ~5120/4096 - convert to 0.1°C

typedef long STC3100_id_t;

/* RAM Register 32 - 63 */
#define STC3100_REG_RAM_LEN         32  // RAM number of registers
#define STC3100_REG_RAM_START       32  // R/W General-purpose RAM registers 0-31

/* REG MODE - Regoister 0 */
typedef struct
{
uint8_t
    SEL_EXT_CLK:1,  // 0    R/W 0   32,768 Hz clock source: 0: auto-detect, 1: external clock
    GG_RES:2,       // 1-2  R/W 00  Gas gauge ADC resolution: 00:14 bits, 01:13 bits, 10:12 bits
    GG_CAL:1,       // 3    R/W 0   0: no effect 1: used to calibrate the AD converters
    GG_RUN:1,       // 4    R/W 0   0: standby mode. Accumulator and counter registers are frozen, gas gauge and battery monitor functions are in standby. 1: operating mode.
    RESERVED:3;     // 5-7  Unused
} stc3100_reg_mode_t;

/* REG CTRL - Register 1 */
typedef struct
{
uint8_t
    IO0DATA:1,      // 0    R   X   Port IO0 data status: 0 = IO0 input is low, 1 = IO0 input is high
                    //      W   1   Port IO0 data output drive: 0 = IO0 output is driven low,1 = IO0 output is open
    GG_RST:1,       // 1    W   0   0: no effect 1: resets the charge accumulator and conversion counter. GG_RST is a self-clearing bit.
    GG_EOC:1,       // 2    R   1   Set at the end of a battery current conversion cycle. Clears upon reading.
    VTM_EOC:1,      // 3    R   1   Set at the end of a battery voltage or temperature conversion cycle. Clears upon reading.
    PORDET:1,       // 4    R   1   Power on reset (POR) detection bit: 0 = no POR event occurred, 1 = POR event occurred
                    //      W   0   Soft reset: 0 = release the soft-reset and clear the POR detection bit, 1 = assert the soft-reset and set the POR detection bit.
    RESERVED:3;     // 5-7  Unused
} stc3100_reg_ctrl_t;

/* STC3100 basic structure */
typedef struct
{
    stc3100_reg_mode_t mode;
    stc3100_reg_ctrl_t ctrl;
} stc3100_t;

//emlib_i2c_host_t *
//    host;
//uint8_t
//    command,
//    reg,
//    length;
//    union _data
//    {
//        uint8_t
//            byte;
//        void *
//            buffer;
//    } data;
//{ return (i2c_event_t){ (i2c_host_t *)host, I2C_WRITE_REG_EVENT, STC3100_REG_MODE,             STC3100_REG_LEN,    .data.buffer = b }; }
/* I2C Events */
static inline i2c_event_t STC3100SetModeEvent(    comm_host_t * p_host, stc3100_reg_mode_t   data )  { return I2C_WR_REG( p_host, STC3100_REG_MODE,            data ); }
static inline i2c_event_t STC3100GetModeEvent(    comm_host_t * p_host, stc3100_reg_mode_t * p_data )  { return I2C_RD_REG( p_host, STC3100_REG_MODE,            p_data ); }
static inline i2c_event_t STC3100SetControlEvent( comm_host_t * p_host, stc3100_reg_ctrl_t   data )  { return I2C_WR_REG( p_host, STC3100_REG_CTRL,            data ); }
static inline i2c_event_t STC3100GetControlEvent( comm_host_t * p_host, stc3100_reg_ctrl_t * p_data )  { return I2C_RD_REG( p_host, STC3100_REG_CTRL,            p_data ); }
static inline i2c_event_t STC3100GetIDEvent(          comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_REG( p_host, STC3100_REG_ID0,             p_data ); }
static inline i2c_event_t STC3100GetRAMEvent(         comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_REG( p_host, STC3100_REG_RAM_START,       p_data ); }
static inline i2c_event_t STC3100GetChargeEvent(      comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_WD(  p_host, STC3100_REG_CHARGE_LOW,      p_data ); }
static inline i2c_event_t STC3100GetCounterEvent(     comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_WD(  p_host, STC3100_REG_COUNTER_LOW,     p_data ); }
static inline i2c_event_t STC3100GetCurrentEvent(     comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_WD(  p_host, STC3100_REG_CURRENT_LOW,     p_data ); }
static inline i2c_event_t STC3100GetVoltageEvent(     comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_WD(  p_host, STC3100_REG_VOLTAGE_LOW,     p_data ); }
static inline i2c_event_t STC3100GetTemperatureEvent( comm_host_t * p_host, uint8_t * p_data )         { return I2C_RD_WD(  p_host, STC3100_REG_TEMPERATURE_LOW, p_data ); }

static void STC3100SetMode( comm_host_t * p_host, stc3100_reg_mode_t data )
{
    PerformI2CEvent(STC3100SetModeEvent( p_host, data ));
}
static stc3100_reg_mode_t STC3100GetMode( comm_host_t * p_host )
{
    stc3100_reg_mode_t mode;
    PerformI2CEvent(STC3100GetModeEvent( p_host, &mode ) );
    return mode;
}
static void STC3100SetControl( comm_host_t * p_host, stc3100_reg_ctrl_t data )
{
    PerformI2CEvent(STC3100SetControlEvent( p_host, data ));
}
static stc3100_reg_ctrl_t STC3100GetControl( comm_host_t * p_host )
{
    stc3100_reg_ctrl_t reg_ctrl;
    PerformI2CEvent(STC3100GetControlEvent( p_host, &reg_ctrl ));
    return reg_ctrl;
}
static uint8_t STC3100GetID( comm_host_t * p_host )
{
    uint8_t IDE;
    PerformI2CEvent(STC3100GetIDEvent( p_host, &IDE ));
    return IDE;
}
static uint8_t STC3100GetRAM( comm_host_t * p_host )
{
    uint8_t RAM;
    PerformI2CEvent(STC3100GetRAMEvent( p_host, &RAM ) );
    return RAM;
}
static uint8_t STC3100GetCharge( comm_host_t * p_host )
{
    uint8_t charge;
    PerformI2CEvent(STC3100GetChargeEvent( p_host, &charge ) );
    return charge;
}
static uint8_t STC3100GetCounter( comm_host_t * p_host )
{
    uint8_t counter;
    PerformI2CEvent(STC3100GetCounterEvent( p_host, &counter ) );
    return counter;
}
static uint8_t STC3100GetCurrent( comm_host_t * p_host )
{
    uint8_t current;
    PerformI2CEvent(STC3100GetCurrentEvent( p_host, &current ) );
    return current;
}
static uint8_t STC3100GetVoltage( comm_host_t * p_host )
{
    uint8_t voltage;
    PerformI2CEvent(STC3100GetVoltageEvent( p_host, &voltage ) );
    return voltage;
}
static uint8_t STC3100GetTemperature( comm_host_t * p_host )
{
    uint8_t temperature;
    PerformI2CEvent(STC3100GetTemperatureEvent( p_host, &temperature ) );
    return temperature;
}

typedef struct
{
i2c_event_t
    (*SetModeEvent)( comm_host_t *, stc3100_reg_mode_t ),
    (*GetModeEvent)( comm_host_t *, stc3100_reg_mode_t * ),
    (*SetControlEvent)( comm_host_t *, stc3100_reg_ctrl_t ),
    (*GetControlEvent)( comm_host_t *, stc3100_reg_ctrl_t * ),
    (*GetIDEvent)( comm_host_t *, uint8_t * ),
    (*GetRAMEvent)( comm_host_t *, uint8_t * ),
    (*GetChargeEvent)( comm_host_t *, uint8_t * ),
    (*GetCounterEvent)( comm_host_t *, uint8_t * ),
    (*GetCurrentEvent)( comm_host_t *, uint8_t * ),
    (*GetVoltageEvent)( comm_host_t *, uint8_t * ),
    (*GetTemperatureEvent)( comm_host_t *, uint8_t * );
    void (*SetMode)( comm_host_t *, stc3100_reg_mode_t );
    stc3100_reg_mode_t (*GetMode)( comm_host_t * );
    void (*SetControl)( comm_host_t *, stc3100_reg_ctrl_t );
    stc3100_reg_ctrl_t (*GetControl)( comm_host_t * );
uint8_t
    (*GetIDE)( comm_host_t * ),
    (*GetRAM)( comm_host_t * ),
    (*GetCharge)( comm_host_t * ),
    (*GetCounter)( comm_host_t * ),
    (*GetCurrent)( comm_host_t * ),
    (*GetVoltage)( comm_host_t * ),
    (*GetTemperature)( comm_host_t * );
} stc3100_functions;

static stc3100_functions STC3100Functions =
{
    .SetMode             = STC3100SetMode,
    .GetMode             = STC3100GetMode,
    .SetControl          = STC3100SetControl,
    .GetControl          = STC3100GetControl,
    .GetIDE              = STC3100GetID,
    .GetRAM              = STC3100GetRAM,
    .GetCharge           = STC3100GetCharge,
    .GetCounter          = STC3100GetCounter,
    .GetCurrent          = STC3100GetCurrent,
    .GetVoltage          = STC3100GetVoltage,
    .GetTemperature      = STC3100GetTemperature,
    .SetModeEvent        = STC3100SetModeEvent,
    .GetModeEvent        = STC3100GetModeEvent,
    .SetControlEvent     = STC3100SetControlEvent,
    .GetControlEvent     = STC3100GetControlEvent,
    .GetIDEvent          = STC3100GetIDEvent,
    .GetRAMEvent         = STC3100GetRAMEvent,
    .GetChargeEvent      = STC3100GetChargeEvent,
    .GetCounterEvent     = STC3100GetCounterEvent,
    .GetCurrentEvent     = STC3100GetCurrentEvent,
    .GetVoltageEvent     = STC3100GetVoltageEvent,
    .GetTemperatureEvent = STC3100GetTemperatureEvent
};


#endif /* STC3100_h */
