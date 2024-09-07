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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_DataCommandOutputPin,
		typename T_ResetOutputPin,
		typename T_ResetOutputPin_RESET,
		typename T_SPISpeed
    > class Mitov_Display_GC9A01_SPI :
		public T_ChipSelectOutputPin,
		public T_DataCommandOutputPin,
		public T_ResetOutputPin,
		public T_ResetOutputPin_RESET,
		public T_SPISpeed
    {
	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( DataCommandOutputPin )
		_V_PIN_( ResetOutputPin )

	public:
		_V_PROP_( SPISpeed )

	public:
		inline constexpr bool HasPixelRead() { return false; }
		TUniColor GetPixelColor( int16_t x, int16_t y ) { return 0; }

		inline void SetResetValueHigh() // Used by the T_RESET_IMPLEMENTATION
		{
//Serial.println( "Reset High" );
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetValueLow() // Used by the T_RESET_IMPLEMENTATION
		{
//Serial.println( "Reset Low" );
			T_ResetOutputPin::SetPinValueLow();
		}

		inline void SetDataCommandHigh()
		{
//Serial.println( "DC = true" );
			T_DataCommandOutputPin::SetPinValueHigh();
		}

		inline void SetDataCommandLow()
		{
//Serial.println( "DC = false" );
			T_DataCommandOutputPin::SetPinValueLow();
		}

		inline void SetShipSeletHigh()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void WriteStart()
		{
//Serial.println( "WriteStart" );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ));
		}

		inline void WriteEnd()
		{
//Serial.println( "WriteEnd" );
			C_SPI.endTransaction();
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void Write8( uint8_t data )
		{
//    Serial.print( "W: " ); Serial.println( data, HEX );
			C_SPI.transfer( data );
		}

		inline void Write8Test( uint8_t data )
		{
//    Serial.print( "W: " ); Serial.println( data, HEX );
			C_SPI.transfer( data );
		}

		inline void WriteCount( const uint8_t *ABuffer, uint8_t ASize )
		{
			C_SPI.transfer( ABuffer, ASize );
		}

		inline void Write16( uint16_t d )
		{
			C_SPI.transfer16( d );
        }

		inline void WriteCommandData16( uint8_t r, uint16_t d )
		{
			SetDataCommandLow();

//    Serial.print( "W: " ); Serial.println( r, HEX );
			C_SPI.transfer( r );

			SetDataCommandHigh();

//    Serial.print( "W16: " ); Serial.println( d, HEX );
/*
			uint8_t ABuffer[ 2 ] =
			{
				uint8_t( d >> 8 ),
				uint8_t( d )
			};
*/
//			C_SPI.transfer( ABuffer, 2 );
			C_SPI.transfer16( d );
		}

		inline void WriteCommandData2( uint8_t r, uint16_t d1, uint16_t d2 )
		{
//			WriteStart();
			SetDataCommandLow();

//    Serial.print( "W: " ); Serial.println( r, HEX );
			C_SPI.transfer( r );

			SetDataCommandHigh();

//    Serial.print( "W16: " ); Serial.println( d1, HEX );
//    Serial.print( "W16: " ); Serial.println( d2, HEX );
//			C_SPI.transfer( d1 );
//			C_SPI.transfer( d2 );

			uint8_t ABuffer[ 4 ] = 
			{ 
				uint8_t( d1 >> 8 ),
				uint8_t( d1 ),
				uint8_t( d2 >> 8 ),
				uint8_t( d2 ),
			};

			C_SPI.transfer( ABuffer, 4 );
//			WriteEnd();
		}

		inline void w_data( uint8_t data )
		{
//			USBSerial.print( "w_data: " ); USBSerial.println( data, HEX );

			SetDataCommandHigh();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ));
			Write8( data );
			C_SPI.endTransaction();

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
