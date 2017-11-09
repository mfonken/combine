/*
 * clock_sp.h
 *
 *  Created on: Jan 15, 2017
 *      Author: Matthew Fonken
 */

#ifndef SYSTEM_UTILITIES_CLOCK_SP_H_
#define SYSTEM_UTILITIES_CLOCK_SP_H_

#include <stdint.h>
//#include "em_rtcc.h"
//#include "em_cmu.h"

#define SYSCLK_CHANNEL 	1

#define	RTCC_MAX_COUNT		0xffffffff
#define RTCC_OVER_FLOW_DIFF	0x10000000

void	 SYSCLK_Init( void );
uint32_t timestamp( void );
double   seconds_since( uint32_t );

#endif /* SYSTEM_UTILITIES_CLOCK_SP_H_ */
