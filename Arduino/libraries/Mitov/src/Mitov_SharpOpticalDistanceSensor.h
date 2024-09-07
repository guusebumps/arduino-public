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
	namespace TArduinoSharpOpticalDistanceSensor_Type
	{
		enum TArduinoSharpOpticalDistanceSensor_Type 
		{
			GP2Y0A21YK0F, // 1080
			GP2Y0A02YK0F, // 20150
			GP2Y0A710K0F, // 100500
			GP2YA41SK0F,  // 430
			GP2Y0A51SK0F  // 215
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Type,
		typename T_Units,
		typename T_VoltageReference
	> class TArduinoSharpOpticalDistanceSensor :
		public T_Enabled,
		public T_OutputPin,
		public T_Type,
		public T_Units,
		public T_VoltageReference
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Type )
		_V_PROP_( Units )
		_V_PROP_( VoltageReference )

	protected:
		inline float GetVoltageReference()
		{
			if( VoltageReference().Enabled() )
				return VoltageReference().Value().GetValue();

			return __V_REFF_;
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			if( ! Enabled().GetValue() )
				return;

			float AValue = ( (*(float *)_Data) * ( 5.0 / GetVoltageReference() )) * 5.0;

			switch( Type() )
			{
				case TArduinoSharpOpticalDistanceSensor_Type::GP2Y0A21YK0F :	AValue = 29.988 * pow( AValue, -1.173 ); break;
				case TArduinoSharpOpticalDistanceSensor_Type::GP2Y0A02YK0F :	AValue = 60.374 * pow( AValue, -1.16 ); break;
				case TArduinoSharpOpticalDistanceSensor_Type::GP2YA41SK0F :		AValue = 12.08 * pow( AValue, -1.058 ); break;
				case TArduinoSharpOpticalDistanceSensor_Type::GP2Y0A51SK0F:		AValue = 5.2819 * pow( AValue, -1.161 ); break;
				case TArduinoSharpOpticalDistanceSensor_Type::GP2Y0A710K0F :	
				{
					if ( AValue < 1.4 || AValue > 3.3 ) 
						AValue = 0; // false data

					else
						AValue = 1.0 / (( AValue - 1.125 ) / 137.5 );

					break;
				}
			}

			if( Units() == rdInch )
                AValue /= 2.54;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
} // Mitov

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
