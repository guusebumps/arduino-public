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
#include <Mitov_Arduino_Basic_SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin,
		typename T_SPISpeed
	> class BoschBMM150_InterfaceSPISpeed :
		public InterfaceSPISpeed <
				T_SPI, C_SPI,
				T_ChipSelectOutputPin,
				T_SPISpeed
			>
	{
	public:
		inline void UpdateSampleRate( float AValue ) {}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


