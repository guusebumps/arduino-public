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
#include <esp_camera.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoESP32CamImageFormat
	{
		enum TArduinoESP32CamImageFormat
		{
//			RGB24 = TImageFormat::RGB24,
			RGB565 = TImageFormat::RGB565,
			RGB555 = TImageFormat::RGB555,
			GrayScale = TImageFormat::GrayScale,
			JPEG = TImageFormat::JPEG
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoESP32CamImageSize
	{
		enum TArduinoESP32CamImageSize
		{    
			Size_96x96,
			Size_160x120,
			Size_176x144,
			Size_240x176,
			Size_240x240,
			Size_320x240,
			Size_400x296,
			Size_480x320,
			Size_640x480,
			Size_800x600,
			Size_1024x768,
			Size_1280x720,
			Size_1280x1024,
			Size_1600x1200   
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoESP32CamImageSize_640
	{
		enum TArduinoESP32CamImageSize_640
		{    
			Size_96x96 = TArduinoESP32CamImageSize::Size_96x96,
			Size_160x120 = TArduinoESP32CamImageSize::Size_160x120,
			Size_176x144 = TArduinoESP32CamImageSize::Size_176x144,
			Size_240x176 = TArduinoESP32CamImageSize::Size_240x176,
			Size_240x240 = TArduinoESP32CamImageSize::Size_240x240,
			Size_320x240 = TArduinoESP32CamImageSize::Size_320x240,
			Size_400x296 = TArduinoESP32CamImageSize::Size_400x296,
			Size_480x320 = TArduinoESP32CamImageSize::Size_480x320,
			Size_640x480 = TArduinoESP32CamImageSize::Size_640x480
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoESP32CamGrabMode
	{
		enum TArduinoESP32CamGrabMode
		{  
			WhenEmpty,
			Latest    
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoESP32_Camera_SpecialEffect
	{
		enum TArduinoESP32_Camera_SpecialEffect
		{
			None,
			Negative,
			Grayscale,
			RedTint,
			GreenTint,
			BlueTint,
			Sepia
		};
	}
//---------------------------------------------------------------------------
	namespace TArduinoESP32_Camera_WhiteBalanceEnum
	{
		enum TArduinoESP32_Camera_WhiteBalanceEnum
		{
			Auto,
			Sunny,
			Cloudy,
			Office,
			Home
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_CorrectionValue,
		typename T_DSP,
		typename T_Enabled,
		typename T_Level
	> class TArduinoESP32_Camera_AutomaticExposure :
		public T_CorrectionValue,
		public T_DSP,
		public T_Enabled,
		public T_Level
	{
	public:
		_V_PROP_( CorrectionValue )
		_V_PROP_( DSP )
		_V_PROP_( Enabled )
		_V_PROP_( Level )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Ceiling,
		typename T_Enabled,
		typename T_Value
	> class TArduinoESP32_Camera_GainControl :
		public T_Ceiling,
		public T_Enabled,
		public T_Value
	{
	public:
		_V_PROP_( Ceiling )
		_V_PROP_( Enabled )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gain,
		typename T_Value
	> class TArduinoESP32_Camera_WhiteBalance :
		public T_Enabled,
		public T_Gain,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Gain )
		_V_PROP_( Value )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Black,
		typename T_White
	> class TArduinoESP32_Camera_PixelCorrection :
		public T_Black,
		public T_White
	{
	public:
		_V_PROP_( Black )
		_V_PROP_( White )

	};
//---------------------------------------------------------------------------
	template <
		int C_I2C_CHANNEL,
		int C_PIN_HREF,
  		int C_PIN_PCLK,
		int C_PIN_POWER_DOWN,
		int C_PIN_RESET,
		int C_PIN_SIOC,
		int C_PIN_SIOD,
		int C_PIN_VSYNC,
		int C_PIN_XCLK,
		int C_PIN_Y2,
		int C_PIN_Y3,
		int C_PIN_Y4,
		int C_PIN_Y5,
		int C_PIN_Y6,
		int C_PIN_Y7,
		int C_PIN_Y8,
		int C_PIN_Y9,
		typename T_CountBuffers,
		typename T_Enabled,
		typename T_GrabMode,
		typename T_ImageSize,
		typename T_JpegQuality,
		typename T_OutputPin,
		typename T_PixelFormat,
		typename T_UsePSRAM
    > class ESP32BasicCamera :
		public T_CountBuffers,
		public T_Enabled,
		public T_GrabMode,
		public T_ImageSize,
		public T_JpegQuality,
		public T_OutputPin,
		public T_PixelFormat,
		public T_UsePSRAM
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( CountBuffers )
		_V_PROP_( Enabled )
		_V_PROP_( GrabMode )
		_V_PROP_( ImageSize )
		_V_PROP_( JpegQuality )
		_V_PROP_( PixelFormat )
		_V_PROP_( UsePSRAM )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				camera_config_t config;
				config.ledc_channel = LEDC_CHANNEL_0;
				config.ledc_timer = LEDC_TIMER_0;
				config.pin_d0 = C_PIN_Y2;
				config.pin_d1 = C_PIN_Y3;
				config.pin_d2 = C_PIN_Y4;
				config.pin_d3 = C_PIN_Y5;
				config.pin_d4 = C_PIN_Y6;
				config.pin_d5 = C_PIN_Y7;
				config.pin_d6 = C_PIN_Y8;
				config.pin_d7 = C_PIN_Y9;
				config.pin_xclk = C_PIN_XCLK;
				config.pin_pclk = C_PIN_PCLK;
				config.pin_vsync = C_PIN_VSYNC;
				config.pin_href = C_PIN_HREF;
				config.pin_sccb_sda = C_PIN_SIOD;
				config.pin_sccb_scl = C_PIN_SIOC;
				config.pin_pwdn = C_PIN_POWER_DOWN;
				config.pin_reset = C_PIN_RESET;
				config.xclk_freq_hz = 20000000;
//				config.frame_size = FRAMESIZE_UXGA;
//				config.frame_size = FRAMESIZE_96X96;
				config.frame_size = FRAMESIZE_QQVGA;
//				config.pixel_format = PIXFORMAT_JPEG; // for streaming
				config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
//				config.pixel_format = PIXFORMAT_RGB888;
//				config.pixel_format = PIXFORMAT_GRAYSCALE;
//				config.pixel_format = PIXFORMAT_RAW;
				config.grab_mode = ( GrabMode().GetValue() == TArduinoESP32CamGrabMode::WhenEmpty ) ? CAMERA_GRAB_WHEN_EMPTY : CAMERA_GRAB_LATEST;
				config.fb_location = ( UsePSRAM().GetValue() ) ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;
				config.jpeg_quality = JpegQuality();
				config.fb_count = CountBuffers();
				config.sccb_i2c_port = C_I2C_CHANNEL;

//    config.grab_mode     = CAMERA_GRAB_WHEN_EMPTY;
//    config.fb_location   = CAMERA_FB_IN_PSRAM;

//				if( T_JPEGOutputPin::GetPinIsConnected() && ( ! T_OutputPin::GetPinIsConnected() ) )
//					config.pixel_format = PIXFORMAT_JPEG;

//				else
//				{
					switch( PixelFormat().GetValue() )
					{
//						case TArduinoESP32CamImageFormat::RGB24 : config.pixel_format = PIXFORMAT_RGB565; break;
						case TArduinoESP32CamImageFormat::RGB565 : config.pixel_format = PIXFORMAT_RGB565; break;
						case TArduinoESP32CamImageFormat::RGB555 : config.pixel_format = PIXFORMAT_RGB555; break;
						case TArduinoESP32CamImageFormat::JPEG : config.pixel_format = PIXFORMAT_JPEG; break;

						default:
						case TArduinoESP32CamImageFormat::GrayScale : config.pixel_format = PIXFORMAT_GRAYSCALE; break;
					}
//				}

//					config.pixel_format = PIXFORMAT_RGB888;


				switch( ImageSize().GetValue() )
				{
					case TArduinoESP32CamImageSize::Size_96x96 : 
							config.frame_size = FRAMESIZE_96X96;
							config.pixel_format = PIXFORMAT_GRAYSCALE; break;
							break;

					case TArduinoESP32CamImageSize::Size_160x120 : config.frame_size = FRAMESIZE_QQVGA; break;
					case TArduinoESP32CamImageSize::Size_176x144 : config.frame_size = FRAMESIZE_QCIF; break;
					case TArduinoESP32CamImageSize::Size_240x176 : config.frame_size = FRAMESIZE_HQVGA; break;
					case TArduinoESP32CamImageSize::Size_240x240 : 
							config.frame_size = FRAMESIZE_240X240;
							config.pixel_format = PIXFORMAT_GRAYSCALE; break;
							break;

					case TArduinoESP32CamImageSize::Size_320x240 : config.frame_size = FRAMESIZE_QVGA; break;
					case TArduinoESP32CamImageSize::Size_400x296 : config.frame_size = FRAMESIZE_CIF; break;
					case TArduinoESP32CamImageSize::Size_480x320 : config.frame_size = FRAMESIZE_HVGA; break;
					case TArduinoESP32CamImageSize::Size_640x480 : config.frame_size = FRAMESIZE_VGA; break;
					case TArduinoESP32CamImageSize::Size_800x600 : config.frame_size = FRAMESIZE_SVGA; break;
					case TArduinoESP32CamImageSize::Size_1024x768 : config.frame_size = FRAMESIZE_XGA; break;
					case TArduinoESP32CamImageSize::Size_1280x720 : config.frame_size = FRAMESIZE_HD; break;
					case TArduinoESP32CamImageSize::Size_1280x1024 : config.frame_size = FRAMESIZE_SXGA; break;

					default:
					case TArduinoESP32CamImageSize::Size_1600x1200 : config.frame_size = FRAMESIZE_UXGA; break;
				}

//				config.frame_size = FRAMESIZE_QSXGA;

				esp_camera_init( &config );
			}

			else
				esp_camera_deinit();

		}

	protected:
		void ReadFrame()
		{
//					USBSerial.println( "***" );

			if( ! Enabled().GetValue() )
				return;

			camera_fb_t *fb = esp_camera_fb_get();
  
			if( fb )
			{
/*
				USBSerial.print( "Frame: " );
				USBSerial.print( fb->width );
				USBSerial.print( " x " );
				USBSerial.println( fb->height );
*/
/*
				if( fb->format == PIXFORMAT_RGB888 )
				{
					uint8_t * _rgb888_buf = new uint8_t[ fb->width * fb->height * 3 ];
					if( fmt2rgb888((const uint8_t *) fb->buf, fb->len, fb->format, _rgb888_buf ))
					{
						TImageBuffer AImage( TImageFormat::RGB24, fb->width, fb->height, _rgb888_buf );
						T_OutputPin::SetPinValue( AImage );
					}

					delete [] _rgb888_buf;
				}

				else
*/
				{
					TImageFormat::TImageFormat	APixelFormat;
					switch( fb->format )
					{
						case PIXFORMAT_RGB565 : APixelFormat = TImageFormat::RGB565; break;
						case PIXFORMAT_RGB555 : APixelFormat = TImageFormat::RGB555; break;
						case PIXFORMAT_GRAYSCALE : APixelFormat = TImageFormat::GrayScale; break;
						case PIXFORMAT_JPEG : APixelFormat = TImageFormat::JPEG; break;
					}

					TImageBuffer AImage( APixelFormat, fb->width, fb->height, fb->buf, fb->len );
					T_OutputPin::SetPinValue( AImage );
				}


/*
				if( fb->format == PIXFORMAT_JPEG )
				{
					if( T_JPEGOutputPin::GetPinIsConnected() )
					{
						TJPEGImageBuffer AImage( fb->width, fb->height, fb->buf, fb->len );
						T_JPEGOutputPin::SetPinValue( AImage );
					}

					if( T_OutputPin::GetPinIsConnected() )
					{
						uint8_t * _rgb888_buf = new uint8_t[ fb->width * fb->height * 3 ];
						if( fmt2rgb888((const uint8_t *) fb->buf, fb->len, fb->format, _rgb888_buf ))
						{
							TImageBuffer AImage( TImageFormat::RGB24, fb->width, fb->height, _rgb888_buf );
							T_OutputPin::SetPinValue( AImage );
						}

						delete [] _rgb888_buf;
					}
				}

				else
				{
					if( T_OutputPin::GetPinIsConnected() )
					{
						if( fb->format == PIXFORMAT_RGB888 )
						{
							uint8_t * _rgb888_buf = new uint8_t[ fb->width * fb->height * 3 ];
							if( fmt2rgb888((const uint8_t *) fb->buf, fb->len, fb->format, _rgb888_buf ))
							{
								TImageBuffer AImage( TImageFormat::RGB24, fb->width, fb->height, _rgb888_buf );
								T_OutputPin::SetPinValue( AImage );
							}

							delete [] _rgb888_buf;
						}

						else
						{
							TImageFormat::TImageFormat	APixelFormat;
							switch( fb->format )
							{
								case PIXFORMAT_RGB565 : APixelFormat = TImageFormat::RGB565; break;
								case PIXFORMAT_RGB555 : APixelFormat = TImageFormat::RGB555; break;
								case PIXFORMAT_GRAYSCALE : APixelFormat = TImageFormat::GrayScale; break;
							}

							TImageBuffer AImage( APixelFormat, fb->width, fb->height, fb->buf );
							T_OutputPin::SetPinValue( AImage );
						}
					}

					if( T_JPEGOutputPin::GetPinIsConnected() )
					{
						size_t _jpg_buf_len;
						uint8_t * _jpg_buf;
						if( frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len ))
						{
							TJPEGImageBuffer AImage( fb->width, fb->height, _jpg_buf, _jpg_buf_len );
							T_JPEGOutputPin::SetPinValue( AImage );
							free( _jpg_buf );
						}
					}
				}
*/
//				auto ATest1 = esp_camera_sensor_get();
//				Serial.println( ATest1->id.PID, HEX );


				esp_camera_fb_return( fb );
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadFrame();
		}

	public:
		inline void SystemLoopBegin()
		{
			ReadFrame();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Brightness,
		int C_I2C_CHANNEL,
		int C_PIN_HREF,
  		int C_PIN_PCLK,
		int C_PIN_POWER_DOWN,
		int C_PIN_RESET,
		int C_PIN_SIOC,
		int C_PIN_SIOD,
		int C_PIN_VSYNC,
		int C_PIN_XCLK,
		int C_PIN_Y2,
		int C_PIN_Y3,
		int C_PIN_Y4,
		int C_PIN_Y5,
		int C_PIN_Y6,
		int C_PIN_Y7,
		int C_PIN_Y8,
		int C_PIN_Y9,
		typename T_ColorBar,
		typename T_Contrast,
		typename T_CountBuffers,
		typename T_EnableDownsize,
		typename T_Enabled,
		typename T_Exposure,
		typename T_Gain,
		typename T_GammaCorrection,
		typename T_GrabMode,
		typename T_ImageSize,
		typename T_JpegQuality,
		typename T_LensCorrection,
		typename T_Mirror,
		typename T_OutputPin,
		typename T_PixelCorrection,
		typename T_PixelFormat,
		typename T_Quality,
		typename T_Saturation,
		typename T_SpecialEffect,
		typename T_UsePSRAM,
		typename T_WhiteBalance
    > class ESP32Camera :
		public T_Brightness,
		public ESP32BasicCamera <
				C_I2C_CHANNEL,
				C_PIN_HREF,
  				C_PIN_PCLK,
				C_PIN_POWER_DOWN,
				C_PIN_RESET,
				C_PIN_SIOC,
				C_PIN_SIOD,
				C_PIN_VSYNC,
				C_PIN_XCLK,
				C_PIN_Y2,
				C_PIN_Y3,
				C_PIN_Y4,
				C_PIN_Y5,
				C_PIN_Y6,
				C_PIN_Y7,
				C_PIN_Y8,
				C_PIN_Y9,
				T_CountBuffers,
				T_Enabled,
				T_GrabMode,
				T_ImageSize,
				T_JpegQuality,
				T_OutputPin,
				T_PixelFormat,
				T_UsePSRAM
			>,
		public T_ColorBar,
		public T_Contrast,
		public T_EnableDownsize,
		public T_Exposure,
		public T_Gain,
		public T_GammaCorrection,
		public T_LensCorrection,
		public T_Mirror,
		public T_PixelCorrection,
		public T_Quality,
		public T_Saturation,
		public T_SpecialEffect,
		public T_WhiteBalance
	{
		typedef ESP32BasicCamera <
				C_I2C_CHANNEL,
				C_PIN_HREF,
  				C_PIN_PCLK,
				C_PIN_POWER_DOWN,
				C_PIN_RESET,
				C_PIN_SIOC,
				C_PIN_SIOD,
				C_PIN_VSYNC,
				C_PIN_XCLK,
				C_PIN_Y2,
				C_PIN_Y3,
				C_PIN_Y4,
				C_PIN_Y5,
				C_PIN_Y6,
				C_PIN_Y7,
				C_PIN_Y8,
				C_PIN_Y9,
				T_CountBuffers,
				T_Enabled,
				T_GrabMode,
				T_ImageSize,
				T_JpegQuality,
				T_OutputPin,
				T_PixelFormat,
				T_UsePSRAM
			> inherited;

	public:
		_V_PROP_( Brightness )
		_V_PROP_( ColorBar )
		_V_PROP_( Contrast )
		_V_PROP_( EnableDownsize )
		_V_PROP_( Exposure )
		_V_PROP_( Gain )
		_V_PROP_( GammaCorrection )
		_V_PROP_( ImageSize )
		_V_PROP_( LensCorrection )
		_V_PROP_( Mirror )
		_V_PROP_( PixelCorrection )
		_V_PROP_( Quality )
		_V_PROP_( Saturation )
		_V_PROP_( SpecialEffect )
		_V_PROP_( WhiteBalance )

	public:
		void Update_SpecialEffect()
		{
			auto s = esp_camera_sensor_get();
			s->set_special_effect( s, SpecialEffect() );
		}

		void Update_Brightness()
		{
			auto s = esp_camera_sensor_get();
			s->set_brightness( s, ( Brightness() * 4 - 2 ) + 0.5 );
		}

		void Update_Contrast()
		{
			auto s = esp_camera_sensor_get();
			s->set_contrast( s, ( Contrast() * 4 - 2 ) + 0.5 );
		}

		void Update_Saturation()
		{
			auto s = esp_camera_sensor_get();
			s->set_saturation( s, ( Saturation() * 4 - 2 ) + 0.5 );
		}

		void Update_ColorBar()
		{
			auto s = esp_camera_sensor_get();
			s->set_colorbar( s, ColorBar().GetValue() ? 1 : 0 );
		}

		void Update_BlackPixelCorrection()
		{
			auto s = esp_camera_sensor_get();
			s->set_bpc( s, PixelCorrection().Black().GetValue() ? 1 : 0 );
		}

		void Update_WhitePixelCorrection()
		{
			auto s = esp_camera_sensor_get();
			s->set_wpc( s, PixelCorrection().White().GetValue() ? 1 : 0 );
		}

		void Update_exposure_ctrl()
		{
			auto s = esp_camera_sensor_get();
			s->set_exposure_ctrl( s, Exposure().Enabled().GetValue() ? 1 : 0 );
		}

		void Update_ae_level()
		{
			auto s = esp_camera_sensor_get();
			s->set_ae_level( s, ( Exposure().Level() * 2 - 2 ) + 0.5 );
		}

		void Update_aec_value()
		{
			auto s = esp_camera_sensor_get();
			s->set_aec_value( s, ( Exposure().CorrectionValue() * 1200 ) + 0.5 );
		}

		void Update_aec2()
		{
			auto s = esp_camera_sensor_get();
			s->set_aec2( s, Exposure().DSP().GetValue() ? 1 : 0 );
		}

		void Update_LensCorrection()
		{
			auto s = esp_camera_sensor_get();
			s->set_lenc( s, LensCorrection().GetValue() ? 1 : 0 );
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

		void Update_gain_ctrl()
		{
			auto s = esp_camera_sensor_get();
			s->set_gain_ctrl( s, Gain().Enabled().GetValue() ? 1 : 0 );
		}

		void Update_agc_gain()
		{
			auto s = esp_camera_sensor_get();
			s->set_agc_gain( s, ( Gain().Value() * 30 ) + 0.5 );
		}

		void Update_gainceiling()
		{
			auto s = esp_camera_sensor_get();
			s->set_gainceiling( s, gainceiling_t( ( Gain().Ceiling().GetValue() * 6 ) + 0.5 ) );
		}

		void Update_WhiteBalance()
		{
			auto s = esp_camera_sensor_get();
			s->set_wb_mode( s, WhiteBalance().Value() );
		}

		void Update_awb_gain()
		{
			auto s = esp_camera_sensor_get();
			s->set_awb_gain( s, WhiteBalance().Gain() );
		}

		void Update_Whitebal()
		{
			auto s = esp_camera_sensor_get();
			s->set_whitebal( s, WhiteBalance().Enabled().GetValue() ? 1 : 0 );
		}

		void Update_Quality()
		{
			auto s = esp_camera_sensor_get();
			s->set_quality( s, Quality() * 63 + 0.5 );
		}

		void Update_EnableDownsize()
		{
			auto s = esp_camera_sensor_get();
			s->set_dcw( s, EnableDownsize().GetValue() ? 1 : 0 );
		}

		void Update_raw_gma()
		{
			auto s = esp_camera_sensor_get();
			s->set_raw_gma( s, GammaCorrection().GetValue() ? 1 : 0 );
		}

	public:
		inline void SystemStart()
		{
			inherited::UpdateEnabled();

			Update_SpecialEffect();
			Update_Brightness();
			Update_Contrast();
			Update_Saturation();
			Update_ColorBar();
			Update_BlackPixelCorrection();
			Update_WhitePixelCorrection();
			Update_exposure_ctrl();
			Update_ae_level();
			Update_aec_value();
			Update_aec2();
			Update_LensCorrection();
			Update_Mirror_Horizontal();
			Update_Mirror_Vertical();
			Update_gain_ctrl();
			Update_agc_gain();
			Update_gainceiling();
			Update_WhiteBalance();
			Update_Whitebal();
			Update_awb_gain();
			Update_Quality();
			Update_EnableDownsize();
			Update_raw_gma();

/*
auto s = esp_camera_sensor_get();
delay( 10000 );
Serial.println( s->status.agc_gain );
delay( 30000 );
*/

/*
auto s = esp_camera_sensor_get();
delay( 1000 );
Serial.println( s->status.dcw );
delay( 30000 );
*/

			inherited::ReadFrame();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif