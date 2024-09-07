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
#include <Mitov_Core2_AXP192.h>
#include <Mitov_I2S_Amplifier_NS4168.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_Enabled
	> class M5Stack_Core2_Speaker :
		public TArduinoI2SAmplifierNS4168 <	
				44100,
				Mitov::ConstantProperty<5, bool, false > // RightChannel
			>,
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	protected:
		static void ControlCallback( void *AInstance, void *_Data )
		{
//            Serial.println( "TEST1_1" );
//			delay( 1000 );
			if( ! ((TArduinoI2SAmplifierNS4168 *)AInstance )->Enabled().GetValue() )
				return;

			((TArduinoI2SAmplifierNS4168 *)AInstance )->ControlInputPin_o_Receive( _Data );
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
			{
	            Core2_AXP192_Func::Write1Byte( 0x94, Core2_AXP192_Func::Read8bit( 0x94 ) | 0b00000100 );
			}

			else
			{
				Core2_AXP192_Func::Write1Byte( 0x94, Core2_AXP192_Func::Read8bit( 0x94 ) & 0b11111011 );
			}
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

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			C_OWNER.AudioOutputChannel_InputPin_o_Receive( _Data );

//    		TAudioBuffer &ABuffer = *(TAudioBuffer *)_Data;
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

	public:
		inline M5Stack_Core2_Speaker()
		{
			C_OWNER.AddControlCallback( this, &ControlCallback );
		}

	};
//---------------------------------------------------------------------------
} // Mitov
