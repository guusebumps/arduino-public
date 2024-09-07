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
#include <Mitov_Avinic_I2S_Amplifier_AW88298.h>

//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_I2C, T_I2C &C_I2C,
		typename T_Enabled,
		typename T_Mute,
		typename T_Volume
	> class M5Stack_CoreS3_Speaker :
/*
		public TArduinoAwinicAmplifier_I2C_I2S_AW88298 <	
				44100,
				Mitov::ConstantProperty<6, uint8_t, 0x36 > // Address
			>,		
*/
		public T_Enabled,
		public T_Mute,
		public T_Volume
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Mute )
		_V_PROP_( Volume )

	private:
		static const uint8_t C_Address = 0x36;


	protected:
		static void ControlCallback( void *AInstance, void *_Data )
		{
//			USBSerial.begin( 9600 );
//			while( ! USBSerial )
//				;

//            USBSerial.println( "TEST1_1" );
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

                if( AInfo.Output.Modes[ i ].Mode == TI2SMode::Standard )
                {
//                    AInfo.Output.Modes[ i ].SetBitsPerSample( 32, 32 );
                    AInfo.Output.Modes[ i ].SetBitsPerSample( 16, 16 );
                    AInfo.Output.Modes[ i ].SetSampleRate( 24000, 24000 );

//                    AInfo.Output.Modes[ i ].ChannelsMap |= 0b01; // Left channel!
                    AInfo.Output.Modes[ i ].ChannelsMap |= 0b11; // Left channel!
                }

				else
					AInfo.Output.Modes[ i ].Enabled = false;

		    }
		}

	protected:
		void Write(uint8_t ARegister, uint16_t AValue )
		{
			C_I2C.beginTransmission( C_Address );
			C_I2C.write( ARegister );
			C_I2C.write( uint8_t( AValue >> 8 ));
			C_I2C.write( uint8_t( AValue ));
			C_I2C.endTransmission();
		}

	public:
		void Update_SYSCTRL2()
		{
/*
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
*/
			uint16_t AValue = 0x0008;
			if( Mute() )
				AValue |= 0b0000000000010000;

			Write( AW88298_Const::REG_SYSCTRL2, AValue );
		}

		void Update_HAGCCFG4()
		{
			// 0x2064 - 0010 0000 0110 0100
			uint16_t AValue = uint16_t( ( 1 - Volume().GetValue() ) * 255 + 0.5 ) << 8;

/*
			uint16_t AIntValue = AutomaticGainControl().Thresholds().Time().Hold() / 1.33;

			AValue |= AIntValue;
*/
			AValue |= 64;

			Write( AW88298_Const::REG_HAGCCFG4, AValue );
		}

		inline void UpdateEnabled()
		{
/*
			if( Enabled() )
			{
				Write( AW88298_Const::REG_SYSCTRL2, 0x0008 ); // RMSE=0 HAGCE=0 HDCCE=0 HMUTE=0
				Write( AW88298_Const::REG_I2SCTRL, 0x14C8 ); // I2SBCK=0 (BCK mode 16*2)
				Write( AW88298_Const::REG_BSTCTRL2, 0x0673 ); // default:0x6673: BOOST mode disabled
				Write( AW88298_Const::REG_HAGCCFG4, 0x2064 ); // volume setting
				Write( AW88298_Const::REG_SYSCTRL, 0x4040 ); // I2SEN=1 AMPPD=0 PWDN=0
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
//			Write( AW88298_Const::REG_SYSCTRL2, 0x0008 ); // RMSE=0 HAGCE=0 HDCCE=0 HMUTE=0
			Update_SYSCTRL2();
//			Write( AW88298_Const::REG_I2SCTRL, 0x14C8 ); // I2SBCK=0 (BCK mode 16*2) // 0001010011001000
//			Write( AW88298_Const::REG_I2SCTRL, 0b0001010000000101 ); // I2SBCK=0 (BCK mode 16*2) // 16 bit Mono Left
			Write( AW88298_Const::REG_I2SCTRL, 0b0001110000000101 ); // I2SBCK=0 (BCK mode 16*2) // 16 bit Stereo
			Write( AW88298_Const::REG_BSTCTRL2, 0x0673 ); // default:0x6673: BOOST mode disabled
//			Write( AW88298_Const::REG_HAGCCFG4, 0x2064 ); // volume setting
			Update_HAGCCFG4();
			Write( AW88298_Const::REG_SYSCTRL, 0x4040 ); // I2SEN=1 AMPPD=0 PWDN=0
			UpdateEnabled();
		}

	public:
		inline M5Stack_CoreS3_Speaker()
		{
			C_OWNER.AddControlCallback( this, &ControlCallback );
		}

	};
//---------------------------------------------------------------------------
} // Mitov
