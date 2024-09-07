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

namespace Mitov
{
// map of ASCII values/table to 7-segment, offset to position 32.
const  PROGMEM uint8_t C_LED_7SegmentsText[] =
{
  0b10000110, // 0x86, // !
  0b00100010, // 0x22, // "
  0b01111110, // 0x7E, // #
  0b01101101, // 0x6D, // $
  0b11010010, // 0xD2, // %
  0b01000110, // 0x46, // &
  0b00100000, // 0x20, // '
  0b00101001, // 0x29, // (
  0b00001011, // 0x0B, // )
  0b00100001, // 0x21, // *
  0b01110000, // 0x70, // +
  0b00010000, // 0x10, // ,
  0b01000000, // 0x40, // -
  0b10000000, // 0x80, // .
  0b01010010, // 0x52, // /
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
  0b00001001, // 0x09, // :
  0b00001101, // 0x0D, // ;
  0b01100001, // 0x61, // <
  0b01001000, // 0x48, // =
  0b01000011, // 0x43, // >
  0b11010011, // 0xD3, // ?
  0b01011111, // 0x5F, // @
  0b01110111, // 0x77, // A
  0b01111100, // 0x7C, // B
  0b00111001, // 0x39, // C
  0b01011110, // 0x5E, // D
  0b01111001, // 0x79, // E
  0b01110001, // 0x71, // F
  0b00111101, // 0x3D, // G
  0b01110110, // 0x76, // H
//  0b1110110, // H
//  0b0110111, // H
  0b00110000, // 0x30, // I
  0b00011110, // 0x1E, // J
  0b01110101, // 0x75, // K
  0b00111000, // 0x38, // L
  0b00010101, // 0x15, // M
  0b00110111, // 0x37, // N
  0b00111111, // 0x3F, // O
  0b01110011, // 0x73, // P
  0b01101011, // 0x6B, // Q
  0b00110011, // 0x33, // R
  0b01101101, // 0x6D, // S
  0b01111000, // 0x78, // T
  0b00111110, // 0x3E, // U
  0b00111110, // 0x3E, // V
  0b00101010, // 0x2A, // W
  0b01110110, // 0x76, // X
  0b01101110, // 0x6E, // Y
  0b01011011, // 0x5B, // Z
  0b00111001, // 0x39, // [
  0b01100100, // 0x64, // \ //
  0b00001111, // 0x0F, // ]
  0b00100011, // 0x23, // ^
  0b00001000, // 0x08, // _
  0b00000010, // 0x02, // `
  0b01011111, // 0x5F, // a
  0b01111100, // 0x7C, // b
  0b01011000, // 0x58, // c
  0b01011110, // 0x5E, // d
  0b01111011, // 0x7B, // e
  0b01110001, // 0x71, // f
  0b01101111, // 0x6F, // g
  0b01110100, // 0x74, // h
  0b00010000, // 0x10, // i
  0b00001100, // 0x0C, // j
  0b01110101, // 0x75, // k
  0b00110000, // 0x30, // l
  0b00010100, // 0x14, // m
  0b01010100, // 0x54, // n
  0b01011100, // 0x5C, // o
  0b01110011, // 0x73, // p
  0b01100111, // 0x67, // q
  0b01010000, // 0x50, // r
  0b01101101, // 0x6D, // s
  0b01111000, // 0x78, // t
  0b00011100, // 0x1C, // u
  0b00011100, // 0x1C, // v
  0b00010100, // 0x14, // w
  0b01110110, // 0x76, // x
  0b01101110, // 0x6E, // y
  0b01011011, // 0x5B, // z
  0b01000110, // 0x46, // {
  0b00110000, // 0x30, // |
  0b01110000, // 0x70, // }
  0b00000001, // 0x01, // ~
  0b00000000  // 0x00 // (del)
};
//---------------------------------------------------------------------------
const uint8_t * const PROGMEM C_LED_7SegmentDigitSegments = C_LED_7SegmentsText + 15;
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue
	> class LED_TextDisplay7Segments :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

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
				int ARealDigit = C_COUNT_DIGITS - ADigit - 1;
                uint32_t ATextLength = InitialValue().length();
                const char *AText = InitialValue().c_str();
				if( ARealDigit >= ATextLength )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, 0 );
					continue;
                }

				char AValue = AText[ ARealDigit ];

				if( AValue <= ' ' )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, 0 );
					continue;
                }

				if( AValue > 126 )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, 0 );
					continue;
                }

	//			uint8_t AResult = C_LED_7SegmentsText[ AValue ];
				uint8_t AResult = pgm_read_byte( C_LED_7SegmentsText + AValue - ' ' - 1 );
				if( AResult & 0x80 )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, AResult );
					continue;
                }

				if( ARealDigit + 1 >= ATextLength )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, AResult );
					continue;
                }

				if( AText[ ARealDigit + 1 ] != '.' )
                {
               		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, AResult );
					continue;
                }

//				++ARealDigit;
				AResult |= 0x80;

           		C_OWNER.SetDigitSegments( ADigit + C_START_DIGIT, AResult );
			}

			C_OWNER.EndSendingDigits();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AValue = (char *)_Data;
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
}

