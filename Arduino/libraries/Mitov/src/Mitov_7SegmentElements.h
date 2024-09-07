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

#ifdef _VISUINO_LED_TEXT_
#include <Mitov_LED_7SegmentText.h>
#endif // _VISUINO_LED_TEXT_

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
#ifndef _VISUINO_LED_TEXT_
	static const uint8_t PROGMEM C_LED_7SegmentDigitSegments[ 16 ] =
	{
		0b00111111, // 0x3F, // 0 
		0b00000110, // 0x06, // 1 
		0b01011011, // 0x5B, // 2 
		0b01001111, // 0x4F, // 3 
		0b01100110, // 0x66, // 4 
		0b01101101, // 0x6D, // 5 
		0b01111101, // 0x7D, // 6 
		0b00000111, // 0x07, // 7 
		0b01111111, // 0x7F, // 8 
		0b01101111, // 0x6F, // 9 
		0b01110111, // 0x77, // A 
		0b01111100, // 0x7C, // B 
		0b00111001, // 0x39, // C 
		0b01011110, // 0x5E, // D 
		0b01111001, // 0x79, // E 
		0b01110001  // 0x71  // F 
	};
#endif // _VISUINO_LED_TEXT_
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue
	> class LED_Segments :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		uint8_t	FValues;

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( Enabled() )
				C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );

			else
				C_OWNER.SetDigitSegments( C_START_DIGIT, 0x00 );

		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( Func::SetBitField( &FValues, AIndex, AValue ) )
				if( Enabled() )
					C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );

		}

	public:
		inline void SystemInit()
		{
			if( InitialValue() )
				FValues = 0xFF;

			else
				FValues = 0;

			C_OWNER.SetDigitSegments( C_START_DIGIT, FValues );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialDecmalPointValue,
		typename T_InitialValue
	> class LED_ValueSection7Segments :
		public T_Enabled,
		public T_InitialDecmalPointValue,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialDecmalPointValue )
		_V_PROP_( InitialValue )

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
//			Serial.println( pgm_read_byte( C_LED_7SegmentDigitSegments ), BIN );

			uint8_t AValue = ( Enabled() ) ? pgm_read_byte( C_LED_7SegmentDigitSegments + ( InitialValue().GetValue() & 0xF )) | (( InitialDecmalPointValue() & 1 ) ? 0b10000000 : 0 ) : 0;
//			uint8_t AValue = ( Enabled() ) ? 0 | C_LED_7SegmentDigitSegments[ InitialDecmalPointValue() & 1 ] : 0;
			C_OWNER.SetDigitSegments( C_START_DIGIT, AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
			if( AValue < 0 )
				AValue = 0;

			if( AValue > 0xF )
				AValue = 0xF;

			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
			UpdateDisplay();
		}

		inline void DecmalPointInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			if( InitialDecmalPointValue().GetValue() == AValue )
				return;

			InitialDecmalPointValue() = AValue;
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_LeadingZeroes
	> class LED_IntegerDisplay7Segments :
		public T_Enabled,
		public T_InitialValue,
		public T_LeadingZeroes
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( LeadingZeroes )

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( ! Enabled() )
			{
				C_OWNER.ZeroDigits( C_START_DIGIT, C_COUNT_DIGITS );
				return;
			}

			C_OWNER.StartSendingDigits( C_START_DIGIT );

			for( int ADigit = 0; ADigit < C_COUNT_DIGITS; ++ ADigit )
			{
				int AValue;
				if( InitialValue().GetValue() > ( pow( 10, C_COUNT_DIGITS ) - 1 ) )
					AValue = 0b0000001; // Overflow +

				else if( InitialValue().GetValue() < ( -( pow( 10, C_COUNT_DIGITS - 1 ) - 1 ) ) )
					AValue = 0b0001000; // Overflow -

				else
				{
					AValue = int32_t( abs( InitialValue().GetValue() ) ) / ( pow( 10, ADigit ));
	//				Serial.print( ADigit ); Serial.print( " - " ); Serial.println( AValue );
					if( ( InitialValue().GetValue() < 0 ) && ( ADigit > 0 ))
					{
						if( ( ! LeadingZeroes() ) && ( AValue == 0 ) )
						{
							int APreviousValue = ( - InitialValue().GetValue() ) / ( pow( 10, ADigit - 1  ));
							if( APreviousValue > 0 )
								AValue = 0b1000000; // -

							else
								AValue = 0;

						}

						else
						{
							if( ADigit == C_COUNT_DIGITS - 1 )
								AValue = 0b0000001;

							else
							{
								int ADigitValue = AValue % 10;
								AValue = pgm_read_byte( C_LED_7SegmentDigitSegments + ADigitValue );
							}
						}
					}

					else if( ( InitialValue().GetValue() == 0 ) && ( ADigit == 0 ) )
						AValue = pgm_read_byte( C_LED_7SegmentDigitSegments );

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

				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, AValue );
			}

			C_OWNER.EndSendingDigits();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	public:
		inline void SystemStart() 
		{
			UpdateDisplay();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Precision
	> class LED_AnalogDisplay7Segments :
		public T_Enabled,
		public T_InitialValue,
		public T_Precision
	{
	public:
		_V_PROP_( Precision )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart() 
		{
			UpdateDisplay();
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			if( ! Enabled() )
			{
				C_OWNER.ZeroDigits( C_START_DIGIT, C_COUNT_DIGITS );
				return;
			}

			C_OWNER.StartSendingDigits( C_START_DIGIT );

			char *ATextBuffer = new char[ 15 + Precision() ];

			dtostrf( InitialValue(), 1, Precision(), ATextBuffer );
//				Serial.print( "FStartChar = " );
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
				if( ATextLength <= C_COUNT_DIGITS )
					AFirstPos = ATextLength - 1;

				else
					AFirstPos = C_COUNT_DIGITS - 1;

				
//					Serial.println( FStartChar );
//					Serial.println( FTextLength );
//					Serial.println( FFirstPos );
//					Serial.println( FDecimalPointPos );
			}

			else
			{
				if( ATextLength <= C_COUNT_DIGITS + 1 )
					AFirstPos = ATextLength - 1;

				else
					AFirstPos = C_COUNT_DIGITS + 1 - 1;
			}				

			for( int ADigit = 0; ADigit < C_COUNT_DIGITS; ++ ADigit )
			{
				int ACorrectedTextLength = ATextLength;
				if( Precision() > 0 )
					--ACorrectedTextLength;

                uint8_t AValue;
				if( ( ACorrectedTextLength - Precision() ) > C_COUNT_DIGITS )
				{
					if( InitialValue().GetValue() > 0 )
						AValue = 0b0000001; // Overflow +

					else
						AValue = 0b0001000; // Overflow -
				}

				else
				{
					int ATextPos = AFirstPos - ADigit;
					if( ATextPos < 0 )
						AValue = 0;

					else
					{
						if( ATextPos < 0 )                            
                        {
               				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, 0 );
							continue;
                        }

						bool ADecimalPoint = ( ATextBuffer[ ATextPos ] == '.' );
						if( ATextPos <= ADecimalPointPos )
							--ATextPos;

						else if( ADecimalPoint )
							--ATextPos;

						if( ATextPos < 0 )
                        {
               				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, 0 );
							continue;
                        }

	//					Serial.print( "ATextPos = " );
	//					Serial.println( ATextPos );
	/*
						if( FDigit == 0 )
						{
							Serial.println( FStartChar );
							Serial.println( ATextPos );
						}
	*/
						if( ATextBuffer[ ATextPos ] == '-' )
							AValue = 0b1000000;

						else
						{
							AValue = ATextBuffer[ ATextPos ] - '0';
							AValue = pgm_read_byte( C_LED_7SegmentDigitSegments + AValue );
						}

						if( ADecimalPoint )
							AValue |= 0x80;

	//					Serial.println( ADigit );
	//					Serial.println( AValue, HEX );
					}
				}

				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, AValue );
			}

			delete [] ATextBuffer;
			C_OWNER.EndSendingDigits();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
