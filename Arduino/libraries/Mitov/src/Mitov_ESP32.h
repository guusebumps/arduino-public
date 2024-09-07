////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VISUINO_ESP32

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_AutoConfig,
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin_Analog_,
		typename T_OutputPin_Digital_,
		int C_PIN_NUMBER
	> class Board_DAC_DigitalChannel :
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_OutputPin_Digital_
	{
	public:
		_V_PIN_( OutputPin_Digital_ )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsRawInput )

	public:
        void UpdatePinDirections()
        {
			if( IsRawInput() )
				return;

            if( IsOutput() )
			{
				if( IsAnalog() )
					pinMode( C_PIN_NUMBER, ANALOG );

				else
					pinMode( C_PIN_NUMBER, OUTPUT );
			}

            else
				pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : ( IsPullDown().GetValue() ? INPUT_PULLDOWN : INPUT ) );
        }

	public:
		inline void SystemInit()
		{
            UpdatePinDirections();
		}

		inline void SystemStart()
		{
			if( IsRawInput() )
				return;

			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_OutputPin_Digital_::SetPinValue( AValue, false );
		}

		inline void SystemLoopBegin()
        {
            if( ! IsOutput().GetValue() )
            {
    			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//				if( AValue == FLastOutput )
//					return;

//				FLastOutput = AValue;
//	Serial.println( AData.Value );
	    		T_OutputPin_Digital_::SetPinValue( AValue, true );
            }
        }

	public:
		void AnalogInputPin_o_Receive( void *_Data )
        {
            if( IsOutput().GetValue() && IsAnalog().GetValue() )
            {
                float AValue = *(float*)_Data;
                dacWrite( C_PIN_NUMBER, ( AValue * 255 ) + 0.5 );
            }
        }

		void DigitalInputPin_o_Receive( void *_Data )
		{
			if( IsRawInput() )
			{
				*((int*)_Data) = C_PIN_NUMBER;
				return;
			}

            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() ) && ( ! IsAnalog().GetValue() ))
            {
				bool AValue = *(bool *)_Data;

				if( AValue )
				  if( IsCombinedInOut() )
					  pinMode( C_PIN_NUMBER, OUTPUT );

                digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : ( IsPullDown().GetValue() ? INPUT_PULLDOWN : INPUT ) );

            }
		}

/*
	public:
        void SetIsOutput( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsOutput() = AValue;
            UpdatePinDirections();
        }

        void SetIsPullUp( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsPullUp() = AValue;
            UpdatePinDirections();
        }

        void SetIsAnalog( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsAnalog() = AValue;
            UpdatePinDirections();
        }
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // VISUINO_ESP32_S3