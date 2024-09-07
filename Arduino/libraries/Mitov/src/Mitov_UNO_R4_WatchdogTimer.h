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
#include <WDT.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period
	> class UNO_R4_WatchdogTimer :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
				WDT.begin( Period() );

			else
				WDT.begin( 0x7FFFFF );

		}

		inline void UpdatePeriod()
		{
			UpdateEnabled();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				WDT.refresh();
		}

		inline void SystemStart()
		{
			UpdateEnabled();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
