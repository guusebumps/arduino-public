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
#include <Mitov_Avinic_GPIO_AW9523.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
#ifdef __M5STACK_DISPLAY__
	namespace CoreS3_Pin35Mode
	{
		bool	Output = false;
	}
#endif
//---------------------------------------------------------------------------
	namespace AXP2101Const
	{
//		const uint8_t CHIP_ID                          = 0x4A;

		const uint8_t REG_STATUS1                          = 0x00;
		const uint8_t REG_STATUS2                          = 0x01;
		const uint8_t REG_IC_TYPE                          = 0x03;


		const uint8_t REG_DATA_BUFFER1                     = 0x04;
		const uint8_t REG_DATA_BUFFER2                     = 0x05;
		const uint8_t REG_DATA_BUFFER3                     = 0x06;
		const uint8_t REG_DATA_BUFFER4                     = 0x07;

		const uint8_t REG_COMMON_CONFIG                    = 0x10;
		const uint8_t REG_BATFET_CTRL                      = 0x12;
		const uint8_t REG_DIE_TEMP_CTRL                    = 0x13;
		const uint8_t REG_MIN_SYS_VOL_CTRL                 = 0x14;
		const uint8_t REG_INPUT_VOL_LIMIT_CTRL             = 0x15;
		const uint8_t REG_INPUT_CUR_LIMIT_CTRL             = 0x16;
		const uint8_t REG_RESET_FUEL_GAUGE                 = 0x17;
		const uint8_t REG_CHARGE_GAUGE_WDT_CTRL            = 0x18;


		const uint8_t REG_WDT_CTRL                         = 0x19;
		const uint8_t REG_LOW_BAT_WARN_SET                 = 0x1A;


		const uint8_t REG_PWRON_STATUS                     = 0x20;
		const uint8_t REG_PWROFF_STATUS                    = 0x21;
		const uint8_t REG_PWROFF_EN                        = 0x22;
		const uint8_t REG_DC_OVP_UVP_CTRL                  = 0x23;
		const uint8_t REG_VOFF_SET                         = 0x24;
		const uint8_t REG_PWROK_SEQU_CTRL                  = 0x25;
		const uint8_t REG_SLEEP_WAKEUP_CTRL                = 0x26;
		const uint8_t REG_IRQ_OFF_ON_LEVEL_CTRL            = 0x27;

		const uint8_t REG_FAST_PWRON_SET0                  = 0x28;
		const uint8_t REG_FAST_PWRON_SET1                  = 0x29;
		const uint8_t REG_FAST_PWRON_SET2                  = 0x2A;
		const uint8_t REG_FAST_PWRON_CTRL                  = 0x2B;

		const uint8_t REG_ADC_CHANNEL_CTRL                 = 0x30;
		const uint8_t REG_ADC_DATA_RELUST0                 = 0x34;
		const uint8_t REG_ADC_DATA_RELUST1                 = 0x35;
		const uint8_t REG_ADC_DATA_RELUST2                 = 0x36;
		const uint8_t REG_ADC_DATA_RELUST3                 = 0x37;
		const uint8_t REG_ADC_DATA_RELUST4                 = 0x38;
		const uint8_t REG_ADC_DATA_RELUST5                 = 0x39;
		const uint8_t REG_ADC_DATA_RELUST6                 = 0x3A;
		const uint8_t REG_ADC_DATA_RELUST7                 = 0x3B;
		const uint8_t REG_ADC_DATA_RELUST8                 = 0x3C;
		const uint8_t REG_ADC_DATA_RELUST9                 = 0x3D;


		//XPOWERS INTERRUPT REGISTER
		const uint8_t REG_INTEN1                           = 0x40;
		const uint8_t REG_INTEN2                           = 0x41;
		const uint8_t REG_INTEN3                           = 0x42;


		//XPOWERS INTERRUPT STATUS REGISTER
		const uint8_t REG_INTSTS1                          = 0x48;
		const uint8_t REG_INTSTS2                          = 0x49;
		const uint8_t REG_INTSTS3                          = 0x4A;

		const uint8_t REG_TS_PIN_CTRL                      = 0x50;
		const uint8_t REG_TS_HYSL2H_SET                    = 0x52;
		const uint8_t REG_TS_LYSL2H_SET                    = 0x53;


		const uint8_t REG_VLTF_CHG_SET                     = 0x54;
		const uint8_t REG_VHLTF_CHG_SET                    = 0x55;
		const uint8_t REG_VLTF_WORK_SET                    = 0x56;
		const uint8_t REG_VHLTF_WORK_SET                   = 0x57;


		const uint8_t REG_JIETA_EN_CTRL                    = 0x58;
		const uint8_t REG_JIETA_SET0                       = 0x59;
		const uint8_t REG_JIETA_SET1                       = 0x5A;
		const uint8_t REG_JIETA_SET2                       = 0x5B;


		const uint8_t REG_IPRECHG_SET                      = 0x61;
		const uint8_t REG_ICC_CHG_SET                      = 0x62;
		const uint8_t REG_ITERM_CHG_SET_CTRL               = 0x63;

		const uint8_t REG_CV_CHG_VOL_SET                   = 0x64;

		const uint8_t REG_THE_REGU_THRES_SET               = 0x65;
		const uint8_t REG_CHG_TIMEOUT_SET_CTRL             = 0x67;

		const uint8_t REG_BAT_DET_CTRL                     = 0x68;
		const uint8_t REG_CHGLED_SET_CTRL                  = 0x69;

		const uint8_t REG_BTN_BAT_CHG_VOL_SET              = 0x6A;


		const uint8_t REG_DC_ONOFF_DVM_CTRL                = 0x80;
		const uint8_t REG_DC_FORCE_PWM_CTRL                = 0x81;
		const uint8_t REG_DC_VOL0_CTRL                     = 0x82;
		const uint8_t REG_DC_VOL1_CTRL                     = 0x83;
		const uint8_t REG_DC_VOL2_CTRL                     = 0x84;
		const uint8_t REG_DC_VOL3_CTRL                     = 0x85;
		const uint8_t REG_DC_VOL4_CTRL                     = 0x86;


		const uint8_t REG_LDO_ONOFF_CTRL0                  = 0x90;
		const uint8_t REG_LDO_ONOFF_CTRL1                  = 0x91;
		const uint8_t REG_LDO_VOL0_CTRL                    = 0x92;
		const uint8_t REG_LDO_VOL1_CTRL                    = 0x93;
		const uint8_t REG_LDO_VOL2_CTRL                    = 0x94;
		const uint8_t REG_LDO_VOL3_CTRL                    = 0x95;
		const uint8_t REG_LDO_VOL4_CTRL                    = 0x96;
		const uint8_t REG_LDO_VOL5_CTRL                    = 0x97;
		const uint8_t REG_LDO_VOL6_CTRL                    = 0x98;
		const uint8_t REG_LDO_VOL7_CTRL                    = 0x99;
		const uint8_t REG_LDO_VOL8_CTRL                    = 0x9A;


		const uint8_t REG_BAT_PARAME                       = 0xA1;
		const uint8_t REG_FUEL_GAUGE_CTRL                  = 0xA2;
		const uint8_t REG_BAT_PERCENT_DATA                 = 0xA4;
	}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InFahrenheit,
		typename T_OutputPin
	> class AXP2101_Thermometer :
		public T_Enabled,
		public T_InFahrenheit,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InFahrenheit )

	protected:
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! T_OutputPin::GetPinIsConnected() )
				return;

			float AValue = ( 22.0 + ( 7274 - C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST8 ) ) / 20.0 );
			if( InFahrenheit() )
				AValue = Func::ConvertCtoF( AValue );

			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_AvailableOutputPin,
		typename T_ChargingOutputPin,
		typename T_DischargingOutputPin,
		typename T_Enabled,
//		typename T_GoodSourceOutputPin,
		typename T_LevelOutputPin,
		typename T_VoltageOutputPin
	> class AXP2101_Battery :
		public T_AvailableOutputPin,
		public T_ChargingOutputPin,
		public T_DischargingOutputPin,
		public T_Enabled,
//		public T_GoodSourceOutputPin,
		public T_LevelOutputPin,
		public T_VoltageOutputPin
	{
	public:
		_V_PIN_( AvailableOutputPin )
		_V_PIN_( ChargingOutputPin )
		_V_PIN_( DischargingOutputPin )
//		_V_PIN_( GoodSourceOutputPin )
		_V_PIN_( LevelOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

//			USBSerial.println( "ReadSensor" );

/*
			if( T_GoodSourceOutputPin::GetPinIsConnected() )
			{
				uint8_t AOwerOnStatus = C_OWNER.AXP.Read8( AXP2101Const::REG_PWRON_STATUS );
				T_GoodSourceOutputPin::SetPinValue( ( AOwerOnStatus & 0b00010000 ) != 0 );
			}
*/
			if( T_VoltageOutputPin::GetPinIsConnected() || 
				T_LevelOutputPin::GetPinIsConnected() || 
				T_AvailableOutputPin::GetPinIsConnected() ||
				T_ChargingOutputPin::GetPinIsConnected() || 
				T_DischargingOutputPin::GetPinIsConnected() )
			{

				uint8_t AStatus = C_OWNER.AXP.Read8( AXP2101Const::REG_STATUS1 );
//				USBSerial.println( AStatus, BIN );
				int16_t ABatteryVoltage = int16_t( C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST0 ) & 0b0011111111111111 );
				if( ABatteryVoltage & 0b0010000000000000 )
					ABatteryVoltage = 0;

				bool AHasBattery = ( AStatus & 0b00001000 ) && ( ABatteryVoltage > 100 );
				T_AvailableOutputPin::SetPinValue( AHasBattery );
				T_ChargingOutputPin::SetPinValue( ( AStatus & 0b00100000 ) != 0 );
				T_DischargingOutputPin::SetPinValue( ( AStatus & 0b01000000 ) != 0 );
				if( T_VoltageOutputPin::GetPinIsConnected() )
				{
					if( ! AHasBattery )
						T_VoltageOutputPin::SetPinValue( 0 );

					else
					{
	//					USBSerial.println( C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST0 ));
//						float AValue = ( float( C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST0 ) & 0b0011111111111111 ) / float( 0b0011111111111111 ) ) * 8.192;
						float AValue = float( ABatteryVoltage ) / 1000.0;
						T_VoltageOutputPin::SetPinValue( AValue );
					}
				}

				if( T_LevelOutputPin::GetPinIsConnected() )
				{
					if( ! AHasBattery )
						T_LevelOutputPin::SetPinValue( 0 );

					else
					{
//						USBSerial.println( C_OWNER.AXP.Read8( AXP2101Const::REG_BAT_PERCENT_DATA ) );
						float AValue = float( C_OWNER.AXP.Read8( AXP2101Const::REG_BAT_PERCENT_DATA ) ) / 100.0;
						T_LevelOutputPin::SetPinValue( AValue );
//						C_OWNER.AXP.Write8( AXP2101Const::REG_RESET_FUEL_GAUGE, 0b0100 );
						C_OWNER.AXP.Write8( AXP2101Const::REG_RESET_FUEL_GAUGE, 0b1000 );
					}
				}
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_OTGPower,
		typename T_VoltageOutputPin
	> class AXP2101_USBPower :
		public T_ConnectedOutputPin,
		public T_Enabled,
		public T_OTGPower,
		public T_VoltageOutputPin
	{
	public:
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( VoltageOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( OTGPower )

	protected:
/*
		void ReadSensor()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_ConnectedOutputPin::GetPinIsConnected() )
			{
				uint8_t AOwerOnStatus = C_OWNER.AXP.Read8( AXP2101Const::REG_PWRON_STATUS );
				T_ConnectedOutputPin::SetPinValue( ( AOwerOnStatus & 0b00000100 ) != 0 );

				if( T_VoltageOutputPin::GetPinIsConnected() )
				{
//					float AValue = ( float( C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST4 ) & 0b0011111111111111 ) / float( 0b0011111111111111 ) ) * 8.192;
					float AValue = float( C_OWNER.AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST4 ) & 0b0011111111111111 ) / 1000.0;
					T_VoltageOutputPin::SetPinValue( AValue );
				}
			}

			else
				T_VoltageOutputPin::SetPinValue( 0 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensor();
		}

	public:
		inline void SystemStart()
		{
			ReadSensor();
		}

		inline void SystemLoopBegin()
		{
			ReadSensor();
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_OutputPin
	> class TArduinoESP32_M5Stack_CoreS3_PowerModule_Measurement :
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PIN_( OutputPin )

	};
//---------------------------------------------------------------------------
	template <
//		typename T_Boost,
//		typename T_Current,
		typename T_Enabled
//		typename T_Voltage
	> class AXP2101_PeripheralsPower :
//		public T_Boost,
//		public T_Current,
		public T_Enabled
//		public T_Voltage
	{
	public:
//		_V_PROP_( Boost )
//		_V_PROP_( Current )
		_V_PROP_( Enabled )
//		_V_PROP_( Voltage )

//	public:
//		bool IsEnabled() { return Enabled().GetValue(); }

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Peripherals,
		typename T_USBPower
	> class Arduino_CoreS3_PowerModule :
		public T_Peripherals,
		public T_USBPower
	{
	public:
		_V_PROP_( Peripherals )
		_V_PROP_( USBPower )

	public:
		class AXP2101
		{
		public:
			const uint8_t ADDR = 0x34;

		public:
			void Write8(uint8_t ARegister, uint8_t AData ) 
			{
				C_I2C.beginTransmission( ADDR );
				C_I2C.write( ARegister );
				C_I2C.write( AData );
				C_I2C.endTransmission();
			}

			uint8_t Read8( uint8_t ARegister )
			{
				C_I2C.beginTransmission( ADDR );
				C_I2C.write( ARegister );
				C_I2C.endTransmission();
				C_I2C.requestFrom( ADDR, _VISUINO_I2C_SIZE_( 1 ));
				return C_I2C.read();
			}

			uint16_t Read16( uint8_t ARegister )
			{
				C_I2C.beginTransmission( ADDR );
				C_I2C.write( ARegister );
				C_I2C.endTransmission();
				C_I2C.requestFrom( ADDR, _VISUINO_I2C_SIZE_( 2 ));
				uint16_t AResult = uint16_t( C_I2C.read() ) << 8;
				return AResult | C_I2C.read();
			}

			uint16_t Read12( uint8_t ARegister ) 
			{
				C_I2C.beginTransmission( ADDR );
				C_I2C.write( ARegister );
				C_I2C.endTransmission();
				C_I2C.requestFrom( ADDR, _VISUINO_I2C_SIZE_( 2 ));
				uint16_t AResult = uint16_t( C_I2C.read() ) << 4;
				return AResult | C_I2C.read();
			}

		};

		class AW9523B
		{
		public:
			static const uint8_t ADDR = 0x58;

		public:
			void Write8(uint8_t ARegister, uint8_t AData ) 
			{
				C_I2C.beginTransmission( ADDR );
				C_I2C.write( ARegister );
				C_I2C.write( AData );
				C_I2C.endTransmission();
			}

		};

		AXP2101	AXP;
		AW9523B	GPIO;

	protected:
		void ReadSensor()
		{
			uint8_t AOwerOnStatus = AXP.Read8( AXP2101Const::REG_PWRON_STATUS );
			if( USBPower().ConnectedOutputPin().GetPinIsConnected() )
				USBPower().ConnectedOutputPin().SetPinValue( ( AOwerOnStatus & 0b00000100 ) != 0 );

			if( USBPower().VoltageOutputPin().GetPinIsConnected() )
			{
//					float AValue = ( float( AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST4 ) & 0b0011111111111111 ) / float( 0b0011111111111111 ) ) * 8.192;
				float AValue = float( AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST4 ) & 0b0011111111111111 ) / 1000.0;
				USBPower().VoltageOutputPin().SetPinValue( AValue );
			}
		}

/*
		void ReadPeripheralsPowerVoltage()
		{
			uint16_t AValue = AXP.Read16( AXP2101Const::REG_JIETA_SET1 );
			Peripherals().Voltage().OutputPin().SetPinValue( AValue * 1.7f / 1000.0f );
		}
*/
	public:		
		void Update_P0_OUTPUT_STATE()
		{
			uint8_t AValue = 0b00000101;
			if( Peripherals().Enabled() )
				AValue |= 0b00000010;

			if( USBPower().OTGPower() )
				AValue |= 0b00100000;

			GPIO.Write8( AW9523Const::REG_P0_OUTPUT_STATE,	AValue );
		}

		void Update_P1_OUTPUT_STATE()
		{
			uint8_t AValue = 0b00000011;
			if( Peripherals().Enabled() )
//				if( GrovePower().Boost() )
					AValue |= 0b10000000;

			GPIO.Write8( AW9523Const::REG_P1_OUTPUT_STATE,	AValue );
		}

		void Update_GrovePower_Enabled()
		{
			Update_P0_OUTPUT_STATE();
			Update_P1_OUTPUT_STATE();
		}

	public:		
		inline void PowerOffInputPin_o_Receive( void *_Data )
		{
			AXP.Write8( AXP2101Const::REG_COMMON_CONFIG, AXP.Read8( AXP2101Const::REG_COMMON_CONFIG ) | 0b00000001 );
		}

	public:
		inline void SystemInit()
		{
//USBSerial.begin();
			AXP.Write8( AXP2101Const::REG_CHGLED_SET_CTRL, 0b00110101 );

			AXP.Write8( AXP2101Const::REG_LDO_ONOFF_CTRL0, 0xBF );  // AXP ALDO~4,BLDO0~2,DIDO1 Enable
			AXP.Write8( AXP2101Const::REG_LDO_VOL3_CTRL, 0b00011100 );  // AXP ALDO4 voltage / SD card / 3.3 V

			//Experimental >>
			AXP.Write8( AXP2101Const::REG_BAT_DET_CTRL, 1 );
			AXP.Write8( AXP2101Const::REG_ADC_CHANNEL_CTRL, 0b10101 );
			AXP.Write8( AXP2101Const::REG_CHARGE_GAUGE_WDT_CTRL, 0b1000 );
			//<< Experimental
			
			Update_P0_OUTPUT_STATE();
			Update_P1_OUTPUT_STATE();
			GPIO.Write8( AW9523Const::REG_P0_DIRECTION,		0b00011000 );
			GPIO.Write8( AW9523Const::REG_P1_DIRECTION,		0b00001100 );
			GPIO.Write8( AW9523Const::REG_GLOBAL_CONTROL,	0b00010000 );
			GPIO.Write8( AW9523Const::REG_P0_MODE_CONFIG,	0b11111111 );
			GPIO.Write8( AW9523Const::REG_P1_MODE_CONFIG,	0b11111111 );
		}

	public:
		inline void USBPower_ClockInputPin_o_Receive( void *_Data )
		{
			if( USBPower().Enabled() && USBPower().ConnectedOutputPin().GetPinIsConnected() && USBPower().ConnectedOutputPin().GetPinIsConnected() )
				ReadSensor();
		}

/*
		inline void PeripheralsPower_Current_ClockInputPin_o_Receive( void *_Data )
		{
			SystemStartPeripheralsPowerCurrent();
		}

		inline void PeripheralsPower_Voltage_ClockInputPin_o_Receive( void *_Data )
		{
			ReadPeripheralsPowerVoltage();
		}
*/
	public:
		inline void SystemStart()
		{
			if( USBPower().Enabled() && USBPower().ConnectedOutputPin().GetPinIsConnected() && USBPower().ConnectedOutputPin().GetPinIsConnected() )
				ReadSensor();
		}

		inline void SystemLoopBegin()
		{
//USBSerial.println( (AXP.Read8(0x00) & 0B00010000) );
//delay( 100 );
			if( USBPower().Enabled() && USBPower().ConnectedOutputPin().GetPinIsConnected() && USBPower().ConnectedOutputPin().GetPinIsConnected() )
				ReadSensor();

		}

/*
		inline void SystemStartPeripheralsPowerCurrent()
		{
		}

		inline void SystemStartPeripheralsPowerVoltage()
		{
			if( ! Peripherals().Voltage().ClockInputPin_o_IsConnected().GetValue() )
				ReadPeripheralsPowerVoltage();

		}
*/
/*
		inline void SystemLoopBegin()
		{
			
			USBSerial.println( (22.0 + ( 7274 - AXP.Read16( AXP2101Const::REG_ADC_DATA_RELUST8 ) ) / 20.0) );
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
