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
		typename T_Enabled
	> class SamProcessorReset :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				rstc_start_software_reset( RSTC );

		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif