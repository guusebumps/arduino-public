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

#ifndef VISUINO_TEENSY
  #ifdef VISUINO_ARDUINO_PORTENTA
	#include <PluggableUSBHID.h>
	#include <USBMouse.h>
  #else // VISUINO_ARDUINO_PORTENTA
	  #ifdef VISUINO_ANY_ESP32
		#include <USBHIDMouse.h>
	  #else
		#include <Mouse.h>
	  #endif
  #endif // VISUINO_ARDUINO_PORTENTA
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
#ifdef VISUINO_ANY_ESP32
    USBHIDMouse Mouse;
#endif
#ifdef VISUINO_ARDUINO_PORTENTA
        USBMouse Mouse;
#endif
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FButton1,
		typename T_FButton2,
		typename T_FButton3
	> class MitovHIDMouse :
		public T_Enabled,
		public T_FButton1,
		public T_FButton2,
		public T_FButton3
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FButton1 )
		_V_PROP_( FButton2 )
		_V_PROP_( FButton3 )

	public:
#ifndef VISUINO_ARDUINO_PORTENTA
		void UpdateEnabled()
		{
			if( Enabled() )
				Mouse.begin();

			else
				Mouse.end();

		}
#endif
	public:
		void ButtonsInputPins_o_Receive( int AIndex, void *_Data )
		{
			if( ! Enabled() )
				return;

			bool AValue = *(bool *)_Data;
			switch( AIndex )
			{
				case 0 : DoButtonReceive1( AValue ); break;
				case 1 : DoButtonReceive2( AValue ); break;
				case 2 : DoButtonReceive3( AValue ); break;
			}
		}

	protected:
		inline void DoButtonReceive1( bool AValue )
		{
			if( FButton1().GetValue() == AValue )
				return;

			FButton1() = AValue;
			if( AValue )
				Mouse.press( MOUSE_LEFT );

			else
				Mouse.release( MOUSE_LEFT );
		}

		inline void DoButtonReceive2( bool AValue )
		{
			if( FButton2().GetValue() == AValue )
				return;

			FButton2() = AValue;
			if( AValue )
				Mouse.press( MOUSE_MIDDLE );

			else
				Mouse.release( MOUSE_MIDDLE );
		}

		inline void DoButtonReceive3( bool AValue )
		{
			if( FButton3().GetValue() == AValue )
				return;

			FButton3() = AValue;
			if( AValue )
				Mouse.press( MOUSE_RIGHT );

			else
				Mouse.release( MOUSE_RIGHT );
		}

	public:
		inline void SystemInit()
		{
#ifndef VISUINO_ARDUINO_PORTENTA
			if( Enabled() )
				Mouse.begin();
#endif
		}

	public:
		inline MitovHIDMouse()
		{
			FButton1() = false;
			FButton2() = false;
			FButton3() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FPressed,
		typename T_Wheel,
		typename T_X,
		typename T_Y
	> class MitovHIDMouseElementMove :
		public T_FPressed,
		public T_Wheel,
		public T_X,
		public T_Y
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Wheel )

	protected:
		_V_PROP_( FPressed )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.Enabled() )
#ifdef VISUINO_ARDUINO_PORTENTA
				Mouse.move( X(), Y() );
				Mouse.scroll( Wheel() );
#else
				Mouse.move( X(), Y(), Wheel() );
#endif
		}

	public:
		inline MitovHIDMouseElementMove()
		{
			FPressed() = false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class MitovHIDMousePosition
	{
	protected:
		int32_t	FCurrentX = 0;
		int32_t	FCurrentY = 0;

	public:
		inline void XInputPin_o_Receive( void *_Data )
		{
			int32_t	AValue = *(int32_t *)_Data;
			if( FCurrentX == AValue )
				return;

			if( C_OWNER.Enabled() )
			{
#ifdef VISUINO_ARDUINO_PORTENTA
				Mouse.move( AValue - FCurrentX, 0 );
#else
				Mouse.move( AValue - FCurrentX, 0, 0 );
#endif
				FCurrentX = AValue;
			}
		}

		inline void YInputPin_o_Receive( void *_Data )
		{
			int32_t	AValue = *(int32_t *)_Data;
			if( FCurrentY == AValue )
				return;

			if( C_OWNER.Enabled() )
			{
#ifdef VISUINO_ARDUINO_PORTENTA
				Mouse.move( 0, AValue - FCurrentY );
#else
				Mouse.move( 0, AValue - FCurrentY, 0 );
#endif
				FCurrentY = AValue;
			}
		}

		inline void ZeroInputPin_o_Receive( void *_Data )
		{
			FCurrentX = 0;
			FCurrentY = 0;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_OWNER, T_OWNER &C_OWNER> class MitovHIDMouseWheelPosition
	{
	protected:
		int32_t	FCurrent = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t	AValue = *(int32_t *)_Data;
			if( FCurrent == AValue )
				return;

			if( C_OWNER.Enabled() )
			{
#ifdef VISUINO_ARDUINO_PORTENTA
				Mouse.scroll( AValue - FCurrent );
#else
				Mouse.move( 0, 0, AValue - FCurrent );
#endif
				FCurrent = AValue;
			}
		}

		inline void ZeroInputPin_o_Receive( void *_Data )
		{
			FCurrent = 0;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif