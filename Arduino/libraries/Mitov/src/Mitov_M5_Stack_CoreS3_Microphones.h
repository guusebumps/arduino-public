////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_ESP32_I2S.h>
#include <Mitov_EverestSemiconductor_I2S_Microphone_ES7210.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Left,
		typename T_Right
	> class TArduinoM5StackCoreS3MicrophonesModule_Channels :
		public T_Left,
		public T_Right
	{
	public:
		_V_PROP_( Left )
		_V_PROP_( Right )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Volume
	> class TArduinoM5StackCoreS3MicrophonesModule_Channel :
		public T_Volume
	{
	public:
		_V_PROP_( Volume )

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_I2C, T_I2C &C_I2C,
		typename T_Channels,
		typename T_Enabled,
		typename T_OutputPin
	> class M5Stack_CoreS3_Microphones :
/*
		public TArduinoI2SAmplifierNS4168 <	
				44100,
				Mitov::ConstantProperty<5, bool, false > // RightChannel
			>,
*/
		public T_Channels,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Channels )
		_V_PROP_( Enabled )

	private:
		static const uint8_t C_Address = 0x40;


	protected:
		static void OutputCallback( void *AInstance, void *_Data )
		{
			if( ((M5Stack_CoreS3_Microphones *)AInstance )->Enabled() )
				((M5Stack_CoreS3_Microphones *)AInstance )->OutputPin().PinNotify( _Data );

		}

		static void ControlCallback( void *AInstance, void *_Data )
		{
//            Serial.println( "TEST1_1" );
//			delay( 1000 );
//			((TArduinoI2SAmplifierNS4168 *)AInstance )->ControlInputPin_o_Receive( _Data );

		    TI2SInfo &AInfo = *(TI2SInfo *)_Data;

		    AInfo.Input.Used = true;

//            Serial.println( "TEST1" );
//            delay( 10000 );

		    for( int i = 0; i < AInfo.Input.Modes.size(); ++ i )
		    {
                if( ! AInfo.Input.Modes[ i ].Enabled )
                    continue;

                if( AInfo.Input.Modes[ i ].Mode == TI2SMode::Standard )
                {
                    AInfo.Input.Modes[ i ].SetBitsPerSample( 16, 16 );
                    AInfo.Input.Modes[ i ].SetSampleRate( 24000, 24000 );

                    AInfo.Input.Modes[ i ].ChannelsMap |= 0b11;
                }

				else
					AInfo.Input.Modes[ i ].Enabled = false;

		    }
		}

	protected:
		void Write(uint8_t ARegister, uint8_t AValue )
		{
			C_I2C.beginTransmission( C_Address );
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( AValue ));
			C_I2C.endTransmission();
		}

	public:
		void Update_MIC1_GAIN_REG43()
		{
			uint8_t AValue = 0b10000 | uint8_t( ( 1.0f - Channels().Left().Volume().GetValue() ) * 14 + 0.5f );
			Write( ES7210_Const::MIC1_GAIN_REG43, AValue ); // MIC1_GAIN
//			Write( ES7210_Const::MIC1_GAIN_REG43, 0x1B ); // MIC1_GAIN
		}

		void Update_MIC2_GAIN_REG44()
		{
			uint8_t AValue = 0b10000 | uint8_t( ( 1.0f - Channels().Right().Volume().GetValue() ) * 14 + 0.5f );
			Write( ES7210_Const::MIC2_GAIN_REG44, AValue ); // MIC2_GAIN
//			Write( ES7210_Const::MIC2_GAIN_REG44, 0x1B ); // MIC2_GAIN
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
			{
				Write( ES7210_Const::RESET_REG00, 0xff ); // reset

				Write( ES7210_Const::RESET_REG00, 0x41 ); // RESET_CTL
				Write( ES7210_Const::CLOCK_OFF_REG01, 0x1f ); // CLK_ON_OFF
				Write( ES7210_Const::POWER_DOWN_REG06, 0x00 ); // DIGITAL_PDN
				Write( ES7210_Const::OSR_REG07, 0x20 ); // ADC_OSR
				Write( ES7210_Const::MODE_CONFIG_REG08, 0x10 ); // MODE_CFG
				Write( ES7210_Const::TIME_CONTROL0_REG09, 0x30 ); // TCT0_CHPINI
				Write( ES7210_Const::TIME_CONTROL1_REG0A, 0x30 ); // TCT1_CHPINI
				Write( ES7210_Const::ADC34_HPF2_REG20, 0x0a ); // ADC34_HPF2
				Write( ES7210_Const::ADC34_HPF1_REG21, 0x2a ); // ADC34_HPF1
				Write( ES7210_Const::ADC12_HPF1_REG22, 0x0a ); // ADC12_HPF2
				Write( ES7210_Const::ADC12_HPF2_REG23, 0x2a ); // ADC12_HPF1
				Write( ES7210_Const::MAINCLK_REG02, 0xC1 );
				Write( ES7210_Const::LRCK_DIVH_REG04, 0x01 );
				Write( ES7210_Const::LRCK_DIVL_REG05, 0x00 );
				Write( ES7210_Const::SDP_INTERFACE1_REG11, 0x60 );
				Write( ES7210_Const::ANALOG_REG40, 0x42 ); // ANALOG_SYS
				Write( ES7210_Const::MIC12_BIAS_REG41, 0x70 ); // MICBIAS12
				Write( ES7210_Const::MIC34_BIAS_REG42, 0x70 ); // MICBIAS34

				Update_MIC1_GAIN_REG43();
				Update_MIC2_GAIN_REG44();

				Write( ES7210_Const::MIC3_GAIN_REG45, 0x00 ); // MIC3_GAIN
				Write( ES7210_Const::MIC4_GAIN_REG46, 0x00 ); // MIC4_GAIN
				Write( ES7210_Const::MIC1_POWER_REG47, 0x00 ); // MIC1_LP
				Write( ES7210_Const::MIC2_POWER_REG48, 0x00 ); // MIC2_LP
				Write( ES7210_Const::MIC3_POWER_REG49, 0x00 ); // MIC3_LP
				Write( ES7210_Const::MIC4_POWER_REG4A, 0x00 ); // MIC4_LP
				Write( ES7210_Const::MIC12_POWER_REG4B, 0x00 ); // MIC12_PDN
				Write( ES7210_Const::MIC34_POWER_REG4C, 0xFF ); // MIC34_PDN
				Write( ES7210_Const::CLOCK_OFF_REG01, 0x14 ); // CLK_ON_OFF
			}

/*
			if( Enabled() )
			{
	            Core2_AXP192_Func::Write1Byte( 0x94, Core2_AXP192_Func::Read8bit( 0x94 ) | 0b00000100 );
			}

			else
			{
				Core2_AXP192_Func::Write1Byte( 0x94, Core2_AXP192_Func::Read8bit( 0x94 ) & 0b11111011 );
			}
*/
/*
			if( Enabled() )
			{
				i2s_pin_config_t pin_config =
				{
					.mck_io_num = I2S_PIN_NO_CHANGE,
					.bck_io_num = 12,
					.ws_io_num = 0,
					.data_out_num = 2,
					.data_in_num = 34
				};

                i2s_set_pin( I2S_NUM_0, &pin_config );
                xTaskCreate(
                    onDmaInputComplete,   // Function to implement the task
                    "onM5StackDmaInputComplete", // Name of the task
                    20000,                   // Stack size in words
                    NULL,                    // Task input parameter
                    2,                       // Priority of the task
                    &FCallbackTaskInputHandle     // Task handle.
                );

                i2s_set_clk( I2S_NUM_0, AInputConfig.sample_rate, AInputConfig.bits_per_sample, ( AInputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
			}

			else
			{
			}
*/
		}

/*
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			C_OWNER.AudioOutputChannel_InputPin_o_Receive( _Data );

//    		TAudioBuffer &ABuffer = *(TAudioBuffer *)_Data;
		}
*/
	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

	public:
		inline M5Stack_CoreS3_Microphones()
		{
			C_OWNER.AddControlCallback( this, &ControlCallback );
			C_OWNER.AddOutputCallback( this, &OutputCallback );
		}

	};
//---------------------------------------------------------------------------
} // Mitov
