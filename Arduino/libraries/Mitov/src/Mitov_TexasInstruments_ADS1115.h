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
	enum TArduinoTexasInstrumentsADS1115Range { adsRange_6_144, adsRange_4_096, adsRange_2_048, adsRange_1_024, adsRange_0_512, adsRange_0_256 };
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Comparator,
		typename T_Differential,
		typename T_Enabled,
		typename T_InputIndex,
		typename T_OutputPin,
		typename T_Range,
		typename T_SampleRate,
		typename T_SingleShot
	> class TexasInstrumentsADS1115 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Comparator,
		public T_Differential,
		public T_Enabled,
		public T_InputIndex,
		public T_OutputPin,
		public T_Range,
		public T_SampleRate,
		public T_SingleShot
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Comparator )

		_V_PROP_( Address )
		_V_PROP_( Enabled )
		_V_PROP_( Range )
		_V_PROP_( InputIndex )
		_V_PROP_( Differential )
		_V_PROP_( SingleShot )
		_V_PROP_( SampleRate )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void SetComparatorQueueSize( uint32_t AOldValue, uint32_t AValue )
		{
			if( AValue > 4 )
				AValue = 4;

			if( AValue == 3 )
				AValue = 2;

			if( AOldValue != AValue )
			{
				Comparator().QueueSize() = AValue;
				UpdateConfig();
			}
		}

	public:
		void UpdateConfig()
		{
			static const uint16_t CSampleRates[] = { 8, 16, 32, 64, 128, 250, 475, 860 };
			uint8_t ASampleRate = 0;
			if( SampleRate().GetValue() > 860 )
				ASampleRate = 7;

			else
			{
				for( int i = 0; i < sizeof( CSampleRates ) / sizeof( CSampleRates[ 0 ] ); ++i )
					if( CSampleRates[ i ] >= SampleRate().GetValue() )
					{
						ASampleRate = i;
						break;
					}

			}

			uint16_t AValue =	( uint16_t( Range() ) << 9 ) |
								( Comparator().Latching() ? ( uint16_t( 1 ) << 2 ) : 0 ) |
								( Comparator().ActiveHigh() ? ( uint16_t( 1 ) << 3 ) : 0 ) |
								( Comparator().WindowMode() ? ( uint16_t( 1 ) << 4 ) : 0 ) |
								( uint16_t( ASampleRate ) << 5 ) |
								( ( SingleShot() && ClockInputPin_o_IsConnected() ) ? uint16_t( 1 ) << 8 : 0 ) |
								( Differential() ? 0 : ( uint16_t( 1 ) << 14 ) ) |
								( uint16_t( InputIndex() ) << 12 );

			if( ! Comparator().Enabled() )
				AValue |= 0b11;

			else
			{
				if( Comparator().QueueSize() == 4 )
					AValue |= 0x10;

				else
					AValue |= Comparator().QueueSize() - 1;
			}

			writeRegister( ADS1X1XConst::REG_POINTER_CONFIG, AValue );

			if( Comparator().UseAsReady() )
			{
				writeRegister( ADS1X1XConst::REG_POINTER_LOWTHRESH, 0x8000 );
				writeRegister( ADS1X1XConst::REG_POINTER_HITHRESH, 0x7FFF );
			}

			else
			{
				uint16_t AHigh = Comparator().ThresholdHigh() * 0x7FFF + 0.5;
				uint16_t ALow = Comparator().ThresholdLow() * 0x7FFF + 0.5;

				if( ALow > AHigh )
				{
					writeRegister( ADS1X1XConst::REG_POINTER_LOWTHRESH, AHigh );
					writeRegister( ADS1X1XConst::REG_POINTER_HITHRESH, ALow );
				}

				else
				{
					writeRegister( ADS1X1XConst::REG_POINTER_LOWTHRESH, ALow );
					writeRegister( ADS1X1XConst::REG_POINTER_HITHRESH, AHigh );
				}
			}
		}

	protected:
		void writeRegister( uint8_t AReg, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( uint8_t( AReg ));
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.write( uint8_t( AValue & 0xFF ));
			C_I2C.endTransmission();
		}

		int16_t readRegister()
		{
			C_I2C.beginTransmission( uint8_t( Address().GetValue() ) );
			C_I2C.write( ADS1X1XConst::REG_POINTER_CONVERT );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address().GetValue() ), _VISUINO_I2C_SIZE_( 2 ));
			return ((int16_t( int8_t( C_I2C.read() )) << 8) | C_I2C.read());
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			int16_t	AValue = readRegister(); // >> 4;
//			Serial.println( AValue );

			float AFloatValue = float( AValue ) / 0x7FFF;

			T_OutputPin::SetPinValue( AFloatValue );

//			Serial.println( "ClockInputPin_o_Receive" );
//			CurrentValue = InitialValue;
//			UpdateValue();
		}

	public:
		inline void SystemStart()
		{
			UpdateConfig();
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
