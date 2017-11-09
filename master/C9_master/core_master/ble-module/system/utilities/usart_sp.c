/***********************************************************************************************//**
 * \file   usart_sp.c
 * \brief  USART Special Functions
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/
#include <stdio.h>
#include <math.h>
/* em headers */
#include "em_usart.h"
#include "em_i2c.h"

#include "usart_sp.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup sp
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 Function Declarations
 **************************************************************************************************/
char* itoa(int val, int base)
{
	static char buf[32] = {0};
	if( !val )
	{
		buf[1] = '0';
		return &buf[1];
	}
	int i = 30;
	for(; val && i ; --i, val /= base) buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

//	for(int i = 30; i ; --i, c /= b)
//	{
//		s[i-1] = "0123456789abcdef"[c % b];
//	}
//}
/***********************************************************************************************//**
 *  \brief  Print character
 *  \param[in] c Character to print
 **************************************************************************************************/
uint8_t Read_Char( void )
{
	uint8_t c = USART_Rx( USART0 );
	return c;
}

/***********************************************************************************************//**
 *  \brief  Print character
 *  \param[in] c Character to print
 **************************************************************************************************/
void Print_Char( uint8_t c )
{
	USART_Tx( USART0, c );
}

/***********************************************************************************************//**
 *  \brief  Print string
 *  \param[in] s String to print
 *  \param[in] len Length of string
 **************************************************************************************************/
void Print_String( char s[] )
{
    uint8_t l = 0;
    for(; l < MAX_STRING_LENGTH; l++)
        if( s[l] == '\0' ) break;
	for( int i = 0; i < l; i++ )
		USART_Tx( USART0, s[i] );
}

void Print_Line( char s[] )
{
    Print_String( s );
    Print_Char('\r');
    Print_Char('\n');
}

void Print_Hex( uint8_t c )
{
	Print_String( "0x" );
	char * s = itoa(c, 16);
	Print_String( s );
}

void Print_Int( int c )
{
	char * s = itoa(c, 10);
	Print_String( s );
}

/***********************************************************************************************//**
 *  \brief  Print two byte integer
 *  \param[in] v integer to print
 **************************************************************************************************/
void Print_Double_Ascii( double v )
{
	uint8_t output[9];
	uint8_t len = sprintf( ( char * )output,"%.3f", v);
	Print_String( (char *)output );
}


/** @} (end addtogroup sp) */
/** @} (end addtogroup Application) */
