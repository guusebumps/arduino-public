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
	namespace ES7210_Const
	{
		const uint8_t RESET_REG00                 = 0x00;        // Reset control
		const uint8_t CLOCK_OFF_REG01             = 0x01;        // Used to turn off the ADC clock
		const uint8_t MAINCLK_REG02               = 0x02;        // Set ADC clock frequency division
		const uint8_t MASTER_CLK_REG03            = 0x03;        // MCLK source $ SCLK division
		const uint8_t LRCK_DIVH_REG04             = 0x04;        // lrck_divh
		const uint8_t LRCK_DIVL_REG05             = 0x05;        // lrck_divl
		const uint8_t POWER_DOWN_REG06            = 0x06;        // power down
		const uint8_t OSR_REG07                   = 0x07;
		const uint8_t MODE_CONFIG_REG08           = 0x08;        // Set master/slave & channels
		const uint8_t TIME_CONTROL0_REG09         = 0x09;        // Set Chip intial state period
		const uint8_t TIME_CONTROL1_REG0A         = 0x0A;        // Set Power up state period
		const uint8_t SDP_INTERFACE1_REG11        = 0x11;        // Set sample & fmt
		const uint8_t SDP_INTERFACE2_REG12        = 0x12;        // Pins state
		const uint8_t ADC_AUTOMUTE_REG13          = 0x13;        // Set mute
		const uint8_t ADC34_MUTERANGE_REG14       = 0x14;        // Set mute range
		const uint8_t ADC34_HPF2_REG20            = 0x20;        // HPF
		const uint8_t ADC34_HPF1_REG21            = 0x21;
		const uint8_t ADC12_HPF1_REG22            = 0x22;
		const uint8_t ADC12_HPF2_REG23            = 0x23;
		const uint8_t ANALOG_REG40                = 0x40;        // ANALOG Power
		const uint8_t MIC12_BIAS_REG41            = 0x41;
		const uint8_t MIC34_BIAS_REG42            = 0x42;
		const uint8_t MIC1_GAIN_REG43             = 0x43;
		const uint8_t MIC2_GAIN_REG44             = 0x44;
		const uint8_t MIC3_GAIN_REG45             = 0x45;
		const uint8_t MIC4_GAIN_REG46             = 0x46;
		const uint8_t MIC1_POWER_REG47            = 0x47;
		const uint8_t MIC2_POWER_REG48            = 0x48;
		const uint8_t MIC3_POWER_REG49            = 0x49;
		const uint8_t MIC4_POWER_REG4A            = 0x4A;
		const uint8_t MIC12_POWER_REG4B           = 0x4B;        // MICBias & ADC & PGA Power
		const uint8_t MIC34_POWER_REG4C           = 0x4C;
	}													  
//---------------------------------------------------------------------------
} // Mitov
