////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		typename T_SPI,
		T_SPI &C_SPI
	> class ArduinoBasicSPI
	{
	public:
		inline uint16_t transfer16(uint16_t data)
		{
			return C_SPI.transfer16( data );
		}

		inline uint8_t transfer(uint8_t data)
		{
//			Serial.println( data, HEX );
			return C_SPI.transfer( data );
		}

		inline void transfer( const void *buf, size_t count )
		{
#if defined(VISUINO_ESP8266) || defined( VISUINO_ANY_ESP32 )
			C_SPI.writeBytes( (uint8_t*)buf, count );
#else // VISUINO_ESP8266
			C_SPI.transfer( (void *)buf, count );
#endif // VISUINO_ESP8266
		}

		inline void beginTransaction(SPISettings settings)
		{
		#if defined(SPI_HAS_TRANSACTION)
			C_SPI.beginTransaction( settings );
		#endif
		}

		inline void endTransaction()
		{
		#if defined(SPI_HAS_TRANSACTION)
			C_SPI.endTransaction();
		#endif
		}

#if ( defined (__AVR__) || defined (__SAM3X8E__))
		inline void setDataMode( uint8_t dataMode )
		{
			C_SPI.setDataMode( dataMode );
		}

/*
		inline void setBitOrder( uint8_t bitOrder )
		{
			C_SPI.setBitOrder( bitOrder );
		}
*/
		inline void setClockDivider( uint8_t clockDiv )
		{
			C_SPI.setClockDivider( clockDiv );
		}
#endif
		inline void usingInterrupt( uint8_t interruptNumber )
		{
#if ! ( defined(VISUINO_ESP8266) || defined( VISUINO_ANY_ESP32 ) || defined(VISUINO_STMDUINO) )
			C_SPI.usingInterrupt( interruptNumber );
#endif
		}

	public:
		inline void SystemInit()
		{
			C_SPI.begin();
		}

	};
//---------------------------------------------------------------------------
	template <SPIClass &C_SPI> class ArduinoSPI :
		public ArduinoBasicSPI<SPIClass, C_SPI>
	{
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
