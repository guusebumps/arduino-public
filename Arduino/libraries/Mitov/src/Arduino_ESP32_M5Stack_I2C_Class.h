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
#include <Arduino_ESP32_I2C_Class.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoESP32_Generic_I2C_StickC_I2C_Pins
	{
		enum TArduinoESP32_Generic_I2C_StickC_I2C_Pins 
		{
			Auto,
			Hat,
			Grove,
			Custom
		};

	}
//---------------------------------------------------------------------------
	template <
		TwoWire &C_I2C,
		typename T_Address,
		typename T_INSTANCE_RequestData,
        typename T_INSTANCE_SendByte,
		typename T_IsSlave,
		typename T_OutputPin,
		typename T_Pins,
		typename T_RequestOutputPin,
		typename T_SCL,
		typename T_SDA,
		typename T_Speed
	> class ArduinoStickC_I2C :
		public Arduino_ESP32_I2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA,
				T_Speed
			>,
		public T_Pins
	{
		typedef Arduino_ESP32_I2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA,
				T_Speed
			> inherited;

	public:
		_V_PROP_( Pins )
        
	public:
		inline void SystemInit()
		{
			switch( Pins() )
			{
				case TArduinoESP32_Generic_I2C_StickC_I2C_Pins::Auto :
#ifdef _VISUINO_STICKC_SHIELD_I2C_
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 0, 26, inherited::Speed() );

					else
						C_I2C.begin( 0, 26, inherited::Speed() );

#else // _VISUINO_STICKC_SHIELD_I2C_

  #ifdef _VISUINO_STICKC_GROVE_I2C_
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 32, 33, inherited::Speed() );

					else
						C_I2C.begin( 32, 33, inherited::Speed() );

  #else _VISUINO_STICKC_GROVE_I2C_
			if( inherited::IsSlave() )
				C_I2C.begin( uint8_t( inherited::Address().GetValue() ), inherited::SDA(), inherited::SCL(), inherited::Speed() );

			else
				C_I2C.begin( inherited::SDA(), inherited::SCL(), inherited::Speed() );

  #endif _VISUINO_STICKC_GROVE_I2C_

#endif // _VISUINO_STICKC_SHIELD_I2C_
					break;

				case TArduinoESP32_Generic_I2C_StickC_I2C_Pins::Hat :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 0, 26, inherited::Speed() );

					else
						C_I2C.begin( 0, 26, inherited::Speed() );

					break;

				case TArduinoESP32_Generic_I2C_StickC_I2C_Pins::Grove :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 32, 33, inherited::Speed() );

					else
						C_I2C.begin( 32, 33, inherited::Speed() );

					break;

				case TArduinoESP32_Generic_I2C_StickC_I2C_Pins::Custom :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), inherited::SDA(), inherited::SCL(), inherited::Speed() );

					else
						C_I2C.begin( inherited::SDA(), inherited::SCL(), inherited::Speed() );

					break;

			}

			inherited::SystemInitInternal();
		}

	};
//---------------------------------------------------------------------------
	namespace TArduinoESP32_Generic_I2C_Atom_I2C_Pins
	{
		enum TArduinoESP32_Generic_I2C_Atom_I2C_Pins 
		{
			Auto,
			Base,
			Grove,
			Custom
		};

	}
//---------------------------------------------------------------------------
	template <
		TwoWire &C_I2C,
		typename T_Address,
		typename T_INSTANCE_RequestData,
        typename T_INSTANCE_SendByte,
		typename T_IsSlave,
		typename T_OutputPin,
		typename T_Pins,
		typename T_RequestOutputPin,
		typename T_SCL,
		typename T_SDA,
		typename T_Speed
	> class ArduinoAtom_I2C :
		public Arduino_ESP32_I2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA,
				T_Speed
			>,
		public T_Pins
	{
		typedef Arduino_ESP32_I2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin,
				T_SCL,
				T_SDA,
				T_Speed
			> inherited;

	public:
		_V_PROP_( Pins )
        
	public:
		inline void SystemInit()
		{
			switch( Pins() )
			{
				case TArduinoESP32_Generic_I2C_Atom_I2C_Pins::Auto :
#ifdef _VISUINO_ATOM_SHIELD_I2C_
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 25, 21, inherited::Speed() );

					else
						C_I2C.begin( 25, 21, inherited::Speed() );

#else // _VISUINO_ATOM_SHIELD_I2C_

  #ifdef _VISUINO_STICKC_GROVE_I2C_
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 26, 32, inherited::Speed() );

					else
						C_I2C.begin( 26, 32, inherited::Speed() );

  #else _VISUINO_STICKC_GROVE_I2C_
			if( inherited::IsSlave() )
				C_I2C.begin( uint8_t( inherited::Address().GetValue() ), inherited::SDA(), inherited::SCL(), inherited::Speed() );

			else
				C_I2C.begin( inherited::SDA(), inherited::SCL(), inherited::Speed() );

  #endif _VISUINO_STICKC_GROVE_I2C_

#endif // _VISUINO_ATOM_SHIELD_I2C_
					break;

				case TArduinoESP32_Generic_I2C_Atom_I2C_Pins::Base :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 25, 21, inherited::Speed() );

					else
						C_I2C.begin( 25, 21, inherited::Speed() );

					break;

				case TArduinoESP32_Generic_I2C_Atom_I2C_Pins::Grove :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), 26, 32, inherited::Speed() );

					else
						C_I2C.begin( 26, 32, inherited::Speed() );

					break;

				case TArduinoESP32_Generic_I2C_Atom_I2C_Pins::Custom :
					if( inherited::IsSlave() )
						C_I2C.begin( uint8_t( inherited::Address().GetValue() ), inherited::SDA(), inherited::SCL(), inherited::Speed() );

					else
						C_I2C.begin( inherited::SDA(), inherited::SCL(), inherited::Speed() );

					break;

			}

			inherited::SystemInitInternal();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


