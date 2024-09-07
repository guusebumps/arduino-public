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

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	namespace AW88298_Const
	{
		const uint8_t REG_ID		= 0x00;
		const uint8_t REG_SYSST     = 0x01;
		const uint8_t REG_SYSINT    = 0x02;
		const uint8_t REG_SYSINTM   = 0x03;
		const uint8_t REG_SYSCTRL   = 0x04;
		const uint8_t REG_SYSCTRL2  = 0x05;
		const uint8_t REG_I2SCTRL   = 0x06;
		const uint8_t REG_I2SCFG1   = 0x07;
		const uint8_t REG_HAGCCFG1  = 0x09;
		const uint8_t REG_HAGCCFG2  = 0x0A;
		const uint8_t REG_HAGCCFG3  = 0x0B;
		const uint8_t REG_HAGCCFG4  = 0x0C;
		const uint8_t REG_HAGCST    = 0x10;
		const uint8_t REG_VDD		= 0x12;
		const uint8_t REG_TEMP      = 0x13;
		const uint8_t REG_PVDD      = 0x14;
		const uint8_t REG_BSTCTRL1  = 0x60;
		const uint8_t REG_BSTCTRL2  = 0x61;
	}													  
//---------------------------------------------------------------------------
	template <
		typename T_Attack,
		typename T_Release
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_AmplitudeThresholds :
		public T_Attack,
		public T_Release
	{
	public:
		_V_PROP_( Attack )
		_V_PROP_( Release )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Attack,
		typename T_Hold,
		typename T_Release
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_TimeThresholds :
		public T_Attack,
		public T_Hold,
		public T_Release
	{
	public:
		_V_PROP_( Attack )
		_V_PROP_( Hold )
		_V_PROP_( Release )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Amplitude,
		typename T_Time
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_AutomaticGainControl_Thresholds :
		public T_Amplitude,
		public T_Time
	{
	public:
		_V_PROP_( Amplitude )
		_V_PROP_( Time )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Peak,
		typename T_RMS,
		typename T_Thresholds
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_AutomaticGainControl :
		public T_Peak,
		public T_RMS,
		public T_Thresholds
	{
	public:
		_V_PROP_( Peak )
		_V_PROP_( RMS )
		_V_PROP_( Thresholds )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Attack,
		typename T_Release
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoostThresholds :
		public T_Attack,
		public T_Release
	{
	public:
		_V_PROP_( Attack )
		_V_PROP_( Release )

	};
//---------------------------------------------------------------------------
	template <
		typename T_PeakCurrentLimit,
		typename T_Smart1DeglitchTime,
		typename T_Thresholds
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_SmartPowerBoost :
		public T_PeakCurrentLimit,
		public T_Smart1DeglitchTime,
		public T_Thresholds
	{
	public:
		_V_PROP_( PeakCurrentLimit )
		_V_PROP_( Smart1DeglitchTime )
		_V_PROP_( Thresholds )

	};
//---------------------------------------------------------------------------
	namespace TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode
	{
		enum TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode
		{
			Transparent,
			Force,
			Smart1,
			Smart2
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_MaxVoltage,
		typename T_Mode,
		typename T_Smart
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost :
		public T_MaxVoltage,
		public T_Mode,
		public T_Smart
	{
	public:
		_V_PROP_( MaxVoltage )
		_V_PROP_( Mode )
		_V_PROP_( Smart )

	};
//---------------------------------------------------------------------------
	namespace TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels
	{
		enum TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels
		{
			Left,
			Rigth,
			Both
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode
	{
		enum TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode
		{
			Standard = TI2SMode::Standard,  // Phillips
			MSB = TI2SMode::MSB,
			TDM = TI2SMode::TDM				// Multichannel
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_BitsPerSample,
		typename T_I2SChannels,
		typename T_Mode,
		typename T_SampleRate,
		typename T_TDMChannel
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298_Format :
		public T_BitsPerSample,
		public T_I2SChannels,
		public T_Mode,
		public T_SampleRate,
		public T_TDMChannel
	{
	public:
		_V_PROP_( BitsPerSample )
		_V_PROP_( I2SChannels )
		_V_PROP_( Mode )
		_V_PROP_( SampleRate )
		_V_PROP_( TDMChannel )

	};
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,	
		typename T_Attenuate,
		typename T_AutomaticGainControl,
		typename T_Format,
		typename T_Mute,
		typename T_PowerBoost,
		typename T_PowerDown,
		typename T_RemoveDC,
		typename T_Volume
	> class TArduinoAwinicAmplifier_I2C_I2S_AW88298 :
		public T_Address,
		public T_Attenuate,
		public T_AutomaticGainControl,
		public T_Format,
		public T_Mute,
		public T_PowerBoost,
		public T_PowerDown,
		public T_RemoveDC,
		public T_Volume
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Attenuate )
		_V_PROP_( AutomaticGainControl )
		_V_PROP_( Format )
		_V_PROP_( Mute )
		_V_PROP_( PowerBoost )
		_V_PROP_( PowerDown )
		_V_PROP_( RemoveDC )
		_V_PROP_( Volume )

	public:
		void Update_SYSCTRL()
		{
			// 0x4040 - 0100 0000 0100 0000
			uint16_t AValue = 0b001111 + ( ( PowerBoost().MaxVoltage().GetValue() - 5.0 ) / 0.125 );
			Write( AW88298_Const::REG_SYSCTRL, AValue );
		}

		void Update_SYSCTRL2()
		{
			uint16_t AValue = ( AutomaticGainControl().RMS() ) ? 0b0000000010000000 : 0;
			if( AutomaticGainControl().Peak() )
				AValue |= 0b0000000001000000;

			if( RemoveDC() )
				AValue |= 0b0000000000100000;

			if( Mute() )
				AValue |= 0b0000000000010000;

			if( PowerBoost().PeakCurrentLimit() >= 4.25 )
				AValue |= 0b1011;

			else if( PowerBoost().PeakCurrentLimit() >= 4 )
				AValue |= 0b1010;

			else if( PowerBoost().PeakCurrentLimit() >= 3.75 )
				AValue |= 0b1001;

			else if( PowerBoost().PeakCurrentLimit() >= 3.5 )
				AValue |= 0b1000;

			else if( PowerBoost().PeakCurrentLimit() >= 3.25 )
				AValue |= 0b0111;

			else if( PowerBoost().PeakCurrentLimit() >= 3.0 )
				AValue |= 0b0110;

			else if( PowerBoost().PeakCurrentLimit() >= 2.75 )
				AValue |= 0b0101;

			else if( PowerBoost().PeakCurrentLimit() >= 2.5 )
				AValue |= 0b0100;

			else if( PowerBoost().PeakCurrentLimit() >= 2.25 )
				AValue |= 0b0011;

			else if( PowerBoost().PeakCurrentLimit() >= 2.0 )
				AValue |= 0b0010;

			else if( PowerBoost().PeakCurrentLimit() >= 1.75 )
				AValue |= 0b0001;

			Write( AW88298_Const::REG_SYSCTRL2, AValue );
		}

		void Update_I2SCTRL()
		{
//			0x14C8 0001 0100 1100 1000
			uint16_t AValue = ( Attenuate() ) ? 0b0010000000000000 : 0;

			AValue |= 0b0001000000000000; // Enable Receiver

			switch( Format().I2SChannels() )
			{
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Both :	AValue |= 0b0000110000000000; break;
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Rigth :	AValue |= 0b0000100000000000; break;

				default :
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Left :	AValue |= 0b0000010000000000; break;
			}

			switch( Format().Mode() )
			{
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode::MSB :	AValue |= 0b0000000100000000; break;
//				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode::LSB :	AValue |= 0b0000001000000000; break;

				default :
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode::Standard : ; // AValue |= 0;
			}

			if( Format().BitsPerSample() >= 32 )
				AValue |= 0b0000000011000000;

			else if( Format().BitsPerSample() >= 24 )
				AValue |= 0b0000000010000000;

			else if( Format().BitsPerSample() >= 20 )
				AValue |= 0b0000000001000000;

			if( Format().SampleRate() >= 192000 )
				AValue |= 0b0000000000001010;

			else if( Format().SampleRate() >= 96000 )
				AValue |= 0b0000000000001001;

			else if( Format().SampleRate() >= 48000 )
				AValue |= 0b0000000000001000;

			else if( Format().SampleRate() >= 44100 )
				AValue |= 0b0000000000000111;

			else if( Format().SampleRate() >= 32000 )
				AValue |= 0b0000000000000110;

			else if( Format().SampleRate() >= 24000 )
				AValue |= 0b0000000000000101;

			else if( Format().SampleRate() >= 22050 )
				AValue |= 0b0000000000000100;

			else if( Format().SampleRate() >= 16000 )
				AValue |= 0b0000000000000011;

			else if( Format().SampleRate() >= 12000 )
				AValue |= 0b0000000000000010;

			else if( Format().SampleRate() >= 11025 )
				AValue |= 0b0000000000000001;

//			else if( Format().SampleRate() >= 8000 )
//				AValue |= 0b0000000000000000;

			Write( AW88298_Const::REG_I2SCTRL, AValue );
		}

		void Update_I2SCFG1()
		{
			// Do not implement for now! (Transmitter config)
		}

		void Update_HAGCCFG1()
		{
			uint16_t AValue = uint16_t( AutomaticGainControl().Thresholds().Amplitude().Release().GetValue() * 255 + 0.5 ) << 8;
			AValue |= uint16_t( AutomaticGainControl().Thresholds().Amplitude().Attack().GetValue() * 255 + 0.5 );

			Write( AW88298_Const::REG_HAGCCFG1, AValue );
		}

		void Update_HAGCCFG2()
		{
			uint16_t AValue = ( AutomaticGainControl().Thresholds().Time().Attack().GetValue() / 20.8 ) * 0xFFFF + 0.5;
			Write( AW88298_Const::REG_HAGCCFG2, AValue );
		}

		void Update_HAGCCFG3()
		{
			uint16_t AValue = ( AutomaticGainControl().Thresholds().Time().Release().GetValue() / 20.8 ) * 0xFFFF + 0.5;
			Write( AW88298_Const::REG_HAGCCFG3, AValue );
		}

		void Update_HAGCCFG4()
		{
			// 0x2064 - 0010 0000 0110 0100
			uint16_t AValue = uint16_t( Volume().GetValue() * 255 + 0.5 ) << 8;

			uint16_t AIntValue = AutomaticGainControl().Thresholds().Time().Hold() / 1.33;

			AValue |= AIntValue;

			Write( AW88298_Const::REG_HAGCCFG4, AValue );
		}

		void Update_BSTCTRL1()
		{
			uint16_t AValue = uint16_t( PowerBoost().Smart().Thresholds().Release() * 0b111111 + 0.5 ) << 8;

			uint16_t AIntValue = PowerBoost().Smart().Thresholds().Attack() * 0b111111 + 0.5;

			AValue |= AIntValue;
			Write( AW88298_Const::REG_BSTCTRL1, AValue );
		}

		void Update_BSTCTRL2()
		{
			// 0x0673 - 0000 0110 0111 0011
			uint16_t AValue;
			switch( PowerBoost().Mode() )
			{
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode::Transparent :	break; // 0
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode::Force :	AValue |= 0b0001000000000000; break;
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode::Smart1 :	AValue |= 0b0101000000000000; break;

				default :
				case TArduinoAwinicAmplifier_I2C_I2S_AW88298_PowerBoost_Mode::Smart2 :	AValue |= 0b0110000000000000; break;
			}

			if( PowerBoost().Smart().Smart1DeglitchTime() >= 1400.0 )
				AValue |= 0b0000011100000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 352.0 )
				AValue |= 0b0000011000000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 88.0 )
				AValue |= 0b0000010100000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 44 )
				AValue |= 0b0000010000000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 21.3 )
				AValue |= 0b0000001100000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 5.6 )
				AValue |= 0b0000001000000000;

			else if( PowerBoost().Smart().Smart1DeglitchTime() >= 1.4 )
				AValue |= 0b0000000100000000;

			uint16_t AIntValue = 0b001111 + ( ( PowerBoost().MaxVoltage().GetValue() - 5.0 ) / 0.125 );

			AValue |= AIntValue; 

			Write( AW88298_Const::REG_BSTCTRL2, AValue );
		}

	protected:
		void Write( uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( uint8_t( Address() ));
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.write( uint8_t( AValue ));
			C_I2C.endTransmission();
		}

	public:
		inline void ControlInputPin_o_Receive( void *_Data )
		{
//            Serial.println( "TEST1_1" );
//			delay( 1000 );
//			((TArduinoI2SAmplifierNS4168 *)AInstance )->ControlInputPin_o_Receive( _Data );

		    TI2SInfo &AInfo = *(TI2SInfo *)_Data;

		    AInfo.Output.Used = true;

//            Serial.println( "TEST1" );
//            delay( 10000 );

		    for( int i = 0; i < AInfo.Output.Modes.size(); ++ i )
		    {
                if( ! AInfo.Output.Modes[ i ].Enabled )
                    continue;

                if( AInfo.Output.Modes[ i ].Mode == Format().Mode().GetValue() )
                {
                    AInfo.Output.Modes[ i ].SetBitsPerSample( Format().BitsPerSample(), Format().BitsPerSample() );
                    AInfo.Output.Modes[ i ].SetSampleRate( Format().SampleRate(), Format().SampleRate() );

					if( Format().Mode().GetValue() == TArduinoAwinicAmplifier_I2C_I2S_AW88298_I2SMode::TDM )
						AInfo.Output.Modes[ i ].ChannelsMap |= 1 << Format().TDMChannel().GetValue();

					else
					{
						switch( Format().I2SChannels() )
						{
							case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Left :	AInfo.Output.Modes[ i ].ChannelsMap |= 0b10;
							case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Rigth :	AInfo.Output.Modes[ i ].ChannelsMap |= 0b01;

							default :
							case TArduinoAwinicAmplifier_I2C_I2S_AW88298_Channels::Both :	AInfo.Output.Modes[ i ].ChannelsMap |= 0b11;
						}
					}

                }

				else
					AInfo.Output.Modes[ i ].Enabled = false;

		    }
		}

	public:
		inline void SystemInit()
		{
			Update_SYSCTRL();
			Update_SYSCTRL2();
			Update_I2SCTRL();
			Update_I2SCFG1();
			Update_HAGCCFG1();
			Update_HAGCCFG2();
			Update_HAGCCFG3();
			Update_HAGCCFG4();
			Update_BSTCTRL1();
			Update_BSTCTRL2();
		}

	};
//---------------------------------------------------------------------------
} // Mitov
