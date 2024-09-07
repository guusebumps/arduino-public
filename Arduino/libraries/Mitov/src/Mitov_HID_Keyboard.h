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

#if ! ( defined( VISUINO_TEENSY ) || defined( VISUINO_RPI_PICO ))
  #ifdef VISUINO_ARDUINO_PORTENTA
	#include <PluggableUSBHID.h>
	#include <USBKeyboard.h>
  #else // VISUINO_ARDUINO_PORTENTA
    #ifdef VISUINO_ANY_ESP32
      #include <USBHIDKeyboard.h>
    #else
	  #include <Keyboard.h>
    #endif
  #endif // VISUINO_ARDUINO_PORTENTA
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
#ifdef VISUINO_ANY_ESP32
    USBHIDKeyboard Keyboard;
#endif
//---------------------------------------------------------------------------
#ifdef VISUINO_ARDUINO_PORTENTA
        USBKeyboard Keyboard;
#endif
	template <
		typename T_Enabled
	> class MitovHIDKeyboard :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
#ifndef VISUINO_ARDUINO_PORTENTA
/*
			if( Enabled() )
				FKeyboard = new USBKeyboard;

			else
			{
				delete FKeyboard;
				FKeyboard = nullptr;
			}
*/
			if( Enabled() )
				Keyboard.begin();

			else
				Keyboard.end();

#endif
		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( Enabled() )
#ifdef VISUINO_ARDUINO_PORTENTA
			{
				String AStr( AValue );
				Keyboard.printf( AStr.c_str() );
				Keyboard.printf( "\n\r" );
			}
#else
				Keyboard.println( AValue );
#endif
		}

		inline void PrintChar( char AValue )
		{
			if( Enabled() )
#ifdef VISUINO_ARDUINO_PORTENTA
				Keyboard._putc( AValue );
#else
				Keyboard.write( AValue );
#endif
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
			if( ! Enabled().GetValue() )
                return;
                
			for( uint32_t i = 0; i < ASize; ++ i )
#ifdef VISUINO_ARDUINO_PORTENTA
				Keyboard._putc( AData[ i ] );
#else
				Keyboard.write( AData[ i ] );
#endif
		}

	public:
		inline void SystemInit()
		{
#ifdef VISUINO_ARDUINO_PORTENTA
/*
			if( Enabled() )
				FKeyboard = new USBKeyboard;
*/
#else
			if( Enabled() )
				Keyboard.begin();
#endif
		}

	};
//---------------------------------------------------------------------------
#ifndef VISUINO_ARDUINO_PORTENTA
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Character,
		typename T_FPressed
	> class MitovHIDKeyboardElementKey :
		public T_Character,
		public T_FPressed
	{
	public:
		_V_PROP_( Character )

	protected:
		_V_PROP_( FPressed )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.Enabled() )
			{
				bool AValue = *(bool *)_Data;
				if( AValue != FPressed().GetValue() )
				{
					FPressed() = AValue;
					if( AValue )
						Keyboard.press( Character() );

					else
						Keyboard.release( Character() );

				}
			}
		}
	};
#endif
//---------------------------------------------------------------------------
}

