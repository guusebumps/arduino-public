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
#include <Wire.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#ifndef BUFFER_LENGTH
	#define BUFFER_LENGTH 256
#endif

#if defined( VISUINO_STMDUINO_MAPLE_OLD )
	typedef HardWire TwoWire;
#endif



namespace Mitov
{
	template <
		TwoWire &C_I2C,
		typename T_Address,
		typename T_INSTANCE_RequestData,
        typename T_INSTANCE_SendByte,
		typename T_IsSlave,
		typename T_OutputPin,
		typename T_RequestOutputPin
	> class ArduinoI2C :
		public ArduinoI2CMaster <
                C_I2C,
                T_INSTANCE_SendByte,
				T_OutputPin
			>,
		public T_Address,
		public T_IsSlave,
		public T_RequestOutputPin
	{
        typedef ArduinoI2CMaster <
                C_I2C,
                T_INSTANCE_SendByte,
				T_OutputPin
			> inherited;
        
	public:
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( IsSlave )

#if ! ( defined( VISUINO_ARDUINO_101 ) || defined( VISUINO_BBC_MICRO_BIT ) || defined( VISUINO_ARDUINO_RASPBERRY_PI ))
	protected:
		inline void SystemInitInternal()
		{
            inherited::SystemInitInternal();
			if( IsSlave() && T_RequestOutputPin::GetPinIsConnected() )
				C_I2C.onRequest( requestEvent );

		}

	protected:
		static void requestEvent()
		{
		    T_INSTANCE_RequestData::Call();
		}

#else // VISUINO_ARDUINO_101
	protected:
		inline void SystemInitInternal() 
        {
            inherited::SystemInitInternal();            
        }
        
#endif // VISUINO_ARDUINO_101
    public:
        inline void RequestData()
        {
            T_RequestOutputPin::ClockPin();
        }

	public:
		inline void SystemInit()
		{
			if( IsSlave() )
				C_I2C.begin( uint8_t( Address().GetValue() ));

			else
				C_I2C.begin();

			SystemInitInternal();
		}

	};
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
	> class ArduinoPinMapI2C :
		public ArduinoI2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin
			>,
		public T_SCL,
		public T_SDA
	{
		typedef ArduinoI2C <
				C_I2C,
				T_Address,
				T_INSTANCE_RequestData,
                T_INSTANCE_SendByte,
				T_IsSlave,
				T_OutputPin,
				T_RequestOutputPin
			> inherited;

	public:
        _V_PROP_( SCL )
        _V_PROP_( SDA )
        
	public:
		inline void SystemInit()
		{
			if( inherited::IsSlave() )
				C_I2C.begin( uint8_t( inherited::Address().GetValue() ), SDA(), SCL() );

			else
				C_I2C.begin( SDA(), SCL() );

			inherited::SystemInitInternal();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


