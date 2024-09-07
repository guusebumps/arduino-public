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
#define USBSerial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace LTR_553ALS
	{
		const uint8_t ALS_CONTR			= 0x80;
		const uint8_t PS_CONTR			= 0x81;
		const uint8_t PS_LED			= 0x82;
		const uint8_t PS_N_PULSES		= 0x83;
		const uint8_t PS_MEAS_RATE		= 0x84;
		const uint8_t ALS_MEAS_RATE		= 0x85;

		const uint8_t PART_ID			= 0x86;
		const uint8_t MANUFAC_ID		= 0x87;

		const uint8_t ALS_DATA_CH1_0	= 0x88;
		const uint8_t ALS_DATA_CH1_1	= 0x89;
		const uint8_t ALS_DATA_CH0_0	= 0x8A;
		const uint8_t ALS_DATA_CH0_1	= 0x8B;
		const uint8_t ALS_PS_STATUS		= 0x8C;
		const uint8_t PS_DATA_LOW  		= 0x8D;
		const uint8_t PS_DATA_HIGH	    = 0x8E;
		const uint8_t REG_INTERRUPT    		= 0x8F;
		const uint8_t PS_THRES_UP_0		= 0x90;
		const uint8_t PS_THRES_UP_1		= 0x91;
		const uint8_t PS_THRES_LOW_0	= 0x92;
		const uint8_t PS_THRES_LOW_1	= 0x93;
		const uint8_t PS_OFFSET_1	    = 0x94;
		const uint8_t PS_OFFSET_0	    = 0x95;
		const uint8_t ALS_THRES_UP_0	= 0x97;
		const uint8_t ALS_THRES_UP_1	= 0x98;
		const uint8_t ALS_THRES_LOW_0	= 0x99;
		const uint8_t ALS_THRES_LOW_1	= 0x9A;
		const uint8_t INTERRUPT_PERSIST = 0x9E;
	}
//---------------------------------------------------------------------------
	template <
		typename T_CountIgnored,
		typename T_Enabled,
		typename T_Max,
		typename T_Min
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_Interrupt_Item :
		public T_CountIgnored,
		public T_Enabled,
		public T_Max,
		public T_Min
	{
	public:
		_V_PROP_( CountIgnored )
		_V_PROP_( Enabled )
		_V_PROP_( Max )
		_V_PROP_( Min )

	};
//---------------------------------------------------------------------------
	template <
		typename T_ActiveLow,
		typename T_Light,
		typename T_Proximity
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_Interrupt :
		public T_ActiveLow,
		public T_Light,
		public T_Proximity
	{
	public:
		_V_PROP_( ActiveLow )
		_V_PROP_( Light )
		_V_PROP_( Proximity )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Current,
		typename T_DutyCycle,
		typename T_Enabled,
		typename T_Frequency,
		typename T_NumberPulses
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_LED :
		public T_Current,
		public T_DutyCycle,
		public T_Enabled,
		public T_Frequency,
		public T_NumberPulses
	{
	public:
		_V_PROP_( Current )
		_V_PROP_( DutyCycle )
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )
		_V_PROP_( NumberPulses )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_Gain,
		typename T_LED,
		typename T_Offset,
		typename T_OutputPin,
		typename T_RepeatPeriod,
		typename T_Saturation,
		typename T_Standby
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_Proximity :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_Gain,
		public T_LED,
		public T_Offset,
		public T_OutputPin,
		public T_RepeatPeriod,
		public T_Saturation,
		public T_Standby
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Gain )
		_V_PROP_( LED )
		_V_PROP_( Offset )
		_V_PROP_( RepeatPeriod )
		_V_PROP_( Saturation )
		_V_PROP_( Standby )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void Get_PS_CONTR( uint8_t &AValue )
		{
			if( Saturation().Enabled().GetValue() && Saturation().OutputPin().GetPinIsConnected() )
				AValue |= 0b00100000;

			if( ! Standby().GetValue() )
				AValue |= 0b00000011;

			if( Gain() < 32 )
				;

			else if( Gain() < 64 )
				AValue |= 0b00000010;

			else
				AValue |= 0b00000011;

		}

		inline void Get_PS_LED( uint8_t &AValue )
		{
			if( LED().Current() < 10 )
				;

			else if( LED().Current() < 20 )
				AValue |= 0b00000001;

			else if( LED().Current() < 50 )
				AValue |= 0b00000010;

			else if( LED().Current() < 100 )
				AValue |= 0b00000011;

			else
				AValue |= 0b00000111;


			if( LED().DutyCycle() < 50 )
				;

			else if( LED().DutyCycle() < 75 )
				AValue |= 0b00001000;

			else if( LED().DutyCycle() < 100 )
				AValue |= 0b00010000;

			else
				AValue |= 0b00011000;

			if( LED().Frequency() < 40 )
				;

			else if( LED().Frequency() < 50 )
				AValue |= 0b00100000;

			else if( LED().Frequency() < 60 )
				AValue |= 0b01000000;

			else if( LED().Frequency() < 70 )
				AValue |= 0b01100000;

			else if( LED().Frequency() < 80 )
				AValue |= 0b10000000;

			else if( LED().Frequency() < 90 )
				AValue |= 0b10100000;

			else if( LED().Frequency() < 100 )
				AValue |= 0b11000000;

			else
				AValue |= 0b11100000;

		}

		inline void Get_PS_N_Pulses( uint8_t &AValue )
		{
			AValue = LED().NumberPulses();
		}

		inline void Get_PS_MEAS_RATE( uint8_t &AValue )
		{
			if( RepeatPeriod() < 50 )
				AValue |= 0b00001111;

			else if( RepeatPeriod() < 70 )
				;

			else if( RepeatPeriod() < 100 )
				AValue |= 0b00000001;

			else if( RepeatPeriod() < 200 )
				AValue |= 0b00000010;

			else if( RepeatPeriod() < 500 )
				AValue |= 0b00000011;

			else if( RepeatPeriod() < 1000 )
				AValue |= 0b00000100;

			else if( RepeatPeriod() < 2000 )
				AValue |= 0b00000101;

			else
				AValue |= 0b00000111;

		}

		inline void Get_PS_OFFSET( uint16_t &AValue )
		{
			AValue = Offset() * 0b0000001111111111 + 0.5;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void Get_IsClocked( bool &AValue )
		{
			AValue = ClockInputPin_o_IsConnected();
		}

		inline void SendOutputs()
		{
			uint16_t AData = C_OWNER.Read16( LTR_553ALS::PS_DATA_LOW );
			if( Saturation().Enabled() )
				Saturation().OutputPin().SetPinValue( ( AData & 0x8000 ) != 0 );

//			float AFloatData = ( float( AData & 0x7FFF ) * Range() ) / 0x7FFF;
			float AFloatData = float( AData & 0x7FFF ); // / 0x7FFF;
			T_OutputPin::SetPinValue( AFloatData );

//			USBSerial.println( AData & 0x7FFF );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_Proximity_Saturation :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ChannelsOutputPins_0,
		typename T_ChannelsOutputPins_1,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_IntegrationTime,
		typename T_InvalidOutputPin,
		typename T_OutputPin,
		typename T_Range,
		typename T_RepeatPeriod,
		typename T_Standby
	> class TArduinoLightProximitySensor_LTR_553ALS_WA_Light :
		public T_ChannelsOutputPins_0,
		public T_ChannelsOutputPins_1,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_IntegrationTime,
		public T_InvalidOutputPin,
		public T_OutputPin,
		public T_Range,
		public T_RepeatPeriod,
		public T_Standby
	{
	public:
		_V_PIN_( ChannelsOutputPins_0 )
		_V_PIN_( ChannelsOutputPins_1 )
		_V_PIN_( InvalidOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( IntegrationTime )
		_V_PROP_( Range )
		_V_PROP_( RepeatPeriod )
		_V_PROP_( Standby )

	protected:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void Get_ALS_CONTR( uint8_t & AValue )
		{
			if( ! Standby().GetValue() )
				AValue |= 0b00000001;

			if( Range() < 1300 )
				AValue |= 0b00011100;

			else if( Range() < 8000 )
				AValue |= 0b00011000;

			else if( Range() < 16000 )
				AValue |= 0b00001100;

			else if( Range() < 32000 )
				AValue |= 0b00001000;

			else if( Range() < 64000 )
				AValue |= 0b00000100;

		}

		inline void Get_ALS_MEAS_RATE( uint8_t & AValue )
		{
			if( RepeatPeriod() < 100 )
				;

			else if( RepeatPeriod() < 200 )
				AValue |= 0b00000001;

			else if( RepeatPeriod() < 500 )
				AValue |= 0b00000010;

			else if( RepeatPeriod() < 1000 )
				AValue |= 0b00000011;

			else if( RepeatPeriod() < 2000 )
				AValue |= 0b00000100;

			else
				AValue |= 0b00000111;

			if( IntegrationTime() < 100 )
				AValue |= 0b00001000;

			else if( IntegrationTime() < 150 )
				;

			else if( IntegrationTime() < 200 )
				AValue |= 0b00100000;

			else if( IntegrationTime() < 250 )
				AValue |= 0b00010000;

			else if( IntegrationTime() < 300 )
				AValue |= 0b00101000;

			else if( IntegrationTime() < 350 )
				AValue |= 0b00110000;

			else if( IntegrationTime() < 400 )
				AValue |= 0b00111000;

			else
				AValue |= 0b00011000;

		}
/*
		inline void Set_InvalidOutputPin( bool AValue )
		{
			T_InvalidOutputPin::SetPinValue( AValue );
		}
*/
		inline void SendOutputs( uint8_t AStatus )
		{
//			USBSerial.println( AStatus, BIN );
			float AValue0;
			float AValue1;

			{
				uint8_t AData[ 4 ];
				C_OWNER.Read( LTR_553ALS::ALS_DATA_CH1_0, AData, 4 );
				if( Range().GetIsConstant() )
				{
					AValue0 = ( float( AData[ 2 ] | ( uint16_t( AData[ 3 ] ) << 8 )) * Range() ) / 0xFFFF;
					AValue1 = ( float( AData[ 0 ] | ( uint16_t( AData[ 1 ] ) << 8 )) * Range() ) / 0xFFFF;
				}

				else
				{
					uint16_t ARange; // = Range();
					switch( AStatus & 0b01110000 )
					{
						case 0b00000000: ARange = 64000; break;
						case 0b00010000: ARange = 32000; break;
						case 0b00100000: ARange = 16000; break;
						case 0b00110000: ARange = 8000; break;
						case 0b01100000: ARange = 1300; break;

						default:
						case 0b01110000: ARange = 600; break;
					}

					AValue0 = ( float( AData[ 2 ] | ( uint16_t( AData[ 3 ] ) << 8 )) * ARange ) / 0xFFFF;
					AValue1 = ( float( AData[ 0 ] | ( uint16_t( AData[ 1 ] ) << 8 )) * ARange ) / 0xFFFF;
				}

			} // Release the AData

			T_ChannelsOutputPins_0::SetPinValue( AValue0 );

			T_ChannelsOutputPins_1::SetPinValue( AValue1 );

//return;
			T_OutputPin::SetPinValue( ( AValue0 + AValue1 ) / 2 );

//			USBSerial.println( ( AValue * Range() ) / 0xFFFF );
			T_InvalidOutputPin::SetPinValue( ( AStatus & 0b10000000 ) != 0 );
		}

		inline void Get_IsClocked( bool &AValue )
		{
			AValue = ClockInputPin_o_IsConnected();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs( C_OWNER.Read( LTR_553ALS::ALS_PS_STATUS ) );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_Interrupt,
//		typename T_Interrupt_Get_ALS_THRES_Max,
//		typename T_Interrupt_Get_ALS_THRES_Min,
//		typename T_Interrupt_Get_INTERRUPT,
//		typename T_Interrupt_Get_INTERRUPT_PERSIST,
//		typename T_Interrupt_Get_PS_THRES_Max,
//		typename T_Interrupt_Get_PS_THRES_Min,
		typename T_Light_Get_ALS_CONTR,
		typename T_Light_Get_ALS_MEAS_RATE,
		typename T_Light_Get_IsClocked,
		typename T_Light_SendOutputs,
//		typename T_Light_Set_InvalidOutputPin,
		typename T_Proximity_Get_IsClocked,
		typename T_Proximity_Get_PS_CONTR,
		typename T_Proximity_Get_PS_LED,
		typename T_Proximity_Get_PS_MEAS_RATE,
		typename T_Proximity_Get_PS_N_Pulses,
		typename T_Proximity_Get_PS_OFFSET,
		typename T_Proximity_SendOutputs
 	> class TArduinoLightProximitySensor_LTR_553ALS_WA :
		public T_Address,
		public T_Interrupt
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Interrupt )

	public:
/*
		void Update_ALS_CONTR_Reset( bool AReset )
		{
			uint8_t AValue = AReset ? 0b00000010 : 0;

			T_Light_Get_ALS_CONTR::Call( AValue );

			Write( LTR_553ALS::ALS_CONTR, AValue ); 
		}
*/
		inline void Update_ALS_CONTR()
		{
//			Update_ALS_CONTR_Reset( false );
			uint8_t AValue = 0;

			T_Light_Get_ALS_CONTR::Call( AValue );

			Write( LTR_553ALS::ALS_CONTR, AValue ); 
		}

		inline void Update_ALS_MEAS_RATE()
		{
			uint8_t AValue = 0;
			T_Light_Get_ALS_MEAS_RATE::Call( AValue );
			Write( LTR_553ALS::ALS_MEAS_RATE, AValue ); 
		}

		void Update_PS_CONTR()
		{
			uint8_t AValue = 0;
			T_Proximity_Get_PS_CONTR::Call( AValue );
			Write( LTR_553ALS::PS_CONTR, AValue ); 
		}

		void Update_PS_MEAS_RATE()
		{
			uint8_t AValue = 0;
			T_Proximity_Get_PS_MEAS_RATE::Call( AValue );
			Write( LTR_553ALS::PS_MEAS_RATE, AValue ); 
		}

		void Update_PS_LED()
		{
			uint8_t AValue = 0;
			T_Proximity_Get_PS_LED::Call( AValue );
			Write( LTR_553ALS::PS_LED, AValue ); 
		}

		void Update_INTERRUPT()
		{
			uint8_t AValue = ( Interrupt().ActiveLow() ) ? 0 : 0b00000100;

			if( Interrupt().Light().Enabled() )
				AValue |= 0b00000010;

			if( Interrupt().Proximity().Enabled() )
				AValue |= 0b00000001;

//			T_Interrupt_Get_INTERRUPT::Call( AValue );

			Write( LTR_553ALS::REG_INTERRUPT, AValue ); 
		}		

		void Update_INTERRUPT_PERSIST()
		{
			uint8_t AValue = Interrupt().Light().CountIgnored() | ( Interrupt().Proximity().CountIgnored() << 4 ); 

//			T_Interrupt_Get_INTERRUPT_PERSIST::Call( AValue );
			Write( LTR_553ALS::INTERRUPT_PERSIST, AValue ); 
		}

		void Update_PS_OFFSET()
		{
			uint16_t AValue = 0;
			T_Proximity_Get_PS_OFFSET::Call( AValue );
			Write( LTR_553ALS::PS_OFFSET_0, AValue & 0xFF ); 
			Write( LTR_553ALS::PS_OFFSET_1, AValue >> 8 ); 
		}

		void Update_PS_N_Pulses()
		{
			uint8_t AValue = 0;
			T_Proximity_Get_PS_N_Pulses::Call( AValue );
			Write( LTR_553ALS::PS_N_PULSES, AValue ); 
		}

		void Update_ALS_THRES_Min()
		{
			uint16_t AValue = Interrupt().Light().Min() * 0xFFFF + 0.5;
		
//			T_Interrupt_Get_ALS_THRES_Min::Call( AValue );
			Write16( LTR_553ALS::ALS_THRES_LOW_0, AValue ); 
		}

		void Update_ALS_THRES_Max()
		{
			uint16_t AValue = Interrupt().Light().Max() * 0xFFFF + 0.5;
//			T_Interrupt_Get_ALS_THRES_Max::Call( AValue );
			Write16( LTR_553ALS::ALS_THRES_UP_0, AValue ); 
		}

		void Update_PS_THRES_Min()
		{
			uint16_t AValue = Interrupt().Proximity().Min() * 0xFFFF + 0.5;
//			T_Interrupt_Get_PS_THRES_Min::Call( AValue );
			Write16( LTR_553ALS::PS_THRES_LOW_0, AValue ); 
		}

		void Update_PS_THRES_Max()
		{
			uint16_t AValue = Interrupt().Proximity().Max() * 0xFFFF + 0.5;
//			T_Interrupt_Get_PS_THRES_Max::Call( AValue );
			Write16( LTR_553ALS::PS_THRES_UP_0, AValue ); 
		}

	public:
		void Read( uint8_t start_reg_addr, uint8_t *pdata, uint8_t size )
		{
			C_I2C.beginTransmission( Address() );
			C_I2C.write( start_reg_addr );
			C_I2C.endTransmission();

			C_I2C.requestFrom( Address(), _VISUINO_I2C_SIZE_( size ));
			for( uint8_t i = 0; i < size; ++ i )
				*pdata ++ = C_I2C.read();

		}

		uint8_t Read( uint8_t start_reg_addr )
		{
			uint8_t AResult;
			Read( start_reg_addr, &AResult, 1 );
			return AResult;
		}

		uint16_t Read16( uint8_t start_reg_addr )
		{
			C_I2C.beginTransmission( Address() );
			C_I2C.write( start_reg_addr );
			C_I2C.endTransmission();

			C_I2C.requestFrom( Address(), _VISUINO_I2C_SIZE_( 2 ));
			uint16_t AResult = C_I2C.read();
			return AResult | ( uint16_t( C_I2C.read() ) << 8 );
		}

		void Write( uint8_t start_reg_addr, uint8_t *pdata, uint8_t size )
		{
			C_I2C.beginTransmission( Address() );
			C_I2C.write( start_reg_addr );
			for( uint8_t i = 0; i < size; ++ i )
				C_I2C.write( *pdata ++ );

			C_I2C.endTransmission();
		}

		inline void Write( uint8_t start_reg_addr, uint8_t AData )
		{
/*
			USBSerial.print( "A: " );
			USBSerial.print( start_reg_addr, HEX );
			USBSerial.print( " D: " );
			USBSerial.println( AData, BIN );
*/
			Write( start_reg_addr, &AData, 1 );
		}

		inline void Write16( uint8_t start_reg_addr, uint16_t AData )
		{
			C_I2C.beginTransmission( Address() );
			C_I2C.write( start_reg_addr );

			C_I2C.write( AData & 0xFF );
			C_I2C.write( ( AData >> 8 ) & 0xFF );

//			for( uint8_t i = 0; i < size; ++ i )
//				C_I2C.write( *pdata ++ );

			C_I2C.endTransmission();
		}

	public:
		inline void ResetInputPin_o_Receive( void *_Data )
		{
			SystemInit();
		}

	public:
		inline void SystemInit()
		{
			Write( LTR_553ALS::ALS_CONTR, 0b00000010 ); // Reset
//			Update_ALS_CONTR_Reset( true );
//			delay( 1000 );
//			Update_ALS_CONTR_Reset( false );
			Update_ALS_CONTR();
			Update_PS_CONTR();
			Update_ALS_MEAS_RATE();
			Update_PS_MEAS_RATE();
			Update_PS_LED();
			Update_PS_OFFSET();
			Update_PS_N_Pulses();
			Update_INTERRUPT();
			Update_INTERRUPT_PERSIST();
			Update_ALS_THRES_Min();
			Update_ALS_THRES_Max();
			Update_PS_THRES_Min();
			Update_PS_THRES_Max();
		}

		inline void SystemLoopBegin()
		{
			bool A_Light_IsClocked = false;
			T_Light_Get_IsClocked::Call( A_Light_IsClocked );

			bool A_Proximity_IsClocked = false;
			T_Proximity_Get_IsClocked::Call( A_Proximity_IsClocked );

//			USBSerial.println( A_Light_IsClocked );
//			delay( 100 );

			if( A_Light_IsClocked && A_Proximity_IsClocked )
				return;

			uint8_t AStatus = Read( LTR_553ALS::ALS_PS_STATUS );
//			USBSerial.println( AStatus, BIN );
//			delay( 100 );
			if( ! A_Light_IsClocked )
				if( AStatus & 0b00000100 )
					T_Light_SendOutputs::Call( AStatus );

			if( ! A_Proximity_IsClocked )
				if( AStatus & 0b00000001 )
					T_Proximity_SendOutputs::Call();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef USBSerial
#undef Serial
#endif
