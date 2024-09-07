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
#include <OPAMP.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_HighSpeed
	> class TArduino_UNO_R4_OperationalAmplifier :
		public T_Enabled,
		public T_HighSpeed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( HighSpeed )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				OPAMP.begin( ( HighSpeed().GetValue() ) ? OPAMP_SPEED_HIGHSPEED : OPAMP_SPEED_LOWSPEED );

			else
				OPAMP.end();

		}

		inline void UpdateHighSpeed()
		{
			if( Enabled() )
			{
				OPAMP.end();
				OPAMP.begin( ( HighSpeed().GetValue() ) ? OPAMP_SPEED_HIGHSPEED : OPAMP_SPEED_LOWSPEED );
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
