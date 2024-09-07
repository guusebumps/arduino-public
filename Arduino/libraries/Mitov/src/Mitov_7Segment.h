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
#include <Mitov_7SegmentElements.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedSegments,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6
	> class Display7Segment :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedSegments,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6
	{
	public:
		_V_PROP_( InvertedSegments )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;
			UpdateDisplay();
		}

	public:
		inline void SystemInit()
		{
			UpdateDisplay();
		}

	public:
		void UpdateDisplay()
		{
			uint8_t AValue = ( Enabled() ) ? pgm_read_byte( C_LED_7SegmentDigitSegments + ( InitialValue().GetValue() & 0x0F )) : 0x00;

			for( int i = 0; i < 7; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		int C_COUNT_DigitPins,
		typename T_DigitPins_SetPinValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_LeadingZeroes,
		typename T_RefreshInterval,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6
	> class DisplayIntegerDynamic7Segment :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_LeadingZeroes,
		public T_RefreshInterval,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( InvertedDigits )
		_V_PROP_( InvertedSegments )
		_V_PROP_( LeadingZeroes )
		_V_PROP_( RefreshInterval )

	protected:
		unsigned long	FLastTime = 0;
		uint8_t			FDigit = 0;

	protected:
		inline int	GetSegmentsValue()
		{
			int AValue;
			if( InitialValue() > pow( 10, C_COUNT_DigitPins ) - 1 )
				AValue = 0b0000001;

			else if( InitialValue() < -( pow( 10, C_COUNT_DigitPins - 1 ) - 1 ) )
				AValue = 0b0001000;

			else
			{
				uint32_t APower = pow( 10, FDigit );
				AValue = ( (int)abs( InitialValue() ) ) / APower;
				if( ( InitialValue() < 0 ) && ( FDigit > 0 ))
				{
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
					{
						uint32_t APower1 = pow( 10, FDigit - 1 );
						int APreviousValue = ( - InitialValue() ) / APower1;
						if( APreviousValue > 0 )
							AValue = 0b1000000;

						else
							AValue = 0;

					}
					else
					{
						if( FDigit == C_COUNT_DigitPins - 1 )
							AValue = 0b1000000;

						else
						{
							int ADigitValue = AValue % 10;
							AValue = pgm_read_byte( C_LED_7SegmentDigitSegments + ADigitValue );
						}
					}
				}
				else
				{
					if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
						AValue = 0;

					else
					{
						int ADigitValue = AValue % 10;
						AValue = pgm_read_byte( C_LED_7SegmentDigitSegments + ADigitValue );
					}
				}
			}

			return AValue;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;
		}

	public:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
				AValue = GetSegmentsValue();

			else
				AValue = 0;

			bool ABitValue = false;
			ABitValue ^= InvertedDigits().GetValue();
			for( int i = 0; i < C_COUNT_DigitPins; ++i )
				T_DigitPins_SetPinValue::Call( i, ABitValue );

			ComponentsHardware::SystemUpdateHardware();

			for( int i = 0; i < 7; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}

/*
			for( int i = 0; i < 16; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				SegmentPins[ i ].Notify( &ABitValue );
				AValue >>= 1;
			}
*/
			AValue = FDigit;
			for( int i = 0; i < C_COUNT_DigitPins; ++i )
			{
				bool ABitValue = ( AValue-- == 0 );
				ABitValue ^= InvertedDigits().GetValue();
				T_DigitPins_SetPinValue::Call( i, ABitValue );
			}
		}

	public:
		inline void SystemInit()
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
	template <
		int C_COUNT_DigitPins,
		typename T_DigitPins_SetPinValue,
		typename T_Enabled,
		typename T_FModified,
		typename T_InitialValue,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_Precision,
		typename T_RefreshInterval,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6,
		typename T_SegmentPins_7
	> class DisplayAnalogDynamic7Segment :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_Precision,
		public T_RefreshInterval,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6,
		public T_SegmentPins_7
	{
	public:
		_V_PROP_( Precision )
		_V_PROP_( InvertedSegments )
		_V_PROP_( InvertedDigits )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( RefreshInterval )

	protected:
		_V_PROP_( FModified )

	protected:
		uint8_t	FBuffer[ C_COUNT_DigitPins ];

	protected:
		unsigned long	FLastTime = 0;
		uint8_t			FDigit = 0;

	protected:
		int	GetSegmentsValue()
		{
			if( FModified() )
			{

				char *ATextBuffer = new char[ 15 + Precision() ];

				dtostrf( InitialValue(), 1, Precision(), ATextBuffer );
//				Serial.println( FStartChar );
				uint8_t ATextLength = strlen( ATextBuffer );
	//				FTextLength = strlen( FStartChar ); 
	//				Serial.println( FTextLength );
				int8_t ADecimalPointPos;
				char *ACharPtr = strchr( ATextBuffer, '.' );
				if( ! ACharPtr )
					ADecimalPointPos = -1;

				else
					ADecimalPointPos = uint8_t( ACharPtr - ATextBuffer );

				uint8_t AFirstPos;

				if( Precision() == 0 )
				{
					if( ATextLength <= C_COUNT_DigitPins )
						AFirstPos = ATextLength - 1;

					else
						AFirstPos = C_COUNT_DigitPins - 1;

				}

				else
				{
					if( ATextLength <= C_COUNT_DigitPins + 1 )
						AFirstPos = ATextLength - 1;

					else
						AFirstPos = C_COUNT_DigitPins + 1 - 1;
				}

				int ACorrectedTextLength = ATextLength;
				if( Precision() > 0 )
					--ACorrectedTextLength;

				for( int i = 0; i < C_COUNT_DigitPins; ++ i )
				{
					uint8_t AValue;
					if( ( ACorrectedTextLength - Precision() ) > C_COUNT_DigitPins )
					{
						if( InitialValue() > 0 )
							AValue = 0b0000001; // Overflow +

						else
							AValue = 0b0001000; // Overflow -
					}

					else
					{
						int ATextPos = AFirstPos - i;
						if( ATextPos < 0 )
							AValue = 0;

						else
						{
							if( ATextPos < 0 )
							{
								FBuffer[ i ] = 0;
								continue;
							}

							bool ADecimalPoint = ( ATextBuffer[ ATextPos ] == '.' );
							if( ATextPos <= ADecimalPointPos )
								--ATextPos;

		//					if( ADecimalPoint )
		//						--ATextPos;

							if( ATextPos < 0 )
							{
								FBuffer[ i ] = 0;
								continue;
							}

							if( ATextBuffer[ ATextPos ] == '-' )
								AValue = 0b1000000;

							else
							{
								AValue = ATextBuffer[ ATextPos ] - '0';
								AValue = pgm_read_byte( C_LED_7SegmentDigitSegments + AValue );
							}

							if( ADecimalPoint )
								AValue |= 0x80;

						}
					}

					FBuffer[ i ] = AValue;
				}

//				Serial.println( FFirstPos );
				delete [] ATextBuffer;
				FModified() = false;
			}

			return FBuffer[ FDigit ];
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FModified() = true;
			InitialValue() = *(float *)_Data;
		}

	public:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
				AValue = GetSegmentsValue();

			else
				AValue = 0;

			bool ABitValue = false;
			ABitValue ^= InvertedDigits().GetValue();

			for( int i = 0; i < C_COUNT_DigitPins; ++ i )
				T_DigitPins_SetPinValue::Call( i, ABitValue );

			ComponentsHardware::SystemUpdateHardware();

			for( int i = 0; i < 8; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
					case 7 : T_SegmentPins_7::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}

/*
			for( int i = 0; i < 16; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments;
				SegmentPins[ i ].Notify( &ABitValue );
				AValue >>= 1;
			}
*/
			AValue = FDigit;
			for( int i = 0; i < C_COUNT_DigitPins; ++i )
			{
				bool ABitValue = ( AValue-- == 0 );
				ABitValue ^= InvertedDigits().GetValue();
				T_DigitPins_SetPinValue::Call( i, ABitValue );
			}
		}

	public:
		inline void SystemInit()
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

	public:
		inline DisplayAnalogDynamic7Segment()
		{
			FModified() = true;
		}

	};

//---------------------------------------------------------------------------
	template <
		uint8_t C_COUNT_DigitPins,
		typename T_DigitPins_SetPinValue,
		typename T_Enabled,
		typename T_InvertedDigits,
		typename T_InvertedSegments,
		typename T_RefreshInterval,
		typename T_SegmentPins_0,
		typename T_SegmentPins_1,
		typename T_SegmentPins_2,
		typename T_SegmentPins_3,
		typename T_SegmentPins_4,
		typename T_SegmentPins_5,
		typename T_SegmentPins_6,
		typename T_SegmentPins_7
	> class DisplayDynamic7Segment :
		public T_Enabled,
		public T_InvertedDigits,
		public T_InvertedSegments,
		public T_RefreshInterval,
		public T_SegmentPins_0,
		public T_SegmentPins_1,
		public T_SegmentPins_2,
		public T_SegmentPins_3,
		public T_SegmentPins_4,
		public T_SegmentPins_5,
		public T_SegmentPins_6,
		public T_SegmentPins_7
	{
	public:
		_V_PROP_( InvertedSegments )
		_V_PROP_( InvertedDigits )
		_V_PROP_( Enabled )
		_V_PROP_( RefreshInterval )

	protected:
		unsigned long	FLastTime = 0;
		uint8_t			FDigit = 0;

	protected:
		uint8_t	FBuffer[ C_COUNT_DigitPins ];

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

	protected:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
				AValue = FBuffer[ FDigit ];

			else
				AValue = 0;

			bool ABitValue = false;
			ABitValue ^= InvertedDigits().GetValue();

			for( int i = 0; i < C_COUNT_DigitPins; ++ i )
				T_DigitPins_SetPinValue::Call( i, ABitValue );

			ComponentsHardware::SystemUpdateHardware();

			for( int i = 0; i < 8; ++i )
			{
				bool ABitValue = AValue & 1;
				ABitValue ^= InvertedSegments();
				switch( i )
				{
					case 0 : T_SegmentPins_0::SetPinValue( ABitValue ); break;
					case 1 : T_SegmentPins_1::SetPinValue( ABitValue ); break;
					case 2 : T_SegmentPins_2::SetPinValue( ABitValue ); break;
					case 3 : T_SegmentPins_3::SetPinValue( ABitValue ); break;
					case 4 : T_SegmentPins_4::SetPinValue( ABitValue ); break;
					case 5 : T_SegmentPins_5::SetPinValue( ABitValue ); break;
					case 6 : T_SegmentPins_6::SetPinValue( ABitValue ); break;
					case 7 : T_SegmentPins_7::SetPinValue( ABitValue ); break;
				}

				AValue >>= 1;
			}

			AValue = FDigit;
			for( int i = 0; i < C_COUNT_DigitPins; ++i )
			{
				bool ABitValue = ( AValue -- == 0 );
				ABitValue ^= InvertedDigits().GetValue();
				T_DigitPins_SetPinValue::Call( i, ABitValue );
			}
		}

	public:
		inline void SystemInit()
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
//---------------------------------------------------------------------------
#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif