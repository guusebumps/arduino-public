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
#include <Mitov_Basic_I2C.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		TwoWire &C_I2C,
		typename T_Address,
		typename T_INSTANCE_RequestData,
        typename T_INSTANCE_SendByte,
		typename T_IsSlave,
		typename T_OutputPin,
		typename T_RequestOutputPin,
		typename T_SCL,
		typename T_SDA
	> class Arduino_STM32_I2C :
		public ArduinoPinMapI2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA
			>
	{
		typedef ArduinoPinMapI2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA
			> inherited;

	public:
		inline void SystemInit()
		{
			C_I2C.setSDA( inherited::SDA().GetValue() );
			C_I2C.setSCL( inherited::SCL().GetValue() );
			if( inherited::IsSlave() )
				C_I2C.begin( uint8_t( inherited::Address().GetValue() ) );

			else
				C_I2C.begin();

			inherited::SystemInitInternal();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


