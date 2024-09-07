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
		e_bsp_io_port_pin_t T_PIN,
		typename T_AutoConfig,
		typename T_DigitalInputPin_o_IsConnected,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullUp,
		typename T_IsRawInput,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class Arduino_UNO_R4_LoveChannel :
		public T_AutoConfig,
		public T_DigitalInputPin_o_IsConnected,
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsOpenDrain,
		public T_IsOutput,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( AutoConfig )
		_V_PROP_( InitialValue )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsOutput )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsRawInput )

	protected:
		_V_PROP_( DigitalInputPin_o_IsConnected )

	public:
        void UpdatePinDirections() // Used as Live Binding updater! Do not rename!
        {
//			Serial.begin( 9600 );
			R_IOPORT_PinCfg( nullptr, T_PIN, 
				( IsOutput() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() )) ?
					( IsOpenDrain() ? ( IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PMOS_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_OUTPUT ) :
					( IsPullUp() ? ( IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_INPUT ));


/*
			if( IsRawInput() )
				return;

            if( IsOutput() )
			{
					pinMode( C_PIN_NUMBER, OUTPUT );
			}

            else
				pinMode( C_PIN_NUMBER, IsPullUp().GetValue() ? INPUT_PULLUP : ( IsPullDown().GetValue() ? INPUT_PULLDOWN : INPUT ) );
*/
		}
        
	public:
		inline void SystemInit()
		{
            UpdatePinDirections();
            if( IsCombinedInOut().GetValue() || IsOutput().GetValue() || ( AutoConfig() && DigitalInputPin_o_IsConnected() ) )
            {
				if( InitialValue() )
					if( IsCombinedInOut() )
						R_IOPORT_PinCfg( nullptr, T_PIN, IsOpenDrain() ? ( IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PMOS_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_OUTPUT );

//				Serial.println( AValue );

				R_IOPORT_PinWrite( nullptr, T_PIN, ( InitialValue() ) ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW );

				if( ! InitialValue().GetValue() )
					if( IsCombinedInOut() )
						R_IOPORT_PinCfg( nullptr, T_PIN, IsPullUp() ? ( IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_INPUT );

            }
		}

		inline void SystemStart()
		{
            if( IsCombinedInOut().GetValue() || ( ! ( IsOutput().GetValue() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() ) ) ))
            {
                bsp_io_level_t ret;
                R_IOPORT_PinRead( nullptr, T_PIN, &ret);
                T_OutputPin::SetPinValue( ret != BSP_IO_LEVEL_LOW, true );
            }

//			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//	    	T_OutputPin::SetPinValue( AValue, false );
		}

		inline void SystemLoopBegin()
        {
            if( IsCombinedInOut().GetValue() || ( ! ( IsOutput().GetValue() || ( AutoConfig().GetValue() && DigitalInputPin_o_IsConnected().GetValue() ) ) ))
            {
                bsp_io_level_t ret;
                R_IOPORT_PinRead( nullptr, T_PIN, &ret);
                T_OutputPin::SetPinValue( ret != BSP_IO_LEVEL_LOW, true );
            }

/*
            if( ! IsOutput().GetValue() )
            {
    			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//				if( AValue == FLastOutput )
//					return;

//				FLastOutput = AValue;
//	Serial.println( AData.Value );
	    		T_OutputPin::SetPinValue( AValue, true );
            }
*/
		}

	public:
		void DigitalInputPin_o_Receive( void *_Data )
		{
//            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() ) && ( ! IsAnalog().GetValue() ))
            if( IsCombinedInOut().GetValue() || IsOutput().GetValue() || ( AutoConfig() && DigitalInputPin_o_IsConnected() ) )
            {
				bool AValue = *(bool *)_Data;

				if( AValue )
					if( IsCombinedInOut() )
						R_IOPORT_PinCfg( nullptr, T_PIN, IsOpenDrain() ? ( IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PMOS_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_OUTPUT );

//				Serial.println( AValue );

				R_IOPORT_PinWrite( nullptr, T_PIN, ( AValue ) ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						R_IOPORT_PinCfg( nullptr, T_PIN, IsPullUp() ? ( IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE ) : IOPORT_CFG_PORT_DIRECTION_INPUT );

            }
/*
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
*/
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

        void SetIsOpenDrain( bool AOldValue, bool AValue )
        {
            if( AOldValue == AValue )
                return;

            IsOpenDrain() = AValue;
            UpdatePinDirections();
        }
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif