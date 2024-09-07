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
		uint16_t C_0_PIN_DOWN,
		uint16_t C_0_PIN_LEFT,
		uint16_t C_0_PIN_PRESS,
		uint16_t C_0_PIN_RIGHT,
		uint16_t C_0_PIN_UP,
		typename T_DirectionOutputPins_Up,
		typename T_DirectionOutputPins_Down,
		typename T_DirectionOutputPins_Left,
		typename T_DirectionOutputPins_Right,
		typename T_PressOutputPin
	> class TArduino_Navigation_Joystick_Module :
		public T_DirectionOutputPins_Up,
		public T_DirectionOutputPins_Down,
		public T_DirectionOutputPins_Left,
		public T_DirectionOutputPins_Right,
		public T_PressOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPins_Up )
		_V_PIN_( DirectionOutputPins_Down )
		_V_PIN_( DirectionOutputPins_Left )
		_V_PIN_( DirectionOutputPins_Right )
		_V_PIN_( PressOutputPin )


	protected:
		void SendData()
		{
			T_DirectionOutputPins_Up::SetPinValue( digitalRead( C_0_PIN_UP ) != HIGH );
			T_DirectionOutputPins_Down::SetPinValue( digitalRead( C_0_PIN_DOWN ) != HIGH );
			T_DirectionOutputPins_Left::SetPinValue( digitalRead( C_0_PIN_LEFT ) != HIGH );
			T_DirectionOutputPins_Right::SetPinValue( digitalRead( C_0_PIN_RIGHT ) != HIGH );
			T_PressOutputPin::SetPinValue( digitalRead( C_0_PIN_PRESS ) != HIGH );
		}

	public:
		inline void SystemStart()
		{
			pinMode( C_0_PIN_UP, INPUT_PULLUP );
			pinMode( C_0_PIN_LEFT, INPUT_PULLUP );
			pinMode( C_0_PIN_RIGHT, INPUT_PULLUP );
			pinMode( C_0_PIN_DOWN, INPUT_PULLUP );
			pinMode( C_0_PIN_PRESS, INPUT_PULLUP );

			SendData();
		}

		inline void SystemLoopBegin()
		{
			SendData();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif