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
#include <Mitov_Graphics.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace GC9A01A
	{
		const uint8_t SWRESET	= 0x01;   ///< Software Reset (maybe, not documented)
		const uint8_t RDDID		= 0x04;     ///< Read display identification information
		const uint8_t RDDST		= 0x09;     ///< Read Display Status
		const uint8_t SLPIN		= 0x10;     ///< Enter Sleep Mode
		const uint8_t SLPOUT	= 0x11;    ///< Sleep Out
		const uint8_t PTLON		= 0x12;     ///< Partial Mode ON
		const uint8_t NORON		= 0x13;     ///< Normal Display Mode ON
		const uint8_t INVOFF	= 0x20;    ///< Display Inversion OFF
		const uint8_t INVON		= 0x21;     ///< Display Inversion ON
		const uint8_t DISPOFF	= 0x28;   ///< Display OFF
		const uint8_t DISPON	= 0x29;    ///< Display ON
		const uint8_t CASET		= 0x2A;     ///< Column Address Set
		const uint8_t RASET		= 0x2B;     ///< Row Address Set
		const uint8_t RAMWR		= 0x2C;     ///< Memory Write
		const uint8_t PTLAR		= 0x30;     ///< Partial Area
		const uint8_t VSCRDEF	= 0x33;   ///< Vertical Scrolling Definition
		const uint8_t TEOFF		= 0x34;     ///< Tearing Effect Line OFF
		const uint8_t TEON		= 0x35;      ///< Tearing Effect Line ON
		const uint8_t MADCTL	= 0x36;    ///< Memory Access Control
		const uint8_t VSCRSADD	= 0x37;  ///< Vertical Scrolling Start Address
		const uint8_t IDLEOFF	= 0x38;   ///< Idle mode OFF
		const uint8_t IDLEON	= 0x39;    ///< Idle mode ON
		const uint8_t COLMOD	= 0x3A;    ///< Pixel Format Set
		const uint8_t CONTINUE	= 0x3C;  ///< Write Memory Continue
		const uint8_t TEARSET	= 0x44;   ///< Set Tear Scanline
		const uint8_t GETLINE	= 0x45;   ///< Get Scanline
		const uint8_t SETBRIGHT	= 0x51; ///< Write Display Brightness
		const uint8_t SETCTRL	= 0x53;   ///< Write CTRL Display
		const uint8_t POWER7	= 0xA7;   ///< Power Control 7
		const uint8_t TEWC		= 0xBA;      ///< Tearing effect width control
		const uint8_t POWER1	= 0xC1;   ///< Power Control 1
		const uint8_t POWER2	= 0xC3;   ///< Power Control 2
		const uint8_t POWER3	= 0xC4;   ///< Power Control 3
		const uint8_t POWER4	= 0xC9;   ///< Power Control 4
		const uint8_t RDID1		= 0xDA;     ///< Read ID 1
		const uint8_t RDID2		= 0xDB;     ///< Read ID 2
		const uint8_t RDID3		= 0xDC;     ///< Read ID 3
		const uint8_t FRAMERATE	= 0xE8; ///< Frame rate control
		const uint8_t SPI2DATA	= 0xE9;  ///< SPI 2DATA control
		const uint8_t INREGEN2	= 0xEF;  ///< Inter register enable 2
		const uint8_t GAMMA1	= 0xF0;    ///< Set gamma 1
		const uint8_t GAMMA2	= 0xF1;    ///< Set gamma 2
		const uint8_t GAMMA3	= 0xF2;    ///< Set gamma 3
		const uint8_t GAMMA4	= 0xF3;    ///< Set gamma 4
		const uint8_t IFACE		= 0xF6;     ///< Interface control
		const uint8_t INREGEN1	= 0xFE;  ///< Inter register enable 1

		const uint8_t MADCTL_MY		= 0x80;  ///< Bottom to top
		const uint8_t MADCTL_MX		= 0x40;  ///< Right to left
		const uint8_t MADCTL_MV		= 0x20;  ///< Reverse Mode
		const uint8_t MADCTL_ML		= 0x10;  ///< LCD refresh Bottom to top
		const uint8_t MADCTL_RGB	= 0x00; ///< Red-Green-Blue pixel order
		const uint8_t MADCTL_BGR	= 0x08; ///< Blue-Green-Red pixel order
		const uint8_t MADCTL_MH		= 0x04;  ///< LCD refresh right to left

		const uint8_t VISUINO_CMD_DELAY_150 = 0x80;
		const uint8_t VISUINO_CMD_DELAY = 0x11000000;
		const uint8_t VISUINO_CMD_END_OF_LIST = 0x00;

		const uint8_t PROGMEM InitCmd[] = 
		{
			INREGEN2, 0,
			0xEB, 1, 0x14, // ?
			INREGEN1, 0,
			INREGEN2, 0,
			0xEB, 1, 0x14, // ?
			0x84, 1, 0x40, // ?
			0x85, 1, 0xFF, // ?
			0x86, 1, 0xFF, // ?
			0x87, 1, 0xFF, // ?
			0x88, 1, 0x0A, // ?
			0x89, 1, 0x21, // ?
			0x8A, 1, 0x00, // ?
			0x8B, 1, 0x80, // ?
			0x8C, 1, 0x01, // ?
			0x8D, 1, 0x01, // ?
			0x8E, 1, 0xFF, // ?
			0x8F, 1, 0xFF, // ?
			0xB6, 2, 0x00, 0x00, // ?
			MADCTL, 1, MADCTL_MX | MADCTL_BGR,
			COLMOD, 1, 0x05,
			0x90, 4, 0x08, 0x08, 0x08, 0x08, // ?
			0xBD, 1, 0x06, // ?
			0xBC, 1, 0x00, // ?
			0xFF, 3, 0x60, 0x01, 0x04, // ?
			POWER2, 1, 0x13,
			POWER3, 1, 0x13,
			POWER4, 1, 0x22,
			0xBE, 1, 0x11, // ?
			0xE1, 2, 0x10, 0x0E, // ?
			0xDF, 3, 0x21, 0x0c, 0x02, // ?
			GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
			GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
			GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
			GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
			0xED, 2, 0x1B, 0x0B, // ?
			0xAE, 1, 0x77, // ?
			0xCD, 1, 0x63, // ?
			// Unsure what this line (from manufacturer's boilerplate code) is
			// meant to do, but users reported issues, seems to work OK without:
			//0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03, // ?
			FRAMERATE, 1, 0x34,
			0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, // ?
					0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
			0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, // ?
					0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
			0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07, // ?
			0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00, // ?
			0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98, // ?
			0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00, // ?
			0x98, 2, 0x3e, 0x07, // ?
			TEON, 0,
			INVON, 0,
			SLPOUT, VISUINO_CMD_DELAY_150, // Exit sleep
			DISPON, VISUINO_CMD_DELAY_150, // Display on
			VISUINO_CMD_END_OF_LIST        // End of list
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_INSTANCE,
		typename T_0_IMPLEMENTATION,
		typename T_BackgroundColor,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_Text
	> class TFTDisplay565 :
		public T_0_IMPLEMENTATION,
		public GraphicsImpl<
				T_INSTANCE,
				T_BackgroundColor,
				T_Text
			>,
		public T_FCursorX,
		public T_FCursorY
	{
		typedef TFTDisplay565 T_SELF;

		typedef GraphicsImpl<
				T_INSTANCE,
				T_BackgroundColor,
				T_Text
			> inherited;

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	protected:
//#if ( defined(__ILI9341_SCROLL__) && defined(__PARALLEL_COMPONENT_DISPLAY_ILI9341__))
		template <typename T_FUNC_INSTANCE> static void InternalScroll( T_FUNC_INSTANCE &AInstance, int16_t AInWidth, int16_t AInHeight, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
            // Implement if Read works!
		}

	public:
/*
		static void drawAlphaColorBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.WriteStart();
			AImplementation.setAddrWindow( x, y, x + w - 1, y + h - 1);

			for( int16_t j = 0; j < h; ++ j )
				for( int16_t i = 0; i < w; ++ i )
				{
					uint32_t ARGBColor = pgm_read_dword( bitmap++ );
					uint8_t AAlpha = ARGBColor >> 24;
					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( ARGBColor );
						AImplementation.Write16( AColor );
					}

					else if( AAlpha == 0 )
						AImplementation.Write16( 0 );

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;
						uint8_t	r = ARGBColor >> 16;
						uint8_t	g = ARGBColor >> 8;
						uint8_t	b = ARGBColor;

		//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.Write16( AColor );
					}
				}

			AImplementation.WriteEnd();
		}

		static void drawAlphaColorBitmap_565( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
					uint8_t AAlpha = pgm_read_byte( bitmap ++ );

					uint32_t r = ARedGreen & 0b11111000;
					uint32_t b = 0xFF & ( AGreenBlue << 3 );
					uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.Write16( AColor );
					}

					else if( AAlpha == 0 )
					{
						AImplementation.Write16( 0 );
					}

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.Write16( AColor );
					}
				}

			AImplementation.WriteEnd();
		}
*/
		static void drawColorBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGreen << 8 ) | ( ARed << 16 ) | ABlue );

					AImplementation.Write16( AColor );
				}

			AImplementation.WriteEnd();
		}

		static void drawColorBitmap_565( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t AColor = pgm_read_word( bitmap );
					bitmap += 2;
					AImplementation.Write16( AColor );
/*
					uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );

					AImplementation.w_data_internal( ARedGreen );
					AImplementation.w_data_internal( AGreenBlue );
*/
				}

			AImplementation.WriteEnd();
		}

		static void drawGrayscaleBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

					AImplementation.Write16( AColor );
				}

			AImplementation.WriteEnd();
		}

		static void drawAlphaGrayscaleBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint16_t *_bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			const uint8_t *bitmap = (const uint8_t *)_bitmap;

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );
					bitmap += 2;

					AImplementation.Write16( AColor );
				}

			AImplementation.WriteEnd();
		}

		static void drawBWBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawBWBitmap( AImplementation, x, y, bitmap, w, h, color, bg, size );
				return;
			}

			int16_t byteWidth = (w + 7) / 8;

//			color = Func::Color565( color );
//			bg = Func::Color565( bg );

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor;
					if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
						AColor = 0xFFFF;

					else
      					AColor = 0;

					AImplementation.Write16( AColor );
				}

			AImplementation.WriteEnd();
		}
/*
		static void drawRepeatAlphaColorBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint8_t	r = pgm_read_byte( bitmap++ );
				uint8_t	g = pgm_read_byte( bitmap++ );
				uint8_t	b = pgm_read_byte( bitmap++ );
				uint8_t AAlpha = pgm_read_byte( bitmap++ );

				uint16_t AColor;

				if( AAlpha == 0xFF )
					AColor = Func::Color565( r, g, b );

				else if( AAlpha == 0 )
					AColor = 0;

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					AColor = Func::Color565( r, g, b );
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );

			}

			AImplementation.WriteEnd();
		}

		static void drawRepeatAlphaColorBitmap_565( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
				uint8_t AAlpha = pgm_read_byte( bitmap ++ );

				uint32_t r = ARedGreen & 0b11111000;
				uint32_t b = 0xFF & ( AGreenBlue << 3 );
				uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

				uint16_t AColor;

				if( AAlpha == 0xFF )
					AColor = Func::Color565( r, g, b );

				else if( AAlpha == 0 )
					AColor = 0;

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					AColor = Func::Color565( r, g, b );
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );
			}

			AImplementation.WriteEnd();
		}
*/
		static void drawRepeatColorBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint32_t AGreen = pgm_read_byte( bitmap ++ );
				uint32_t ABlue = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( ARed << 16 ) | ( AGreen << 8 ) | ABlue );

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );

			}

			AImplementation.WriteEnd();
		}

		static void drawRepeatColorBitmap_565( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint16_t AColor = pgm_read_word( bitmap );
				bitmap += 2;
				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );
/*
				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );

				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data_internal( ARedGreen );
					AImplementation.w_data_internal( AGreenBlue );
				}
*/
			}

			AImplementation.WriteEnd();
		}

		static void drawRepeatGrayscaleBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );

			}

			AImplementation.WriteEnd();
		}

		static void drawAlphaGrayscaleRepeatBitmap( T_INSTANCE &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( AImplementation.HasPixelRead() )
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap );

				bitmap += 2;

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.Write16( AColor );

			}

			AImplementation.WriteEnd();
		}

		static void drawGrayscalePixel( T_INSTANCE &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AGray = ( color & 0xFF );
			AImplementation.drawPixel( x, y, color << 16 | ( AGray << 8 ) | AGray );
		}

		static void fillGrayscaleRect( T_INSTANCE &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.print( x ); Serial.print( " " ); Serial.print( y ); Serial.print( " " ); Serial.print( w ); Serial.print( " " ); Serial.print( h ); Serial.print( " " ); Serial.println( uint32_t( color ));

			uint16_t AGray = ( color & 0xFF );
			fillRect( AImplementation, x, y, w, h, color << 16 | ( AGray << 8 ) | AGray );
		}

//#if ( defined(__ILI9341_SCROLL__) && defined(__PARALLEL_COMPONENT_DISPLAY_ILI9341__))

		static inline void Scroll( T_INSTANCE &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( AImplementation.HasPixelRead() )
				InternalScroll<T_INSTANCE>( AImplementation, AImplementation.width(), AImplementation.height(), X, Y, color );
//				T_0_IMPLEMENTATION::template InternalScroll<T_INSTANCE>( AImplementation, AImplementation.width(), AImplementation.height(), X, Y, color );
		}

//#endif

		static void drawFastHLine( T_INSTANCE &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth ) || (y >= AHeight ))
				return;

			uint8_t AAlpha = color >> 24;
			if( AImplementation.HasPixelRead() && AAlpha < 255 )
			{
				inherited::drawFastHLine( AImplementation, x, y, w, color );
				return;
			}

			if((x+w-1) >= AWidth )
				w = AWidth - x;

			color = Func::Color565( color );

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x, y, x+w-1, y);

//			uint8_t hi = color >> 8, lo = color;
			const uint8_t ABuffer[ 2 ] =
			{
				uint8_t( color >> 8 ),
				uint8_t( color )
			};

//			Serial.println( ABuffer[ 0 ], HEX );
//			Serial.println( ABuffer[ 1 ], HEX );

			while( w -- )
				AImplementation.Write16( color );
/*
			{
			uint8_t ABuffer1[ 2 ] =
			{
				uint8_t( color >> 8 ),
				uint8_t( color )
			};
				AImplementation.WriteCount( ABuffer, 2 );
			}
*/
			AImplementation.WriteEnd();
		}

		static void drawBuffer( T_INSTANCE &AImplementation, int16_t AX, int16_t AY, int16_t AWidth, int16_t AHeight, const TImageBuffer &ABuffer, int16_t ABufferX, int16_t ABufferY )
		{
			uint32_t ABufferWidth = ABuffer.GetWidth();
			if( ABufferWidth < AWidth )
				AWidth = ABufferWidth;

			uint32_t ABufferHeight = ABuffer.GetHeight();
			if( ABufferHeight < AHeight )
				AHeight = ABufferHeight;

			if( AWidth == 0 || AHeight == 0 )
				return;

			uint16_t AScreenHeight = AImplementation.height();
			uint16_t AScreenWidth = AImplementation.width();
			if(( AX >= AScreenWidth ) || ( AY >= AScreenHeight))
				return;

			if(( AX + AWidth - 1 ) >= AScreenWidth )
				AWidth = AScreenWidth - AX;

			if(( AY + AHeight - 1 ) >= AScreenHeight )
				AHeight = AScreenHeight - AY;

			if( AX < 0 )
			{
				ABufferX -= AX;
				AWidth += AX;
				AX = 0;
			}

			if( AY < 0 )
			{
				ABufferY -= AY;
				AHeight += AY;
				AY = 0;
			}
/*
			Serial.println( AX );
			Serial.println( AWidth );
			Serial.println( AY );
			Serial.println( AHeight );
*/
			AImplementation.WriteStart();
			AImplementation.setAddrWindow( AX, AY, AX + AWidth - 1, AY + AHeight - 1 );

			int32_t AMaxHeight = MitovMin<int32_t>( AHeight + ABufferY, AScreenHeight - AY );
			int32_t AMaxWidth = MitovMin<int32_t>( AWidth + ABufferX, AScreenWidth - AX );

			if( ABuffer.GetPixelFormat() == TImageFormat::RGB565 )
			{
				uint32_t ACopyWidth = ( AMaxWidth - ABufferX );
				if( ( ! AX ) && ( ! AY ) && ( ( AMaxHeight - ABufferY ) == ABufferHeight ) && ( ACopyWidth == ABufferWidth ))
					AImplementation.Write8( ABuffer.Read(), AWidth * AHeight * 2 );

				else
				{
					uint32_t AStride = ABuffer.GetStride();
					const uint8_t *APtr = ABuffer.Read() + ABufferX * 2 + AStride * ABufferY;
					for( int y = ABufferY; y < AMaxHeight; ++ y )
					{
						AImplementation.Write8( APtr, ACopyWidth * 2 );
						APtr += AStride;
					}
				}
/*
				uint32_t AStride = ABuffer.GetStride();
				const uint8_t APtr = ABuffer.Read();
				for( int y = ABufferY; y < AMaxHeight; ++ y )
				{
					AImplementation.Write8( APtr, AMaxWidth * 2 );
					APtr += AStride;
				}
*/
			}

			else
			{
				for( int y = ABufferY; y < AMaxHeight; ++ y )
					for( int x = ABufferX; x < AMaxWidth; ++ x )
					{
						uint16_t AColor = ABuffer.GetPixel565( x, y );
						AImplementation.Write16( AColor );
	/*
						uint8_t hi = AColor >> 8;
						uint8_t lo = AColor;

						AImplementation.w_data_internal( hi );
						AImplementation.w_data_internal( lo );
	*/
	//					AImplementation.drawPixel( AX + x, AY + y, ABuffer.GetPixel( x + ABufferX, y + ABufferY ) );
					}
			}

			AImplementation.WriteEnd();
		}

		static void drawFastVLine( T_INSTANCE &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth) || (y >= AHeight))
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( AImplementation.HasPixelRead() && AAlpha < 255 )
			{
				inherited::drawFastVLine( AImplementation, x, y, h, color );
				return;
			}

			if((y+h-1) >= AHeight)
				h = AHeight-y;

			color = Func::Color565( color );

			AImplementation.WriteStart();
			AImplementation.setAddrWindow(x, y, x, y+h-1);

			while( h-- )
				AImplementation.Write16( color );

			AImplementation.WriteEnd();
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
/*
			Serial.print( "drawPixel: " );
			Serial.print( x );
			Serial.print( ":" );
			Serial.print( y );
			Serial.print( " = " );
			Serial.println( uint32_t( color ), HEX );
*/
			uint16_t AHeight = T_0_IMPLEMENTATION::height();
			uint16_t AWidth = T_0_IMPLEMENTATION::width();

			if((x < 0) ||(x >= AWidth) || (y < 0) || (y >= AHeight))
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( ! T_0_IMPLEMENTATION::HasPixelRead() )
				color |= 0xFF000000;

//			T_0_IMPLEMENTATION::drawPixel( x, y, color, AAlpha );

			if( AAlpha < 255 )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

				TUniColor AOldColor = T_0_IMPLEMENTATION::GetPixelColor( x, y );
				uint8_t	OldR = AOldColor >> 16;
				uint8_t	OldG = AOldColor >> 8;
				uint8_t	OldB = AOldColor;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + float( OldR ) * ( 1 - ACoefficient ) + 0.5;
				g = float( g ) * ACoefficient + float( OldG ) * ( 1 - ACoefficient ) + 0.5;
				b = float( b ) * ACoefficient + float( OldB ) * ( 1 - ACoefficient ) + 0.5;

				color = Func::Color565( r, g, b );
//				color = Func::Color565( 255, 255, 255 );
			}

			else
				color = Func::Color565( color );

			T_0_IMPLEMENTATION::WriteStart();
			T_0_IMPLEMENTATION::setAddrWindow(x,y,x,y);

//			cmd( GC9A01A::RAMWR );

//			w_data( color >> 8 );
//			w_data( color );


//			cmd( GC9A01A::RAMWR );
			T_0_IMPLEMENTATION::SetDataCommandLow();

			T_0_IMPLEMENTATION::Write8( GC9A01A::RAMWR );

			T_0_IMPLEMENTATION::SetDataCommandHigh();

			T_0_IMPLEMENTATION::Write16( color );
//			T_0_IMPLEMENTATION::Write8( color >> 8 );
//			T_0_IMPLEMENTATION::Write8( color );

			T_0_IMPLEMENTATION::WriteEnd();

		}

		static void fillRect( T_INSTANCE &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			// rudimentary clipping (drawChar w/big text requires this)
			int16_t AWidth = AImplementation.width();
			int16_t AHeight = AImplementation.height();

			if((x >= AWidth) || (y >= AHeight))
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( AImplementation.HasPixelRead() && AAlpha < 255 )
			{
				inherited::fillRect( AImplementation, x, y, w, h, color );
				return;
			}

			fillRectImplementation( AImplementation, AWidth, AHeight, x, y, w, h, color );
		}

		static void fillRectImplementation( T_INSTANCE &AImplementation, int16_t AWidth, int16_t AHeight, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
/*
			Serial.println( "fillRectImplementation" );
			Serial.println( AWidth );
			Serial.println( AHeight );
			Serial.println( x );
			Serial.println( y );
			Serial.println( w );
			Serial.println( h );
*/
			if((x + w - 1) >= AWidth)
				w = AWidth - x;

			if((y + h - 1) >= AHeight)
				h = AHeight - y;

			AImplementation.WriteStart();

			AImplementation.setAddrWindowInternal( x, y, x+w-1, y+h-1 );

			color = Func::Color565( color );

			uint8_t hi = color >> 8, lo = color;

			AImplementation.SetDataCommandLow();
			AImplementation.Write8( GC9A01A::RAMWR ); // Send the command byte
//			sendCommand( GC9A01A::RAMWR ); // Write to RAM

			AImplementation.SetDataCommandHigh();

			for( uint16_t i=0; i < h; ++i )
				for( uint16_t j=0; j < w; ++j )
				{
					AImplementation.Write8Test( hi );
					AImplementation.Write8Test( lo );
				}

			AImplementation.WriteEnd();
		}

	public:
		template <typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return inherited::write( AImplementation, AConfig, AValue );
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
//		template <typename T> size_t print( T AValue )
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

//		template <typename T> size_t println( T AValue )
		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

/*
	public:
		template <typename T_CONFIG> inline static size_t write( T_INSTANCE &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return inherited::write( AImplementation, AConfig, AValue );
		}

		template <typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template <typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
//		template <typename T> size_t print( T AValue )
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_INSTANCE> APrint( this, (typename GraphicsPrint<T_INSTANCE>::TCallback) &T_INSTANCE::write );
			GraphicsPrint<T_INSTANCE, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_INSTANCE, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

//		template <typename T> size_t println( T AValue )
		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_INSTANCE> APrint( this, (typename GraphicsPrint<T_INSTANCE>::TCallback) &T_INSTANCE::write );
			GraphicsPrint<T_INSTANCE, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_INSTANCE, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template <typename T> inline size_t print( T AValue )
		{
			return print<T_INSTANCE, T>( this, AValue );
		}

		template <typename T> inline size_t println( T AValue )
		{
			return println<T_INSTANCE, T>( this, AValue );
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_0_IMPLEMENTATION,
		typename T_0_ORIENTATION_IMPLEMENTATION,
		typename T_Mirror,
		typename T_Orientation
	> class DisplayGC9A01_Impl :
        public T_0_IMPLEMENTATION,
		public T_Mirror,
		public T_Orientation
	{
	public:
		_V_PROP_( Mirror )
		_V_PROP_( Orientation )

	public:
		inline constexpr int16_t width(void)
		{
			return 240; // T_0_ORIENTATION_IMPLEMENTATION::template GetWidth<T_SELF>( this );
		}

		inline constexpr int16_t height(void)
		{
			return 240; // T_0_ORIENTATION_IMPLEMENTATION::template GetHeight<T_SELF>( this );
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if( ! T_0_IMPLEMENTATION::HasPixelRead() )
				return 0x0000;

			if((x < 0) ||(x >= width()) || (y < 0) || (y >= height()))
				return 0x0000;

			return T_0_IMPLEMENTATION::GetPixelColor( x, y );
		}

	public:
		inline void setAddrWindowInternal(int x1, int y1, int x2, int y2)
		{
//			Serial.println( "setAddrWindow" );
/*
			Serial.println( x1 );
			Serial.println( y1 );
			Serial.println( x2 );
			Serial.println( y2 );
*/
			uint32_t t;

//			uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
			T_0_IMPLEMENTATION::WriteCommandData2( GC9A01A::CASET, x1, x2 );  // HX8357D uses same registers!
			T_0_IMPLEMENTATION::WriteCommandData2( GC9A01A::RASET, y1, y2 ); // HX8357D uses same registers!

			sendCommandInternal( GC9A01A::RAMWR );
			T_0_IMPLEMENTATION::SetDataCommandHigh();
		}

		inline void setAddrWindow(int x1, int y1, int x2, int y2)
		{
//			T_0_IMPLEMENTATION::WriteStart();
			setAddrWindowInternal( x1, y1, x2, y2);
//			T_0_IMPLEMENTATION::WriteEnd();
		}

	protected:
		void SendCommandSequence( const uint8_t *APointer )
		{
			T_0_IMPLEMENTATION::WriteStart();
            uint8_t cmd;
			while (( cmd = pgm_read_byte( APointer ++ )) > 0 )
			{
				uint8_t x = pgm_read_byte( APointer ++ );
				uint8_t numArgs = x & 0b00111111;
				sendCommands( cmd, APointer, numArgs );
				APointer += numArgs;
				if( x & 0b11000000 )
				{
					if( x & 0b01000000 )
					{
						uint16_t ADelay = pgm_read_byte( APointer ++ );
						if( ADelay == 255 )
							ADelay = 500;

						delay( ADelay );
					}

					else
						delay( 150 );
				}

			}

			T_0_IMPLEMENTATION::WriteEnd();
		}

        inline void sendCommandInternal( uint8_t commandByte )
		{
			T_0_IMPLEMENTATION::SetDataCommandLow();
			T_0_IMPLEMENTATION::Write8( commandByte );
		}

        inline void sendCommand( uint8_t commandByte )
        {
			T_0_IMPLEMENTATION::WriteStart();
			sendCommandInternal( commandByte );
			T_0_IMPLEMENTATION::WriteEnd();
		}

		void sendCommands( uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes )
		{
//			SPI_BEGIN_TRANSACTION();
//			SPI_CS_LOW();
//			T_0_IMPLEMENTATION::WriteStart();

//			SPI_DC_LOW();          // Command mode
			sendCommandInternal( commandByte );

			T_0_IMPLEMENTATION::SetDataCommandHigh();
//			SPI_DC_HIGH();
			for (int i = 0; i < numDataBytes; ++ i )
				T_0_IMPLEMENTATION::Write8( pgm_read_byte( dataBytes++ ));

//			SPI_CS_HIGH();
//			SPI_END_TRANSACTION();
//			T_0_IMPLEMENTATION::WriteEnd();
		}

	public:
		static inline TUniColor NonTransparent( TUniColor color )
		{
			return 0xFF000000 | ::Mitov::Func::SwapRB( color );
		}

	public:
		void UpdateOrientation()
		{
			sendCommand( GC9A01A::MADCTL );

//			T_IMPLEMENTATION::writedata( C_PIXEL_MODE );
			switch( T_0_ORIENTATION_IMPLEMENTATION::ConvertOrientation( Orientation().GetValue() ) )
			{
//				case goUp:		T_0_IMPLEMENTATION::w_data( GC9A01A::MADCTL_MX | GC9A01A::MADCTL_BGR ); break;
//				case goUp:		T_0_IMPLEMENTATION::w_data( (( Mirror().Horizontal() ) ? 0 : GC9A01A::MADCTL_MX ) | GC9A01A::MADCTL_BGR ); break;
				case goUp:		T_0_IMPLEMENTATION::w_data( (( Mirror().Horizontal() ) ? 0 : GC9A01A::MADCTL_MX ) | (( Mirror().Vertical() ) ? GC9A01A::MADCTL_MY : 0 ) | GC9A01A::MADCTL_BGR ); break;

//				case goLeft:	T_0_IMPLEMENTATION::w_data( GC9A01A::MADCTL_MV | GC9A01A::MADCTL_BGR ); break;
				case goLeft:	T_0_IMPLEMENTATION::w_data( (( Mirror().Horizontal() ) ? GC9A01A::MADCTL_MY : 0 ) | (( Mirror().Vertical() ) ? GC9A01A::MADCTL_MX : 0 ) | GC9A01A::MADCTL_MV | GC9A01A::MADCTL_BGR ); break;

//				case goDown: 	T_0_IMPLEMENTATION::w_data( GC9A01A::MADCTL_MY | GC9A01A::MADCTL_BGR ); break;
				case goDown: 	T_0_IMPLEMENTATION::w_data( (( Mirror().Horizontal() ) ? GC9A01A::MADCTL_MX : 0 ) | (( Mirror().Vertical() ) ? 0 : GC9A01A::MADCTL_MY ) | GC9A01A::MADCTL_BGR ); break;

//				case goRight: 	T_0_IMPLEMENTATION::w_data( GC9A01A::MADCTL_MX | GC9A01A::MADCTL_MY | GC9A01A::MADCTL_MV | GC9A01A::MADCTL_BGR ); break;
				case goRight: 	T_0_IMPLEMENTATION::w_data( (( Mirror().Vertical() ) ? 0 : GC9A01A::MADCTL_MX ) | (( Mirror().Horizontal() ) ? 0 : GC9A01A::MADCTL_MY ) | GC9A01A::MADCTL_MV | GC9A01A::MADCTL_BGR ); break;
			}
		}

	public:
		inline void SystemInit()
		{
//			Serial.begin( 9600 );

			T_0_IMPLEMENTATION::SetDataCommandHigh();
			T_0_IMPLEMENTATION::SetShipSeletHigh();

			T_0_IMPLEMENTATION::ExecuteReset( this, 100, 100, 200 );

//			sendCommand( GC9A01A::SWRESET ); // Engage software reset
//			delay( 150 );

			SendCommandSequence( GC9A01A::InitCmd );

//			ClearScreen( *this, NonTransparent( inherited::BackgroundColor().GetValue() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_0_IMPLEMENTATION,
		typename T_0_ORIENTATION_IMPLEMENTATION,
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_Inverted,
		typename T_Mirror,
		typename T_Orientation,
		typename T_Text
	> class DisplayGC9A01 :
		public TFTDisplay565<
				DisplayGC9A01<
					T_0_IMPLEMENTATION,
					T_0_ORIENTATION_IMPLEMENTATION,
					T_BackgroundColor,
					T_Elements_Render,
					T_FCursorX,
					T_FCursorY,
					T_Inverted,
					T_Mirror,
					T_Orientation,
					T_Text
				>,
				DisplayGC9A01_Impl <
						T_0_IMPLEMENTATION,
						T_0_ORIENTATION_IMPLEMENTATION,
                        T_Mirror,
						T_Orientation
					>,
				T_BackgroundColor,
				T_FCursorX,
				T_FCursorY,
				T_Text
			>,
/*
		public GraphicsImpl<
				DisplayGC9A01<
					T_0_IMPLEMENTATION,
					T_0_ORIENTATION_IMPLEMENTATION,
					T_BackgroundColor,
					T_Elements_Render,
					T_FCursorX,
					T_FCursorY,
					T_Orientation,
					T_Text
				>,
				T_BackgroundColor,
				T_Text
			>,
*/
		public T_Inverted,
		public T_Orientation
    {
		typedef DisplayGC9A01 T_SELF;
/*
		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> inherited;
*/
		typedef DisplayGC9A01_Impl <
						T_0_IMPLEMENTATION,
						T_0_ORIENTATION_IMPLEMENTATION,
						T_Mirror,
						T_Orientation
				> T_SELF_IMPLEMENTATION;

		typedef TFTDisplay565<
				T_SELF,
				T_SELF_IMPLEMENTATION,
				T_BackgroundColor,
				T_FCursorX,
				T_FCursorY,
				T_Text
			> inherited;

	public:
		_V_PROP_( Inverted )

	public:
		inline void UpdateInverted()
		{
			T_SELF_IMPLEMENTATION::sendCommand( ( Inverted() ) ? GC9A01A::INVOFF : GC9A01A::INVON );
		}

	public:
		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			inherited::fillRectImplementation( AImplementation, AImplementation.width(), AImplementation.height(), 0, 0, AImplementation.width(), AImplementation.height(), color );
		}

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
		inline void SystemStart()
		{
			inherited::UpdateOrientation();
			UpdateInverted();
			ClearScreen( *this, T_SELF_IMPLEMENTATION::NonTransparent( inherited::BackgroundColor().GetValue() ));

			inherited::RenderElements();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
