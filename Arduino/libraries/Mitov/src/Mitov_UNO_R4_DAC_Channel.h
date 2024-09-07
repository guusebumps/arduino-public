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
		typename T_AnalogInputPin_o_IsConnected,
		typename T_AutoConfig,
		typename T_DigitalInputPin_o_IsConnected,
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin_Analog_,
		typename T_OutputPin_Digital_,
		int C_PIN_NUMBER,
		typename T_Resolution
	> class Arduino_UNO_R4_DAC_Channel :
		public T_AnalogInputPin_o_IsConnected,
		public T_AutoConfig,
		public T_DigitalInputPin_o_IsConnected,
		public T_InitialValue,
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOpenDrain,
		public T_IsOutput,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_OutputPin_Analog_,
		public T_OutputPin_Digital_,
		public T_Resolution
	{
	public:
		_V_PIN_( OutputPin_Analog_ )
		_V_PIN_( OutputPin_Digital_ )

	public:
		_V_PROP_( AutoConfig )
		_V_PROP_( InitialValue )
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsRawInput )
		_V_PROP_( Resolution )

	protected:
		_V_PROP_( AnalogInputPin_o_IsConnected )
		_V_PROP_( DigitalInputPin_o_IsConnected )

		inline void Update_Resolution()
		{
			analogWriteResolution( Resolution() );
		}

	protected:
        void UpdatePinDirections() // Used as Live Binding updater! Do not rename!
        {
			if( IsRawInput() )
				return;

			pinMode( C_PIN_NUMBER, 
		        ( IsCombinedInOut().GetValue() || IsOutput().GetValue() || ( AutoConfig() && DigitalInputPin_o_IsConnected() ) ) ?
					( IsOpenDrain() ? OUTPUT_OPENDRAIN : OUTPUT ) :
					( IsPullUp().GetValue() ? INPUT_PULLUP : INPUT ));

        }

	public:
		inline void SystemInit()
		{
			Update_Resolution();

            UpdatePinDirections();
		}

		inline void SystemStart()
		{
			if( IsRawInput() )
				return;

			if( T_OutputPin_Digital_::GetPinIsConnected() )
		    	T_OutputPin_Digital_::SetPinValue( digitalRead( C_PIN_NUMBER ), false );

			if( T_OutputPin_Analog_::GetPinIsConnected() )
		    	T_OutputPin_Digital_::SetPinValue( analogRead( C_PIN_NUMBER ), false );

		}

		inline void SystemLoopBegin()
        {
            if( IsCombinedInOut().GetValue() || ( ! ( IsOutput().GetValue() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() ) ) ))
            {
				if( T_OutputPin_Digital_::GetPinIsConnected() )
		    		T_OutputPin_Digital_::SetPinValue( digitalRead( C_PIN_NUMBER ), true );

				if( T_OutputPin_Analog_::GetPinIsConnected() )
		    		T_OutputPin_Digital_::SetPinValue( analogRead( C_PIN_NUMBER ), true );

			}
        }

	public:
		void AnalogInputPin_o_Receive( void *_Data )
        {
            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() ) ) && IsAnalog().GetValue() )
            {
                float AValue = *(float*)_Data;
				analogWrite( C_PIN_NUMBER, ( AValue * ( 1 << Resolution().GetValue() - 1 ) ) + 0.5 );
            }
        }

		void DigitalInputPin_o_Receive( void *_Data )
		{
			if( IsRawInput() )
			{
				*((int*)_Data) = C_PIN_NUMBER;
				return;
			}

            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() ) ) && ( ! IsAnalog().GetValue() ))
            {
				bool AValue = *(bool *)_Data;

				if( AValue )
				  if( IsCombinedInOut() )
					  pinMode( C_PIN_NUMBER, OUTPUT );

                digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : INPUT );

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