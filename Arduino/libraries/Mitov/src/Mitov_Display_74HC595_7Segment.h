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
		typename T_ClockOutputPin,
		uint8_t C_COUNT_DigitPins,
		typename T_DataOutputPin,
		typename T_Enabled,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_LatchOutputPin,
		typename T_RefreshInterval,
		typename T_ReversedDigits,
		typename T_ReversedSegments,
		typename T_RightAllignedDigits
	> class Display_74HC595_7Segment :
		public T_ClockOutputPin,
		public T_DataOutputPin,
		public T_Enabled,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_LatchOutputPin,
		public T_RefreshInterval,
		public T_ReversedDigits,
		public T_ReversedSegments,
		public T_RightAllignedDigits
	{
	public:
		_V_PIN_( DataOutputPin )
		_V_PIN_( ClockOutputPin )
		_V_PIN_( LatchOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InvertedDigits )
		_V_PROP_( InvertedSegments )
		_V_PROP_( RefreshInterval )
		_V_PROP_( ReversedDigits )
		_V_PROP_( ReversedSegments )
		_V_PROP_( RightAllignedDigits )

	protected:
		unsigned long	FLastTime = 0;
		uint8_t			FDigit = 0;

	protected:
		uint8_t	FBuffer[ C_COUNT_DigitPins ];

	protected:
		void SendData( bool AValue )
		{
			T_ClockOutputPin::SetPinValueLow();

			T_DataOutputPin::SetPinValue( AValue );

			T_ClockOutputPin::SetPinValueHigh();
		}

		void UpdateDisplay()
		{
			T_LatchOutputPin::SetPinValueLow();

			T_DataOutputPin::SetPinValueLow();

			int AValue;
			if( Enabled() )
				AValue = FBuffer[ FDigit ];

			else
				AValue = 0;

			if( ReversedSegments() )
			{
				for( int i = 0; i < 8; ++ i )
				{
					bool ABitValue = AValue & 1;
					ABitValue ^= InvertedSegments();

					SendData( ABitValue );

					AValue >>= 1;
				}
			}

			else
			{
				for( int i = 0; i < 8; ++ i )
				{
					bool ABitValue = (( AValue & 0b10000000 ) != 0 );
					ABitValue ^= InvertedSegments();

					SendData( ABitValue );

					AValue <<= 1;
				}
			}

			bool ABitValue = false;
			ABitValue ^= InvertedDigits().GetValue();

			if( ReversedDigits() )
				AValue = FDigit;

			else
				AValue = C_COUNT_DigitPins - FDigit - 1;

			if( RightAllignedDigits() )
				for( int i = 0; i < C_COUNT_DigitPins % 8; ++ i )
					SendData( false );

			int i = 0;
			for( ; i < C_COUNT_DigitPins; ++ i )
			{
				bool ABitValue = ( AValue -- == 0 );
				ABitValue ^= InvertedDigits().GetValue();
				SendData( ABitValue );
			}

			if( ! RightAllignedDigits().GetValue() )
			{
				i %= 8;
				if( i )
					for( ; i < 8; ++ i )
						SendData( false );

			}

			T_LatchOutputPin::SetPinValueHigh();
		}

	public:
		void ZeroDigits( uint8_t AStartIndex, uint8_t ACount )
		{
			for( int i = AStartIndex; i < ACount; ++ i )
				FBuffer[ i ] = 0;

		}

		inline void StartSendingDigits( uint8_t AIndex ) {}

		inline void EndSendingDigits() {}

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			FBuffer[ AIndex ] = AValue;
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( ( ACurrentMicros - FLastTime ) < RefreshInterval() )
				return;

			++ FDigit;

			if( FDigit >= C_COUNT_DigitPins )
				FDigit = 0;

			FLastTime = ACurrentMicros;
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif