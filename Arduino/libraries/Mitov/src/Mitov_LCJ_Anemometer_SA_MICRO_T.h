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
	namespace Mitov_SA_MICRO_T_Const
	{
		const uint8_t C_HEADER[] = { 0x00, 0x43, 0x56, 0x37 };
	}
//---------------------------------------------------------------------------
	template <
		typename T_MeasurementOutputPin,
		typename T_OverRangeOutputPin
	> class TArduinoLCJ_Anemometer_SA_MICRO_T_Alarms :
		public T_MeasurementOutputPin,
		public T_OverRangeOutputPin
	{
	public:
		_V_PIN_( MeasurementOutputPin )
		_V_PIN_( OverRangeOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_AngleOutputPin,
		typename T_SpeedOutputPin
	> class TArduinoLCJ_Anemometer_SA_MICRO_T_Wind :
		public T_AngleOutputPin,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( AngleOutputPin )
		_V_PIN_( SpeedOutputPin )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Angle,
		typename T_InFahrenheit,
		typename T_Speed
	> class TArduinoLCJ_Anemometer_SA_MICRO_T_Units :
		public T_Angle,
		public T_InFahrenheit,
		public T_Speed
	{
	public:
		_V_PROP_( Angle )
		_V_PROP_( InFahrenheit )
		_V_PROP_( Speed )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Alarms,
		typename T_Enabled,
		typename T_FIndex,
		typename T_TemperatureOutputPin,
		typename T_Units,
		typename T_VoltageOutputPin,
		typename T_Wind
	> class TArduinoLCJ_Anemometer_SA_MICRO_T :
		public T_Alarms,
		public T_Enabled,
		public T_FIndex,
		public T_TemperatureOutputPin,
		public T_Units,
		public T_VoltageOutputPin,
		public T_Wind
	{
	public:
		_V_PIN_( TemperatureOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Alarms )
		_V_PROP_( Enabled )
		_V_PROP_( Units )
		_V_PROP_( Wind )

	protected:
		_V_PROP_( FIndex )

	protected:
		uint8_t FData[ 6 ];

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			char AChar = *(char *)_Data;
			if( FIndex() < sizeof( Mitov_SA_MICRO_T_Const::C_HEADER ))
			{
				if( AChar == Mitov_SA_MICRO_T_Const::C_HEADER[ FIndex() ] )
					FIndex() = FIndex() + 1;

				else
					FIndex() = 0;

				return;
			}

			if( FIndex() < 10 )
			{
				FData[ FIndex() - sizeof( Mitov_SA_MICRO_T_Const::C_HEADER ) ] = AChar;
				FIndex() = FIndex() + 1;
				return;
			}

			FIndex() = 0;

			uint8_t AChecksum = 0;
			for( uint8_t i = 0; i < sizeof( FData ); ++ i )
				AChecksum ^= FData[ i ];

			if( AChar != AChecksum )
				return;

			float ASpeed = float( ( uint16_t( FData[ 1 ] & 0b00011111 ) << 5 ) | (( FData[ 2 ] >> 2 ) & 0b00011111 )) / 10.0;

			float AAngle = ( uint16_t( FData[ 0 ] & 0b00011111 ) << 2 ) | (( FData[ 1 ] >> 5 ) & 0b00000011 );

			ASpeed = Func::ConvertKnotsSpeedTo( Units().Speed().GetValue(), ASpeed );
			AAngle = Func::ConvertAngle( Units().Angle().GetValue(), AAngle );

			Wind().SpeedOutputPin().SetPinValue( ASpeed );
			Wind().AngleOutputPin().SetPinValue( AAngle );

			if( FData[ 2 ] & 0b00000001 )
				Alarms().OverRangeOutputPin().ClockPin();

			if( FData[ 2 ] & 0b00000010 )
                Alarms().MeasurementOutputPin().ClockPin();

			if( ( FData[ 4 ] & 0b10000000 ) == 0 )
			{
				float ATemperature = float( FData[ 3 ] & 0b01111111 ) - 40.0;
				if( Units().InFahrenheit() )
					ATemperature = Func::ConvertCtoF( ATemperature );

				TemperatureOutputPin().SetPinValue( ATemperature );

				float AVoltage = float( FData[ 4 ] & 0b00011111 ) * 0.125 + 1.5;
				VoltageOutputPin().SetPinValue( AVoltage );
			}
		}

	public:
		inline TArduinoLCJ_Anemometer_SA_MICRO_T()
		{
			FIndex() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
