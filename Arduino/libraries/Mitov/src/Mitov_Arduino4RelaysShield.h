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
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_InitialValue
	> class TArduino4RelaysShield_Channel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PIN_( Enabled )
		_V_PIN_( InitialValue )

	public:
		inline void UpdateOutput()
		{
			Digital.Write( C_PIN, InitialValue() );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( InitialValue() == *(bool *)_Data )
				return;

			InitialValue() = *(bool *)_Data;
			UpdateOutput();
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN, OUTPUT );
			UpdateOutput();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif