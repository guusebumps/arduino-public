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
//#include <Mitov_7SegmentDigits.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
namespace TM1637
{
//	const uint8_t ACTIVATE_TM	= 0x8F; // Start up
	const uint8_t BUTTONS_MODE	= 0x42; // Buttons mode
	const uint8_t DISPLAY_OFF	= 0x80;
	const uint8_t WRITE_INC		= 0x40; // Incremental write
	const uint8_t WRITE_LOC		= 0x44; // Write to a location
	const uint8_t BRIGHT_ADR	= 0x88; // Brightness address
	const uint8_t SEG_ADR		= 0xC0;  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
	const uint8_t LEDS_ADR		= 0xC1;  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
}
//---------------------------------------------------------------------------
	template <
		uint8_t C_NUM_DIGITS,
		typename T_Brightness,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin,
		typename T_Enabled,
		typename T_ReversedDigits
	> class TM1637_DigitsBasic :
		public T_Brightness,
		public T_Enabled,
		public T_ReversedDigits
	{
	public:
		_V_PROP_( Brightness )
		_V_PROP_( Enabled )
		_V_PROP_( ReversedDigits )

	public:
		TM1637_DigitsBasic &GetOwner() { return *this; }

	public:
		void UpdateBrightness()
		{
			if( Enabled() )
			{
				uint8_t AValue = TM1637::BRIGHT_ADR | ( uint8_t( Brightness().GetValue() * 7 + 0.5 ) & 0x07 );
				SendCommand( AValue );
			}

			else
				SendCommand( TM1637::DISPLAY_OFF );

		}

	public:
		inline static void SendCommand( uint8_t ACommand )
		{
			start();
			SendData( ACommand );
			stop();
//			shiftOut( C_DataOutputPin, C_ClockOutputPin, LSBFIRST, ACommand );
		}

	protected:
		static void start()
		{
			pinMode( C_DataOutputPin, OUTPUT );
			bitDelay();
		}

		static void stop()
		{
			pinMode( C_DataOutputPin, OUTPUT );
			bitDelay();
			pinMode( C_ClockOutputPin, INPUT );
			bitDelay();
			pinMode( C_DataOutputPin, INPUT );
			bitDelay();
		}

		static void SendData( uint8_t AValue )
		{
			// 8 Data Bits
			for(uint8_t i = 0; i < 8; ++ i ) 
			{
				// CLK low
				pinMode( C_ClockOutputPin, OUTPUT );
				delayMicroseconds( 2 );
//				bitDelay();
//				pinMode( C_DataOutputPin, OUTPUT );

				// Set data bit
				if( AValue & 0x01 )
					pinMode( C_DataOutputPin, INPUT );

				else
					pinMode( C_DataOutputPin, OUTPUT );
//				digitalWrite( C_DataOutputPin, ( AValue & 0x01 ) ? HIGH : LOW );

				delayMicroseconds( 2 );
//				bitDelay();

				// CLK high
				pinMode( C_ClockOutputPin, INPUT );
//				delayMicroseconds( 1 );
				bitDelay();
				AValue >>= 1;
			}

			// Wait for acknowledge
			// CLK to zero
			pinMode( C_ClockOutputPin, OUTPUT);
			pinMode( C_DataOutputPin, INPUT);
			bitDelay();

			// CLK to high
			pinMode( C_ClockOutputPin, INPUT );
#ifndef __BORLANDC__
			while( digitalRead( C_DataOutputPin ) )
				;
#endif // __BORLANDC__

//			bitDelay();
//			uint8_t ack = digitalRead(m_pinDIO);
//			if (ack == 0)
				pinMode( C_DataOutputPin, OUTPUT);

			bitDelay();

			pinMode( C_ClockOutputPin, OUTPUT );
			bitDelay();
		}

		static void bitDelay()
		{
			delayMicroseconds( 100 );
		}

	public:
		void ZeroDigits( uint8_t AStartIndex, uint8_t ACount )
		{
			if( ! ACount )
				return;

			SendCommand( TM1637::WRITE_INC );
			start();
			SendData( TM1637::SEG_ADR + AStartIndex );
			while( ACount -- )
				SendData( 0 );

			stop();
		}

		void StartSendingDigits( uint8_t AIndex )
		{
			if( ReversedDigits() )
				return;

			SendCommand( TM1637::WRITE_INC );
			start();
			SendData( TM1637::SEG_ADR + AIndex );
		}

		inline void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( ReversedDigits() )
			{
				if( AIndex > C_NUM_DIGITS - 1 )
					return;

				SendCommand( TM1637::WRITE_LOC );
				start();

				AIndex = C_NUM_DIGITS - 1 - AIndex;

				SendData( TM1637::SEG_ADR + AIndex );
				SendData( AValue );
				stop();
			}

			else
				SendData( AValue );
		}

		inline void EndSendingDigits()
		{
			if( ! ReversedDigits().GetValue() )
				stop();

		}

	public:
		inline static uint8_t GetClockOutputPin() { return C_ClockOutputPin; }
		inline static uint8_t GetDataOutputPin() { return C_DataOutputPin; }

	public:
		inline void SystemInit()
		{
			pinMode( C_ClockOutputPin, INPUT );
			pinMode( C_DataOutputPin, INPUT );

			digitalWrite( C_ClockOutputPin, LOW);

			SendCommand( TM1637::WRITE_INC );
			start();
			SendData( TM1637::SEG_ADR );
			for( uint8_t i = 0; i < C_NUM_DIGITS; ++ i )
				SendData( 0 );

			stop();
		}

		inline void SystemStart()
		{
			UpdateBrightness();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin,
		typename T_Enabled
	> class DIYMore_TM1637_6Digits :
	   public TM1637_DigitsBasic<
			6,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		>
	{
		typedef DIYMore_TM1637_6Digits T_SELF;

		typedef TM1637_DigitsBasic<
			6,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		> inherited;

	public:
		T_SELF &GetOwner() { return *this; }

		void ZeroDigits( uint8_t AStartIndex, uint8_t ACount )
		{
			if( ! ACount )
				return;

			for( int i = 0; i < ACount; ++ i )
				SetDigitSegments( i + AStartIndex, 0 );

		}

		inline void StartSendingDigits( uint8_t AIndex ) {}

		inline void EndSendingDigits() {}

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( AIndex > 5 )
				return;

			inherited::SendCommand( TM1637::WRITE_LOC );
			inherited::start();
			if( AIndex < 3 )
				AIndex += 3;

			else
				AIndex -= 3;

			inherited::SendData( TM1637::SEG_ADR + AIndex );
			inherited::SendData( AValue );
			inherited::stop();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin,
		typename T_Enabled
	> class DIYMore_TM1637_4Digits :
	   public TM1637_DigitsBasic<
			4,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		>
	{
		typedef DIYMore_TM1637_4Digits T_SELF;

		typedef TM1637_DigitsBasic<
			4,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		> inherited;

	public:
		T_SELF &GetOwner() { return *this; }

		void ZeroDigits( uint8_t AStartIndex, uint8_t ACount )
		{
			if( ! ACount )
				return;

			for( int i = 0; i < ACount; ++ i )
				SetDigitSegments( i + AStartIndex, 0 );

		}

		inline void StartSendingDigits( uint8_t AIndex ) {}

		inline void EndSendingDigits() {}

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( AIndex > 3 )
				return;

			inherited::SendCommand( TM1637::WRITE_LOC );
			inherited::start();

			AIndex = 3 - AIndex;

			inherited::SendData( TM1637::SEG_ADR + AIndex );
			inherited::SendData( AValue );
			inherited::stop();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		uint8_t C_ClockOutputPin,
		uint8_t C_DataOutputPin,
		typename T_Enabled,
		typename T_Points
	> class TM1637_4Digits_VerticalPoints :
	    public TM1637_DigitsBasic<
			4,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		>,
		public T_Points
	{
		typedef TM1637_4Digits_VerticalPoints T_SELF;

		typedef TM1637_DigitsBasic<
			4,
			T_Brightness,
			C_ClockOutputPin,
			C_DataOutputPin,
			T_Enabled,
			Mitov::ConstantProperty<88, bool, false >
		> inherited;

	public:
		_V_PROP_( Points )

	protected:
		uint8_t	FPointsByte = 0;

	public:
		inline void UpdatePoints()
		{
			inherited::SendCommand( TM1637::WRITE_LOC );
			inherited::start();
			inherited::SendData( TM1637::SEG_ADR + 1 );
			if( Points().GetValue() )
				inherited::SendData( FPointsByte | 0x80 );

			else
				inherited::SendData( FPointsByte & 0x7F );

			inherited::stop();
		}

	public:
		T_SELF &GetOwner() { return *this; }

		void SetDigitSegments( uint8_t AIndex, uint8_t AValue )
		{
			if( AIndex > 3 )
				return;

			if( AIndex == 2 )
				FPointsByte = AValue;

			inherited::SendCommand( TM1637::WRITE_LOC );
			inherited::start();
			inherited::SendData( TM1637::SEG_ADR + ( 3 - AIndex ) );
			if( Points().GetValue() && ( AIndex == 2 ) )
				inherited::SendData( AValue | 0x80 );

			else
				inherited::SendData( AValue & 0x7F );

			inherited::stop();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
