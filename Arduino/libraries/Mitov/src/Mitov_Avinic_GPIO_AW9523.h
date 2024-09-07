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

// const uint8_t __TCP_HTTP_SERVER__DEBUG__


namespace Mitov
{
//---------------------------------------------------------------------------
	namespace AW9523Const
	{
		const uint8_t REG_P0_INPUT_STATE		= 0x00;
		const uint8_t REG_P1_INPUT_STATE		= 0x01;
		const uint8_t REG_P0_OUTPUT_STATE		= 0x02;
		const uint8_t REG_P1_OUTPUT_STATE		= 0x03;
		const uint8_t REG_P0_DIRECTION			= 0x04;
		const uint8_t REG_P1_DIRECTION			= 0x05;
		const uint8_t REG_P0_INTERRUPT_ENABLE	= 0x06;
		const uint8_t REG_P1_INTERRUPT_ENABLE	= 0x07;
		const uint8_t REG_ID					= 0x10;
		const uint8_t REG_GLOBAL_CONTROL		= 0x11;
		const uint8_t REG_P0_MODE_CONFIG		= 0x12;
		const uint8_t REG_P1_MODE_CONFIG		= 0x13;

		const uint8_t REG_P1_DIM_0				= 0x20;
		const uint8_t REG_P1_DIM_1				= 0x21;
		const uint8_t REG_P1_DIM_2				= 0x22;
		const uint8_t REG_P1_DIM_3				= 0x23;

		const uint8_t REG_P0_DIM_0				= 0x24;
		const uint8_t REG_P0_DIM_1				= 0x25;
		const uint8_t REG_P0_DIM_2				= 0x26;
		const uint8_t REG_P0_DIM_3				= 0x27;
		const uint8_t REG_P0_DIM_4				= 0x28;
		const uint8_t REG_P0_DIM_5				= 0x29;
		const uint8_t REG_P0_DIM_6				= 0x2A;
		const uint8_t REG_P0_DIM_7				= 0x2B;

		const uint8_t REG_P1_DIM_4				= 0x2C;
		const uint8_t REG_P1_DIM_5				= 0x2D;
		const uint8_t REG_P1_DIM_6				= 0x2E;
		const uint8_t REG_P1_DIM_7				= 0x2F;

		const uint8_t REG_RESET					= 0x7F;
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialAnalogValue,
		typename T_InitialValue,
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_OutputPin
	> class TArduinoAwinicGPIO_I2C_AW9523Channel :
		public T_InitialAnalogValue,
		public T_InitialValue,
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialAnalogValue )
		_V_PROP_( InitialValue )
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOutput )

	public:
        inline void UpdatePinDirections()
        {
			C_OWNER.UpdatePinDirections( C_INDEX );
        }

		inline void UpdateIsAnalog()
		{
			C_OWNER.UpdatePinMode( C_INDEX );
		}

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		inline void SetInitialMode( int AMode ) // Placeholder for direct pin control!
		{
//			SetMode( AMode );
		}

		bool DigitalRead()
		{
			return C_OWNER.GetBitValue( C_INDEX );
		}

		inline void DigitalWrite( bool AValue )
		{
			C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

		inline void AnalogWrite( float AValue )
		{
			C_OWNER.SetAnalogChannelValue( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

		inline void AnalogInputPin_o_Receive( void * _Data )
		{
			AnalogWrite( *(float *)_Data );
		}

	public:
		inline void GetDigitalValue( uint8_t &Avalue )
		{
			if( InitialValue() )
				Avalue |= 1 << C_INDEX;
			
		}

		inline void GetAnalogValue( uint8_t &Avalue )
		{
			Avalue = InitialAnalogValue() * 255 + 0.5;
		}

		inline void GetDirection( uint8_t &Avalue )
		{
			if( IsOutput() )
				Avalue |= 1 << C_INDEX;

		}

		inline void SetDigitalValue( uint8_t AValue )
		{
			T_OutputPin::SetPinValue( ( AValue & ( 1 << C_INDEX )) != 0 );
		}

		inline void GetMode( uint8_t &Avalue )
		{
			if( IsAnalog().GetValue() )
				Avalue &= ~( 1 << C_INDEX );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channels_GetAnalogValue,
		typename T_Channels_GetDigitalValue,
		typename T_Channels_GetDirection,
		typename T_Channels_GetMode,
		typename T_Channels_SetDigitalValue,
		typename T_IsOpenDrain
	> class TArduinoAwinicGPIO_I2C_AW9523_Port_0 :
		public T_IsOpenDrain
	{
	public:
		_V_PROP_( IsOpenDrain )

	public:
		inline void UpdatePinDirections( uint8_t AIndex )
		{
			C_OWNER.UpdatePinDirections_Port0( AIndex );
        }

		inline void UpdatePinMode( uint8_t AIndex )
		{
			C_OWNER.UpdatePinMode_Port0( AIndex );
        }

	public:
		inline void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			C_OWNER.SetChannelValue_Port0( AIndex, AValue );
		}

		inline void SetAnalogChannelValue( uint8_t AIndex, float AValue )
		{
			C_OWNER.SetAnalogChannelValue_Port0( AIndex, AValue );
		}

	public:
		inline void GetDirection( uint8_t &ADirection )
		{
			T_Channels_GetDirection::Call( ADirection );
		}

		inline void GetMode( uint8_t &AValue )
		{
			T_Channels_GetMode::Call( AValue );
		}

		inline void GetAnalogValue( uint8_t AIndex, uint8_t &AValue )
		{
			T_Channels_GetAnalogValue::Call( AIndex, AValue );
		}

		inline void GetDigitalValue( uint8_t &AValue )
		{
			T_Channels_GetDigitalValue::Call( AValue );
		}

		inline void SetDigitalValue( uint8_t AValue )
		{
			T_Channels_SetDigitalValue::Call( AValue );
		}

		inline void GetIsOpenDrain( bool &AValue )
		{
			AValue = IsOpenDrain();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_DimmingRange,
//		typename T_Port0,
		typename T_Port0_GetAnalogValue,
		typename T_Port0_GetDigitalValue,
		typename T_Port0_GetDirection,
		typename T_Port0_GetIsOpenDrain,
		typename T_Port0_GetMode,
		typename T_Port0_SetDigitalValue,
		typename T_Port1_GetAnalogValue,
		typename T_Port1_GetDigitalValue,
		typename T_Port1_GetDirection,
		typename T_Port1_GetMode,
		typename T_Port1_SetDigitalValue
	> class TArduinoAwinicGPIO_I2C_AW9523 :
		public T_Address,
		public T_DimmingRange
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( DimmingRange )
//		_V_PROP_( Port0 )

	public:
		inline void UpdatePinDirections_Port0( uint8_t AIndex )
		{
			Update_REG_P0_DIRECTION();
        }

		inline void UpdatePinMode_Port0( uint8_t AIndex )
		{
			Update_REG_P0_MODE_CONFIG();
        }

		inline void UpdatePinDirections( uint8_t AIndex )
		{
			Update_REG_P1_DIRECTION();
        }

		inline void UpdatePinMode( uint8_t AIndex )
		{
			Update_REG_P1_MODE_CONFIG();
        }

	public:
		inline void SetChannelValue_Port0( uint8_t AIndex, bool AValue )
		{
			uint8_t ARegValue = Read8( AW9523Const::REG_P0_OUTPUT_STATE );
			ARegValue &= ~( 1 << AIndex );
			if( AValue )
				ARegValue |= 1 << AIndex;

			Write8( AW9523Const::REG_P0_OUTPUT_STATE, AValue );
		}

		inline void SetChannelValue( uint8_t AIndex, bool AValue )
		{
			uint8_t ARegValue = Read8( AW9523Const::REG_P1_OUTPUT_STATE );
			ARegValue &= ~( 1 << AIndex );
			if( AValue )
				ARegValue |= 1 << AIndex;

			Write8( AW9523Const::REG_P1_OUTPUT_STATE, AValue );
		}

		inline void SetAnalogChannelValue_Port0( uint8_t AIndex, float AValue )
		{
			Write8( AW9523Const::REG_P0_DIM_0 + AIndex, AValue * 255 + 0.5 );
		}

		inline void SetAnalogChannelValue( uint8_t AIndex, float AValue )
		{
			if( AIndex >= 4 )
				Write8( AW9523Const::REG_P1_DIM_4 + AIndex - 4, AValue * 255 + 0.5 );

			else
				Write8( AW9523Const::REG_P1_DIM_0 + AIndex, AValue * 255 + 0.5 );
		}

	public:
		void Update_REG_P0_DIRECTION()
		{
			uint8_t ADirection = 0;
			T_Port0_GetDirection::Call( ADirection );
			Write8( AW9523Const::REG_P0_DIRECTION, ADirection );
		}

		void Update_REG_P1_DIRECTION()
		{
			uint8_t ADirection = 0;
			T_Port1_GetDirection::Call( ADirection );
			Write8( AW9523Const::REG_P1_DIRECTION, ADirection );
		}

		void Update_REG_P0_OUTPUT_STATE()
		{
			uint8_t AValue = 0;
			T_Port0_GetDigitalValue::Call( AValue );
			Write8( AW9523Const::REG_P0_OUTPUT_STATE, AValue );
		}

		void Update_REG_P1_OUTPUT_STATE()
		{
			uint8_t AValue = 0;
			T_Port1_GetDigitalValue::Call( AValue );
			Write8( AW9523Const::REG_P1_OUTPUT_STATE, AValue );
		}

		void Update_REG_GLOBAL_CONTROL()
		{
			bool AIsOpenDrain = true;
			T_Port0_GetIsOpenDrain::Call( AIsOpenDrain );
			uint8_t AValue = ( AIsOpenDrain ) ? 0 : 0b00010000;

			if( DimmingRange().GetValue() < 0.5 )
				AValue |= 0b11;

			else if( DimmingRange().GetValue() < 0.75 )
				AValue |= 0b10;

			else if( DimmingRange().GetValue() < 1.0 )
				AValue |= 0b01;

			Write8( AW9523Const::REG_GLOBAL_CONTROL, AValue );
		}

		void Update_REG_P0_MODE_CONFIG()
		{
			uint8_t AValue = 0xFF;
			T_Port0_GetMode::Call( AValue );
			Write8( AW9523Const::REG_P0_MODE_CONFIG, AValue );
		}

		void Update_REG_P1_MODE_CONFIG()
		{
			uint8_t AValue = 0xFF;
			T_Port1_GetMode::Call( AValue );
			Write8( AW9523Const::REG_P1_MODE_CONFIG, AValue );
		}

		void Update_REG_P0_DIM( uint8_t AIndex )
		{
			uint8_t AValue = 0;
			T_Port0_GetAnalogValue::Call( AIndex, AValue );
			Write8( AW9523Const::REG_P0_DIM_0 + AIndex, AValue );
		}

		void Update_REG_P1_DIM( uint8_t AIndex )
		{
			uint8_t AValue = 0;
			T_Port1_GetAnalogValue::Call( AIndex, AValue );

			if( AIndex >= 4 )
				Write8( AW9523Const::REG_P1_DIM_4 + AIndex - 4, AValue );

			else
				Write8( AW9523Const::REG_P1_DIM_0 + AIndex, AValue );

		}

		void SendOutputs()
		{
			T_Port0_SetDigitalValue::Call( Read8( AW9523Const::REG_P0_INPUT_STATE ));
			T_Port1_SetDigitalValue::Call( Read8( AW9523Const::REG_P1_INPUT_STATE ));
		}

	protected:
		void Write8( uint8_t ARegister, uint8_t AData ) 
		{
/*
			USBSerial.print( "W: " );
			USBSerial.print( ARegister, HEX );
			USBSerial.print( " - " );
			USBSerial.println( AData, BIN );
*/
			C_I2C.beginTransmission( uint8_t( Address() ));
			C_I2C.write( ARegister );
			C_I2C.write( AData );
			C_I2C.endTransmission();
		}

		uint8_t Read8( uint8_t ARegister )
		{
			C_I2C.beginTransmission( uint8_t( Address() ));
			C_I2C.write( ARegister );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( Address() ), _VISUINO_I2C_SIZE_( 1 ));
			return C_I2C.read();
		}

	public:
		inline void SystemInit()
		{
			Write8( AW9523Const::REG_RESET, 0 );
			Update_REG_GLOBAL_CONTROL();

			Update_REG_P0_OUTPUT_STATE();
			Update_REG_P1_OUTPUT_STATE();

			Update_REG_P0_DIRECTION();
			Update_REG_P1_DIRECTION();

			Update_REG_P0_MODE_CONFIG();
			Update_REG_P1_MODE_CONFIG();

			for( int i = 0; i < 8; ++ i )
			{
				Update_REG_P0_DIM( i );
				Update_REG_P1_DIM( i );
			}

		}

		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

