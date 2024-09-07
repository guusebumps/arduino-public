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
#include <Mitov_Graphics_Grayscale.h>
#include <FspTimer.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace Arduino_UNO_R4_Display_Const
	{
		const uint8_t pins[][2] = 
		{
		  { 7, 3 }, // 0
		  { 7, 0 },
		  { 4, 6 },
		  { 8, 5 },
		  { 8, 1 },
		  { 4, 2 },
		  { 7, 10 },
		  { 5, 10 },

		  { 3, 7 },
		  { 0, 7 },
		  { 6, 4 },
		  { 5, 8 },
		  { 1, 8 },
		  { 2, 4 },
		  { 10, 7 },
		  { 10, 5 },

		  { 7, 4 },
		  { 3, 0 },
		  { 8, 6 },
		  { 0, 5 },
		  { 0, 1 },
		  { 8, 2 },
		  { 3, 10 },
		  { 1, 10 },

		  { 4, 7 },
		  { 0, 3 },
		  { 6, 8 },
		  { 5, 0 },
		  { 1, 0 },
		  { 2, 8 },
		  { 10, 3 },
		  { 10, 1 },

		  { 3, 4 },
		  { 4, 0 },
		  { 0, 6 },
		  { 6, 5 },
		  { 6, 1 },
		  { 0, 2 },
		  { 4, 10 },
		  { 2, 10 },

		  { 4, 3 },
		  { 0, 4 },
		  { 6, 0 },
		  { 5, 6 },
		  { 1, 6 },
		  { 2, 0 },
		  { 10, 4 },
		  { 10, 2 },

		  { 7, 8 },
		  { 8, 0 },
		  { 7, 5 },
		  { 7, 1 },
		  { 5, 1 },
		  { 6, 2 },
		  { 8, 10 },
		  { 7, 9 },

		  { 8, 7 },
		  { 0, 8 },
		  { 5, 7 },
		  { 1, 7 },
		  { 1, 5 },
		  { 2, 6 },
		  { 10, 8 },
		  { 9, 7 },

		  { 3, 8 },
		  { 7, 6 },
		  { 3, 5 },
		  { 3, 1 },
		  { 7, 2 },
		  { 5, 2 },
		  { 0, 10 },
		  { 3, 9 },

		  { 8, 3 },
		  { 6, 7 },
		  { 5, 3 },
		  { 1, 3 },
		  { 2, 7 },
		  { 2, 5 },
		  { 10, 0 },
		  { 9, 3 },

		  { 4, 8 }, // 10
		  { 3, 6 },
		  { 4, 5 },
		  { 4, 1 },
		  { 3, 2 },
		  { 1, 2 },
		  { 6, 10 },
		  { 4, 9 },

		  { 8, 4 },
		  { 6, 3 },
		  { 5, 4 },
		  { 1, 4 },
		  { 2, 3 },
		  { 2, 1 },
		  { 10, 6 },
		  { 9, 4 }

		};

/*
		const uint8_t pins[][2] = 
		{
		  { 7, 3 }, // 0
		  { 3, 7 },
		  { 7, 4 },
		  { 4, 7 },
		  { 3, 4 },
		  { 4, 3 },
		  { 7, 8 },
		  { 8, 7 },
		  { 3, 8 },
		  { 8, 3 },
		  { 4, 8 }, // 10
		  { 8, 4 },

		  { 7, 0 },
		  { 0, 7 },
		  { 3, 0 },
		  { 0, 3 },
		  { 4, 0 },
		  { 0, 4 },
		  { 8, 0 },
		  { 0, 8 },
		  { 7, 6 }, // 20
		  { 6, 7 },
		  { 3, 6 },
		  { 6, 3 },

		  { 4, 6 },
		  { 6, 4 },
		  { 8, 6 },
		  { 6, 8 },
		  { 0, 6 },
		  { 6, 0 },
		  { 7, 5 }, // 30
		  { 5, 7 },
		  { 3, 5 },
		  { 5, 3 },
		  { 4, 5 },
		  { 5, 4 },

		  { 8, 5 },
		  { 5, 8 },
		  { 0, 5 },
		  { 5, 0 },
		  { 6, 5 }, // 40
		  { 5, 6 },
		  { 7, 1 },
		  { 1, 7 },
		  { 3, 1 },
		  { 1, 3 },
		  { 4, 1 },
		  { 1, 4 },

		  { 8, 1 },
		  { 1, 8 },
		  { 0, 1 }, // 50
		  { 1, 0 },
		  { 6, 1 },
		  { 1, 6 },
		  { 5, 1 },
		  { 1, 5 },
		  { 7, 2 },
		  { 2, 7 },
		  { 3, 2 },
		  { 2, 3 },

		  { 4, 2 },
		  { 2, 4 },
		  { 8, 2 },
		  { 2, 8 },
		  { 0, 2 },
		  { 2, 0 },
		  { 6, 2 },
		  { 2, 6 },
		  { 5, 2 },
		  { 2, 5 },
		  { 1, 2 },
		  { 2, 1 },

		  { 7, 10 },
		  { 10, 7 },
		  { 3, 10 },
		  { 10, 3 },
		  { 4, 10 },
		  { 10, 4 },
		  { 8, 10 },
		  { 10, 8 },
		  { 0, 10 },
		  { 10, 0 },
		  { 6, 10 },
		  { 10, 6 },

		  { 5, 10 },
		  { 10, 5 },
		  { 1, 10 },
		  { 10, 1 },
		  { 2, 10 },
		  { 10, 2 },
		  { 7, 9 },
		  { 9, 7 },
		  { 3, 9 },
		  { 9, 3 },
		  { 4, 9 },
		  { 9, 4 }

		};
*/
		constexpr uint8_t WIDTH = 12;
		constexpr uint8_t HEIGHT = 8;

//		constexpr float C_RefreshPeroiod = 100; // ( 1000.0 / 10000.0 );
		constexpr float C_RefreshPeroiod = ( 1.0 / 10000.0 );

		const uint32_t LED_MATRIX_PORT0_MASK =      ((1 << 3) | (1 << 4) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 15));
		const uint32_t LED_MATRIX_PORT2_MASK =      ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 12) | (1 << 13));
		constexpr int pin_zero_index = 28;

		uint8_t  __attribute__((aligned)) FBuffer[ HEIGHT * WIDTH / 8 ]; // = { 0 };
//		uint8_t  FBuffer[ HEIGHT * WIDTH ] = { 255, 255, 128, 128, 64, 64, 32, 32, 10, 10, 1, 1, 0 };

		bool FLedUpdated = false;
		uint8_t  FIndex = 0;
		uint8_t  FValue = 0;

		FspTimer FTimer;

		void InterruptHandler( timer_callback_args_t *arg )
		{
			R_PORT0->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT0_MASK);
			R_PORT2->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT2_MASK);

//			if( arg->event != TIMER_EVENT_CREST )
			if( FBuffer[ FIndex >> 3 ] & ( 1 << ( FIndex & 0b111 )))
			{
				bsp_io_port_pin_t pin_a = g_pin_cfg[ pins[ FIndex ][0] + pin_zero_index ].pin;
				R_PFS->PORT[ pin_a >> 8 ].PIN[ pin_a & 0xFF ].PmnPFS =	IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH;

				bsp_io_port_pin_t pin_c = g_pin_cfg[ pins[ FIndex ][1] + pin_zero_index ].pin;
				R_PFS->PORT[ pin_c >> 8 ].PIN[ pin_c & 0xFF ].PmnPFS =	IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW;

			}

			if( FIndex >= HEIGHT * WIDTH - 1 )
				FIndex = 0;

			else
				++ FIndex;

		}

	}
//---------------------------------------------------------------------------
	template <
		typename T_BackgroundColor,
		typename T_Elements_Render,
		typename T_Enabled,
		typename T_FCursorX,
		typename T_FCursorY,
		typename T_Orientation,
		typename T_Text
    > class TArduino_UNO_R4_Display_Module :
		public GrayscaleGraphicsImpl<
				TArduino_UNO_R4_Display_Module <
					T_BackgroundColor,
					T_Elements_Render,
					T_Enabled,
					T_FCursorX,
					T_FCursorY,
					T_Orientation,
					T_Text
				>, 
				T_BackgroundColor,
				T_Text
			>,
		public T_Enabled,
		public T_FCursorX,
		public T_FCursorY,
		public T_Orientation
	{
		typedef TArduino_UNO_R4_Display_Module T_SELF;

		typedef GrayscaleGraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Text
			> graphics;

    public:
		_V_PROP_( Orientation )
        _V_PROP_( Enabled )

	public:
		_V_PROP_( FCursorX )
		_V_PROP_( FCursorY )

	public:
		void UpdateEnabled(void)
		{
			if( Enabled() )
			{
				uint8_t type;
				uint8_t ch = FspTimer::get_available_timer( type );
//				FTimer.begin( TIMER_MODE_PERIODIC, type, ch, 40 * 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
//				FTimer.begin( TIMER_MODE_PERIODIC, type, ch, 10 * 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
//				FTimer.begin( TIMER_MODE_PERIODIC, type, ch, 10 * 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
//				Arduino_UNO_R4_Display_Const::FTimer.begin( TIMER_MODE_ONE_SHOT, type, ch, 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
//				Arduino_UNO_R4_Display_Const::FTimer.begin( TIMER_MODE_PWM, type, ch, 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
				Arduino_UNO_R4_Display_Const::FTimer.begin( TIMER_MODE_PERIODIC, type, ch, 10000.0, 50.0, Arduino_UNO_R4_Display_Const::InterruptHandler, this );
				Arduino_UNO_R4_Display_Const::FTimer.setup_overflow_irq();
				Arduino_UNO_R4_Display_Const::FTimer.open();
				Arduino_UNO_R4_Display_Const::FTimer.start();
			}

			else
			{
				Arduino_UNO_R4_Display_Const::FTimer.end();
			}
		}

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::Call();
		}

	public:
		inline int16_t width(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return Arduino_UNO_R4_Display_Const::WIDTH;

			return Arduino_UNO_R4_Display_Const::HEIGHT;
		}

		inline int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return Arduino_UNO_R4_Display_Const::HEIGHT;

			return Arduino_UNO_R4_Display_Const::WIDTH;
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			if( color == tmcNone )
				return;

			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = Arduino_UNO_R4_Display_Const::WIDTH - x - 1;
					break;

				case goDown:
					x = Arduino_UNO_R4_Display_Const::WIDTH - x - 1;
					y = Arduino_UNO_R4_Display_Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = Arduino_UNO_R4_Display_Const::HEIGHT - y - 1;
					break;

			}

			// x is which column
			switch( color )
			{
				case tmcWhite :		Arduino_UNO_R4_Display_Const::FBuffer[ x ] |=  (1 << ( y & 0b111 )); break;
				case tmcBlack :		Arduino_UNO_R4_Display_Const::FBuffer[ x ] &= ~(1 << ( y & 0b111 )); break; 
				case tmcInvert :	Arduino_UNO_R4_Display_Const::FBuffer[ x ] ^=  (1 << ( y & 0b111 )); break; 
			}

//			FModified() = true;
//			return;
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return false;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = Arduino_UNO_R4_Display_Const::WIDTH - x - 1;
					break;

				case goDown:
					x = Arduino_UNO_R4_Display_Const::WIDTH - x - 1;
					y = Arduino_UNO_R4_Display_Const::HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = Arduino_UNO_R4_Display_Const::HEIGHT - y - 1;
					break;
			}

			return (( Arduino_UNO_R4_Display_Const::FBuffer[ x ] & ( 1 << ( y & 0b111 ))) != 0 );
		}

	public:
//		template <typename T> size_t print( T AValue )
		template <typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

//		template <typename T> size_t println( T AValue )
		template <typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
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

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				UpdateEnabled();

		}

		inline void SystemStart()
		{
			graphics::ClearScreen( *this, graphics::BackgroundColor().GetValue() );
			graphics::RenderElements();
		}

		inline void SystemLoopEnd()
		{
//			auto ATest1 = Arduino_UNO_R4_Display_Const::pins[ 7 ][ 0 ];
		}

/*
	public:
		inline TArduino_UNO_R4_Display_Module()
		{
			FCounter = 0;
			FCounterOn = 0;
			FIndex = 0;
			FLedUpdated = false;
			memset( FBuffer, 0, sizeof( FBuffer ));
		}
*/
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif