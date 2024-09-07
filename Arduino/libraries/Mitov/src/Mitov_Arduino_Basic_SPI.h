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
/*
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin
	> class InterfaceSPI :
		public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		inline void Init()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void WriteRegisters_Progmem( uint8_t ARegister, const uint8_t *AValues, uint32_t ASize )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister );
			for( int i = 0; i < ASize; ++ i )
				C_SPI.transfer( pgm_read_byte( AValues ++ ) );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister8( uint8_t ARegister, uint8_t AValue )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( AValue );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t AValue )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( uint8_t( AValue ));
			C_SPI.transfer( uint8_t( AValue >> 8 ));
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16_HighFirst( uint8_t ARegister, uint16_t AValue )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister ); // write, bit 7 low

			C_SPI.transfer( uint8_t( AValue >> 8 ));
			C_SPI.transfer( uint8_t( AValue ));
			T_ChipSelectOutputPin::SetPinValueHigh();

			C_SPI.endTransaction();

		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 | 0x40 ); // write, bit 7 low
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		void EndReadRegisters()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class InterfaceSPISpeed :
		public T_ChipSelectOutputPin,
		public T_SPISpeed
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( SPISpeed )

	public:
		inline void Init()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void BeginTransaction() 
		{ 
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) ); 
			T_ChipSelectOutputPin::SetPinValueLow();
		}

		inline void EndTransaction() 
		{ 
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction(); 
		}

		inline uint8_t Transfer( uint8_t AValue ) { return C_SPI.transfer( AValue ); }

		void WriteRegisters_Progmem( uint8_t ARegister, const uint8_t *AValues, uint32_t ASize )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister );
			for( int i = 0; i < ASize; ++ i )
				C_SPI.transfer( pgm_read_byte( AValues ++ ) );

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister8( uint8_t ARegister, uint8_t AValue )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( AValue );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16( uint8_t ARegister, uint16_t AValue )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( ARegister ); // write, bit 7 low
			C_SPI.transfer( uint8_t( AValue ));
			C_SPI.transfer( uint8_t( AValue >> 8 ));
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

		void WriteRegister16_HighFirst( uint8_t ARegister, uint16_t AValue )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister ); // write, bit 7 low

			C_SPI.transfer( uint8_t( AValue >> 8 ));
			C_SPI.transfer( uint8_t( AValue ));
			T_ChipSelectOutputPin::SetPinValueHigh();

			C_SPI.endTransaction();

		}

		uint8_t ReadRegister8( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AValue;
		}

		uint16_t ReadRegister16( uint8_t ARegister )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ARegister | 0x80 ); // write, bit 7 low

			uint16_t AResult = C_SPI.transfer( 0 );
			AResult |= uint16_t( C_SPI.transfer( 0 ) ) << 8;

			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();

			return AResult;
		}

		void StartReadRegisters( uint8_t ARegister, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings( SPISpeed().GetValue(), MSBFIRST, SPI_MODE0 ) );
			T_ChipSelectOutputPin::SetPinValueLow();
			C_SPI.transfer( ARegister | 0x80 /*| 0x40 */ ); // read, bit 7 high
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		inline uint16_t ReadUInt16()
		{
			uint16_t AResult = C_SPI.transfer( 0xFF );

			return ( AResult | uint16_t( C_SPI.transfer( 0xFF ) ) << 8 );
		}

		void EndReadRegisters()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
			C_SPI.endTransaction();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
