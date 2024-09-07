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
#include <Mitov_ESP32Camera.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif
//---------------------------------------------------------------------------
// esp32-camera sccb.c
extern "C"
{
    int SCCB_Write(uint8_t slv_addr, uint8_t reg, uint8_t data); 
    uint8_t SCCB_Read(uint8_t slv_addr, uint8_t reg);
}
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
namespace cam { namespace GC0308  {

	template<typename T, size_t N> constexpr auto size(const T(&)[N]) noexcept -> size_t { return N; }

	enum SpecialEffect : int8_t
	{
		NoEffect,  //!< @brief No effect
		Negative,  //!< @brief Negative effect
		Grayscale, //!< @brief Grayscale effect
		RedTint,   //!< @brief Red tint effect
		GreenTint, //!< @brief Green tint effect
		BlueTint,  //!< @brief Blue tint effect
		Sepia,     //!< @brief Sepia effect
	};

	//! @enum WhiteBalance
	enum WhiteBalance : int8_t
	{
		Auto,    //!< Automatic
		Sunny,   //!< Manual (Sunny)
		Cloudy,  //!< Manual (Cloudy)
		Office,  //!< Manual (Office)
		Home,    //!< Manual (Home)
	};

	/*!
	  @brief complement esp32-camera GC0308 driver
	  @warning Must be call after esp_camera_init() once.
	  @note Delete set_gain_ctrl
	  @note Add set_agc_gain
	  @note Add set_specia_effect
	  @note Add set_wb_mode
	  @note Replace set_contrast
	  @retval true Success
	  @retval false Failure
	*/

//	bool complementDriver();
//---------------------------------------------------------------------------
constexpr uint8_t REG_CONTRAST = 0xb3;
constexpr uint8_t REG_AGC_GAIN = 0x50;

struct RegVal { const uint8_t reg; const uint8_t val; };
struct Params { const size_t sz;   const RegVal* params; };

// For special effect
PROGMEM const RegVal special_no_effect[] =
{
    { 0x23, 0x00 }, // special_effect [7:3] NA [2] edge_map [1] CbCr fixed enable [0] Inverse color
    { 0x20, 0x7f }, // Block_enable_1  [7] BKS_en [6] gamma enable [5] CC enable [4] Edge enhancement enable [3] Interpolation enable [2] Noise removal enable [1] Defect removal enable [0] Lens‐shading correction enabl
    { 0xd2, 0x90 }, // AEC_mode2 [7] AEC_en [6] map measure point [5] color Y mode [4] skin weight mode [3] NA [2] color select [1:0] NA
    { 0x73, 0x00 }, // EINTP mode 2  [7] edge_add_mode2 [6]NA [5] direction mode [4] fixed direction threshold [3:0] reserved
    { 0x77, 0x38 }, // Edge effect [7:4] edge effect1 [3:0] edge effect 2
    { 0xb3, 0x40 }, // Luma_contrast, can be adjusted viacontrast center 2.6bits, 0x40=1.0
    { 0xb4, 0x80 }, // Contrast center value
    { 0xba, 0x00 }, // S7, if fixed CbCr function is enabled, current image Cb value will be replace by this value to achieve special effect
    { 0xbb, 0x00 }, // S7, if fixed CbCr function is enabled, current image Cr value will be replace by this value to achieve special effect
};
PROGMEM const RegVal special_negative[] =
{
    { 0x23, 0x01 },
    { 0x20, 0x7f },
    { 0xd2, 0x90 },
    { 0x73, 0x00 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0x00 },
    { 0xbb, 0x00 },
};

PROGMEM const RegVal special_grayscale[] =
{
    { 0x23, 0x02 },
    { 0x20, 0xff },
    { 0xd2, 0x90 },
    { 0x73, 0x00 },
    { 0x77, 0x54 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0x00 },
    { 0xbb, 0x00 },
};

PROGMEM const RegVal special_red[] =
{
    { 0x23, 0x02 },
    { 0x20, 0x7f },
    { 0xd2, 0x90 },
    { 0x77, 0x00 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0xe0 },
    { 0xbb, 0x50 },
};

PROGMEM const RegVal special_green[] =
{
    { 0x23, 0x02 },
    { 0x20, 0xff },
    { 0xd2, 0x90 },
    { 0x77, 0x88 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0xc0 },
    { 0xbb, 0xc0 },
};

PROGMEM const RegVal special_blue[] =
{
    { 0x23, 0x02 },
    { 0x20, 0xff },
    { 0xd2, 0x90 },
    { 0x77, 0x00 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0x50 },
    { 0xbb, 0xe0 },
};

PROGMEM const RegVal special_sepia[] =
{
    { 0x23, 0x02 },
    { 0x20, 0xff },
    { 0xd2, 0x90 },
    { 0x73, 0x00 },
    { 0xb3, 0x40 },
    { 0xb4, 0x80 },
    { 0xba, 0xd0 },
    { 0xbb, 0x20 },
};

PROGMEM const Params param_special_effect[] =
{
    { size(special_no_effect), special_no_effect },
    { size(special_negative),  special_negative },
    { size(special_grayscale), special_grayscale },
    { size(special_red),       special_red },
    { size(special_green),     special_green },
    { size(special_blue),      special_blue },
    { size(special_sepia),     special_sepia },
};

// For White balance
PROGMEM const RegVal wb_auto[] =
{
    { 0x5a, 0x50 }, // AWB_R_gain 2.6 bits red channel gain from AWB
    { 0x5b, 0x40 }, // AWB_G_gain 2.6 bits green channel gain from AWB
    { 0x5c, 0x48 }, // AWB_B_gain 2.6 bits blue channel gain from AWB
};

PROGMEM const RegVal wb_sunny[] =
{
    { 0x5a, 0x5e },
    { 0x5b, 0x41 },
    { 0x5c, 0x54 },
};

PROGMEM const RegVal wb_cloudy[] =
{
    { 0x5a, 0x65 },
    { 0x5b, 0x41 },
    { 0x5c, 0x4f },
};

PROGMEM const RegVal wb_office[] =
{
    { 0x5a, 0x52 },
    { 0x5b, 0x41 },
    { 0x5c, 0x66 },
};

PROGMEM const RegVal wb_home[] =
{
    { 0x5a, 0x42 },
    { 0x5b, 0x3f },
    { 0x5c, 0x71 },
};


PROGMEM const Params param_wb[] =
{
    { size(wb_auto),   wb_auto   },
    { size(wb_sunny),  wb_sunny  },
    { size(wb_cloudy), wb_cloudy },
    { size(wb_office), wb_office },
    { size(wb_home),   wb_home   },
};

// --------------------------------
// Camera methods

//using cam::GC0308::SpecialEffect::NoEffect;
//using cam::GC0308::SpecialEffect::Sepia;
//using cam::GC0308::WhiteBalance::Auto;
//using cam::GC0308::WhiteBalance::Home;

//int set_dummy(sensor_t*, int) { return -1; }

int set_contrast(sensor_t *s, int contrast)
{
    s->status.contrast = contrast;
    int ret = SCCB_Write(s->slv_addr, 0xFE, 0); // page 0
    ret |= SCCB_Write(s->slv_addr, REG_CONTRAST, contrast); //2.6bits, 0x40=1.x
    return ret;
}

int set_agc_gain(sensor_t* s, int val)
{
//    if(val < 0 || val > 63) { return -1; }

    s->status.agc_gain = val;
    int ret = SCCB_Write(s->slv_addr, 0xFE, 0); // page 0
    ret |= SCCB_Write(s->slv_addr, REG_AGC_GAIN, val); // [5:0] global_gain, 2.4bits, 0x10 is 1.0x
    return ret;
}

int set_special_effect( sensor_t *s, int effect )
{
//    if(effect < NoEffect || effect > Sepia) { return -1; }

    s->status.special_effect = effect;

    int ret = SCCB_Write(s->slv_addr, 0xFE, 0); // page 0
    size_t sz = param_special_effect[effect].sz;
    auto p = param_special_effect[effect].params;
    while(!ret && sz--)
    {
        ret |= SCCB_Write(s->slv_addr, p->reg, p->val);
        ++p;
    }

    return ret;
}

int set_wb_mode( sensor_t *s, int mode )
{
//    if(mode < Auto || mode > Home) { return -1; }

    s->status.wb_mode = mode;

    int ret = SCCB_Write(s->slv_addr, 0xFE, 0); // page 0
    
    uint8_t rval = SCCB_Read(s->slv_addr, 0x22); // AAAA_enable  [1] AWB enable
    rval = (rval & ~0x02U) | ((mode == Auto) ? 0x02 : 0x00);
    ret |= SCCB_Write(s->slv_addr, 0x22, rval);
    
    size_t sz = param_wb[mode].sz;
    auto p = param_wb[mode].params;
    while(!ret && sz--)
    {
        ret |= SCCB_Write(s->slv_addr, p->reg, p->val);
        ++p;
    }

    return ret;
}
//
}
}
//---------------------------------------------------------------------------
	namespace TArduinoM5StackCoreS3_ImageFormat
	{
		enum TArduinoM5StackCoreS3_ImageFormat
		{
			RGB565 = TImageFormat::RGB565,
			RGB555 = TImageFormat::RGB555,
			GrayScale = TImageFormat::GrayScale
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_ColorBar,
		typename T_Contrast,
		typename T_CountBuffers,
		typename T_Enabled,
		typename T_Gain,
		typename T_GrabMode,
		typename T_ImageSize,
		typename T_Mirror,
		typename T_OutputPin,
		typename T_PixelFormat,
		typename T_SpecialEffect,
		typename T_WhiteBalance
   > class TArduinoM5StackCoreS3_CameraModule :
		public ESP32BasicCamera <
				1, // C_I2C_CHANNEL
				38, // C_PIN_HREF
				45, // C_PIN_PCLK
				-1, // C_PIN_POWER_DOWN
				-1, // C_PIN_RESET
				-1, // C_PIN_SIOC
				-1, // C_PIN_SIOD
				46, // C_PIN_VSYNC
				-1, // C_PIN_XCLK
				39, // C_PIN_Y2
				40, // C_PIN_Y3
				41, // C_PIN_Y4
				42, // C_PIN_Y5
				15, // C_PIN_Y6
				16, // C_PIN_Y7
				48, // C_PIN_Y8
				47, // C_PIN_Y9
				T_CountBuffers,
				T_Enabled,
				T_GrabMode,
				T_ImageSize,
				Mitov::ConstantProperty<5, uint32_t, 0 >, // JpegQuality
				T_OutputPin,
				T_PixelFormat,
				Mitov::ConstantProperty<99, bool, true > // UsePSRAM
			>,
		public T_ColorBar,
		public T_Contrast,
		public T_Gain,
		public T_Mirror,
		public T_SpecialEffect,
		public T_WhiteBalance
	{
		typedef ESP32BasicCamera <
				1, // C_I2C_CHANNEL
				38, // C_PIN_HREF
				45, // C_PIN_PCLK
				-1, // C_PIN_POWER_DOWN
				-1, // C_PIN_RESET
				-1, // C_PIN_SIOC
				-1, // C_PIN_SIOD
				46, // C_PIN_VSYNC
				-1, // C_PIN_XCLK
				39, // C_PIN_Y2
				40, // C_PIN_Y3
				41, // C_PIN_Y4
				42, // C_PIN_Y5
				15, // C_PIN_Y6
				16, // C_PIN_Y7
				48, // C_PIN_Y8
				47, // C_PIN_Y9
				T_CountBuffers,
				T_Enabled,
				T_GrabMode,
				T_ImageSize,
				Mitov::ConstantProperty<5, uint32_t, 0 >, // JpegQuality
				T_OutputPin,
				T_PixelFormat,
				Mitov::ConstantProperty<99, bool, true > // UsePSRAM
			> inherited;

	public:
		_V_PROP_( ColorBar )
		_V_PROP_( Contrast )
		_V_PROP_( Gain )
        _V_PROP_( Mirror )
		_V_PROP_( SpecialEffect )
		_V_PROP_( WhiteBalance )

	public:
		void UpdateEnabled()
		{
			inherited::UpdateEnabled();

			if( inherited::Enabled() )
			{
				auto s = esp_camera_sensor_get();
//				USBSerial.println( "s->id.PID" );
//				USBSerial.println( s->id.PID );
//				USBSerial.println( "s->id.PID" );
//				USBSerial.println( uint64_t( s ));
//				USBSerial.println( s->id.PID );
//				USBSerial.println( "TEST0" );
				if( !s || s->id.PID != GC0308_PID )
					return;
//					{ log_e("GC0308 not detected"); return false; }

//				USBSerial.println( "TEST1" );
				/*
				  Delete set_gain_ctrl
				  esp32-camera registers a function equivalent to set_agc_gain.
				  Is this a mistake since this method is supposed to turn on/off gain_ctrl?
				*/
//				s->set_gain_ctrl = cam::GC0308::set_dummy;
				s->status.agc = 0;

				// Add agc gain
				s->set_agc_gain = cam::GC0308::set_agc_gain;
				s->status.agc_gain = SCCB_Read(s->slv_addr, cam::GC0308::REG_AGC_GAIN);

//				delay( 10000 );
//				USBSerial.println( s->status.agc_gain );

				// Add special effect
				s->set_special_effect = cam::GC0308::set_special_effect;
				s->status.special_effect = cam::GC0308::SpecialEffect::NoEffect;
				// Add wb_mode
				s->set_wb_mode = cam::GC0308::set_wb_mode;
				s->status.wb_mode = cam::GC0308::WhiteBalance::Auto;

				// Replace set_contrast (Because the esp32-camera does not set a value to the status)
				s->set_contrast = cam::GC0308::set_contrast;
				s->status.contrast = SCCB_Read(s->slv_addr, cam::GC0308::REG_CONTRAST);
			}
		}

		void Update_SpecialEffect()
		{
			auto s = esp_camera_sensor_get();
			s->set_special_effect( s, SpecialEffect() );
		}

		void Update_WhiteBalance()
		{
			auto s = esp_camera_sensor_get();
			s->set_wb_mode( s, WhiteBalance() );
		}

		void Update_ColorBar()
		{
			auto s = esp_camera_sensor_get();
			s->set_colorbar( s, ColorBar().GetValue() ? 1 : 0 );
		}

		void Update_Contrast()
		{
			auto s = esp_camera_sensor_get();
			s->set_contrast( s, ( Contrast() * 127 ) + 0.5 );
		}

		void Update_agc_gain()
		{
			auto s = esp_camera_sensor_get();
			s->set_agc_gain( s, ( Gain().GetValue() * 63 ) + 0.5 );
		}

		void Update_Mirror_Horizontal()
		{
			auto s = esp_camera_sensor_get();
			s->set_hmirror( s, Mirror().Horizontal().GetValue() ? 1 : 0 );
		}

		void Update_Mirror_Vertical()
		{
			auto s = esp_camera_sensor_get();
			s->set_vflip( s, Mirror().Vertical().GetValue() ? 1 : 0 );
		}

	public:
		inline void SystemStart()
		{
//			delay( 10000 );
//			USBSerial.println( "TEST0_1" );
			UpdateEnabled();
			Update_SpecialEffect();
			Update_Contrast();
			Update_WhiteBalance();
			Update_ColorBar();
			Update_agc_gain();
			Update_Mirror_Horizontal();
			Update_Mirror_Vertical();
			inherited::ReadFrame();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif