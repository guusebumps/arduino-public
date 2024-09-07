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
const uint8_t Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_FClocked,
		typename T_FReady,
		typename T_LEDOutputPin,
		typename T_OutputPin,
		int C_SensorInputPin,
		typename T_VoltageReference
	> class TArduinoSharpOpticalDustSensorGP2Y1010AU0F :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_FClocked,
		public T_FReady,
		public T_LEDOutputPin,
		public T_OutputPin,
		public T_VoltageReference
	{
	public:
		_V_PIN_( LEDOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( VoltageReference )

	protected:
		_V_PROP_( FClocked )
		_V_PROP_( FReady )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		inline float GetVoltageReference()
		{
			if( VoltageReference().Enabled() )
				return VoltageReference().Value().GetValue();

			return __V_REFF_;
		}

	protected:
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			T_LEDOutputPin::SetPinValue( false ); // power on the LED
			delayMicroseconds( 280 );

			uint16_t voMeasured = analogRead( C_SensorInputPin );

			delayMicroseconds( 40 );
			T_LEDOutputPin::SetPinValue( true ); // turn the LED off

			float calcVoltage = voMeasured * ( 5.0 / GetVoltageReference() ) * ( 5.0 / float( VISUINO_ANALOG_IN_RANGE ) );
			float dustDensity = 170 * calcVoltage - 0.1;

			T_OutputPin::SetPinValue( dustDensity );
		}

	public:
		inline void ClockInputPin_o_Receive( void * _Data )
		{
			if( FReady() )
			{
				ReadSensor();
				FReady() = false;
			}

			else
				FClocked() = true;

		}

	public:
		inline void PeriodicCall()
		{
			if( ClockInputPin_o_IsConnected() )
			{
				if( FClocked() )
				{
					FClocked() = false;
					ReadSensor();
				}

				else
					FReady() = true;

			}

			else
				ReadSensor();

		}

	public:
		inline void SystemInit()
		{
			T_LEDOutputPin::SetPinValue( true );
		}

		inline void SystemStart()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
} // Mitov

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
