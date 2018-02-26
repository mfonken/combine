/*
 * clock_sp.c
 *
 *  Created on: Jan 15, 2017
 *      Author: Matthew Fonken
 */

/* Own header */
#include "clock_sp.h"

/* Defines*/
#define START_TIME      0x00235945      /* 23:59:45 */
#define START_DATE      0x06160228      /* 2016 Feb 28 */

#define LFRCO_FREQUENCY                 32768
#define WAKEUP_INTERVAL_MS              500
#define RTC_COUNT_BETWEEN_WAKEUP        (((LFRCO_FREQUENCY * WAKEUP_INTERVAL_MS) / 1000)-1)

/**************************************************************************//**
 * @brief  Start LFRCO for RTC/RTCC
 * Starts the low frequency RC oscillator (LFRCO) and routes it to the RTC/RTCC
 *****************************************************************************/
void startLfrcoForRtc(void)
{
  /* Starting LFRCO and waiting until it is stable */
  CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

  /* Enabling clock to the interface of the low energy modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

#if defined(RTCC_PRESENT) && (RTCC_COUNT > 0)
  /* Routing the LFRCO clock to the RTCC */
  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_RTCC, true);
#else
  /* Routing the LFRCO clock to the RTC */
  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_RTC, true);
#endif
}

/**************************************************************************//**
 * @brief  Setup RTC/RTCC
 * On compare match with compare channel, clear counter and set interrupt
 *****************************************************************************/
void SYSCLK_Init( void )
{
	/* Enable LFE domain for RTCC */
	//CMU_ClockEnable(cmuClock_CORELE, true);
	//CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_HFXO);

	/* Enable RTC clock */
	CMU_ClockEnable(cmuClock_RTCC, true);

	RTCC_Init_TypeDef rtccInit = RTCC_INIT_DEFAULT;
	rtccInit.enable   = false;
	//rtccInit.presc = rtccCntPresc_8192;
	//rtccInit.cntMode = rtccCntModeNormal;
	RTCC_Init(&rtccInit);


	/* Enable required interrupt */
	//RTCC_IntEnable(RTCC_IEN_CNTTICK);// + RTCC_IEN_DAYTICK);

	/* Enable RTCC interrupt */
	//NVIC_ClearPendingIRQ(RTCC_IRQn);
	//NVIC_EnableIRQ(RTCC_IRQn);

	/* Start Counter */
	RTCC_Enable(true);
}

uint32_t timestamp( void )
{
	return RTCC_CounterGet();//RTCC_TimeGet();
}

double seconds_since( uint32_t time )
{
	uint32_t curr = timestamp();
	uint32_t diff = curr - time;
	double ret;
	if( diff < RTCC_OVER_FLOW_DIFF ) ret = (double)diff; /* TODO: Consider bit shift? */
	else 							 ret = (double)( RTCC_MAX_COUNT - time + curr );
	return ret / 1000;
}
