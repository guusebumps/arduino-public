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
#include <Esp.h>
#include <driver/rtc_io.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace ESP32Const
	{
		void TouchPinCallback()
		{
			//placeholder callback function
		}	
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class ESP32ProcessorReset :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				ESP.restart();

		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		typename T_Enabled,
		typename T_Sensitivity
	> class TArduinoESP32ProcessorWakeUpByTouchOperationPin :
		public T_Enabled,
		public T_Sensitivity
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Sensitivity )

	public:
		inline void UpdateRegister( bool &AEnabled )
		{
			if( ! Enabled().GetValue() )
				return;

			AEnabled |= true;
			touchAttachInterrupt( C_PIN, ESP32Const::TouchPinCallback, ( 1.0 - Sensitivity().GetValue() ) * 62 + 2.5 );
		}

	};
//---------------------------------------------------------------------------
#ifndef CONFIG_IDF_TARGET_ESP32C3
	template <
		typename T_Enabled,
		typename T_Pins_UpdateRegister
	> class TArduinoESP32ProcessorWakeUpByTouchOperation :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void UpdateRegister( bool &AEnabled )
		{
			if( ! Enabled().GetValue() )
				return;

			bool ASubEnabled = false;
			T_Pins_UpdateRegister::Call( ASubEnabled );
			if( ! ASubEnabled )
				return;

			AEnabled |= true;
			esp_sleep_enable_touchpad_wakeup();
		}

		inline void Init( bool ADeepSleep )
		{
//			if( Enabled() )
//				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TOUCHPAD );

		}

	};
//---------------------------------------------------------------------------
	namespace TArduinoESP32ProcessorWakeUpByPinOperationPullMode
	{
		enum TArduinoESP32ProcessorWakeUpByPinOperationPullMode
		{
			None,
			PullUp,
			PullDown
		};
	}
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Mode,
		typename T_Pin,
		typename T_Value
	> class TArduinoESP32ProcessorWakeUpByPinOperation :
		public T_Enabled,
		public T_Mode,
		public T_Pin,
		public T_Value
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Mode )
		_V_PROP_( Pin )
		_V_PROP_( Value )

	public:
		inline void UpdateRegister( bool &AEnabled )
		{
			if( ! Enabled().GetValue() )
				return;

			AEnabled |= true;
			esp_sleep_enable_ext0_wakeup( gpio_num_t( Pin().GetValue() ), ( Value() ) ? 1 : 0 );

			switch( Mode() )
			{
				case TArduinoESP32ProcessorWakeUpByPinOperationPullMode::PullUp:	rtc_gpio_pullup_en( gpio_num_t( Pin().GetValue() ) ); break;
				case TArduinoESP32ProcessorWakeUpByPinOperationPullMode::PullDown:	rtc_gpio_pulldown_en( gpio_num_t( Pin().GetValue() ) ); break;
				default: ;// Nothing!
			}
		}

	public:
		inline void Init( bool ADeepSleep )
		{
			if( ! Enabled().GetValue() )
				return;

//			esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_EXT0 );

			if( ! ADeepSleep )
				rtc_gpio_hold_dis( gpio_num_t( Pin().GetValue() ));

			rtc_gpio_deinit( gpio_num_t( Pin().GetValue() ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		uint8_t C_INDEX,
		typename T_Mode
	> class TArduinoESP32ProcessorWakeUpByPinsOperationPin :
		public T_Enabled,
		public T_Mode
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Mode )

	public:
		inline void UpdateRegister( bool &AEnabled )
		{
			if( ! Enabled().GetValue() )
				return;

			AEnabled |= true;
			switch( Mode() )
			{
				case TArduinoESP32ProcessorWakeUpByPinOperationPullMode::PullUp:	rtc_gpio_pullup_en( gpio_num_t( C_INDEX ) ); break;
				case TArduinoESP32ProcessorWakeUpByPinOperationPullMode::PullDown:	rtc_gpio_pulldown_en( gpio_num_t( C_INDEX ) ); break;
			}
		}

		inline void Init( bool ADeepSleep )
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! ADeepSleep )
				rtc_gpio_hold_dis( gpio_num_t( C_INDEX ));

			rtc_gpio_deinit( gpio_num_t( C_INDEX ));
		}

		inline void GetMask( uint64_t &AValue )
		{
			if( Enabled() )
				AValue |= ( uint64_t( 1 ) << C_INDEX );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period,
		typename T_Wakeup_Init,
		typename T_Wakeup_UpdateRegister
//		typename T_Wakeup_UpdateUnRegister
	> class ESP32ProcessorDeepSleep :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			bool AEnabled = false;
			T_Wakeup_UpdateRegister::Call( AEnabled );

//			ESP.deepSleep( Period() );
			if( Period().GetValue() > 0 )
			{
				esp_sleep_enable_timer_wakeup( Period().GetValue() );
				esp_deep_sleep( Period().GetValue() );
			}

			else if( AEnabled )
				esp_deep_sleep_start();
/*  
			T_Wakeup_UpdateUnRegister::Call();

			if( Period().GetValue() > 0 )
				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );
*/
		}

	public:
		inline void SystemInit()
		{
			if( ! Enabled().GetValue() )
				return;

//			if( Period().GetValue() > 0 )
//				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );

			T_Wakeup_Init::Call( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period,
        typename T_Wakeup_Init,
		typename T_Wakeup_UpdateRegister
//		typename T_Wakeup_UpdateUnRegister
	> class ESP32ProcessorLightSleep :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

			bool AEnabled = false;
			T_Wakeup_UpdateRegister::Call( AEnabled );

			if( Period().GetValue() > 0 )
			{
				esp_sleep_enable_timer_wakeup( Period().GetValue() );
				esp_light_sleep_start();
			}

			else if( AEnabled )
				esp_light_sleep_start();

//			else
//				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );
  

//			T_Wakeup_UpdateUnRegister::Call();
		}

	public:
		inline void SystemInit()
		{
			if( ! Enabled().GetValue() )
				return;

//			esp_light_sleep_start();
			T_Wakeup_Init::Call( false );
		}

	};
//---------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period
//		typename T_Wakeup_Init,
//		typename T_Wakeup_UpdateRegister
//		typename T_Wakeup_UpdateUnRegister
	> class ESP32ProcessorDeepSleep :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

//			bool AEnabled = false;
//			T_Wakeup_UpdateRegister::Call( AEnabled );

//			ESP.deepSleep( Period() );
			if( Period().GetValue() > 0 )
			{
				esp_sleep_enable_timer_wakeup( Period().GetValue() );
				esp_deep_sleep( Period().GetValue() );
			}

			else // if( AEnabled )
				esp_deep_sleep_start();
/*  
			T_Wakeup_UpdateUnRegister::Call();

			if( Period().GetValue() > 0 )
				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );
*/
		}

	public:
		inline void SystemInit()
		{
/*
			if( ! Enabled().GetValue() )
				return;

//			if( Period().GetValue() > 0 )
//				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );

			T_Wakeup_Init::Call( true );
*/
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period
//        typename T_Wakeup_Init,
//		typename T_Wakeup_UpdateRegister
//		typename T_Wakeup_UpdateUnRegister
	> class ESP32ProcessorLightSleep :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled().GetValue() )
				return;

//			bool AEnabled = false;
//			T_Wakeup_UpdateRegister::Call( AEnabled );

			if( Period().GetValue() > 0 )
			{
				esp_sleep_enable_timer_wakeup( Period().GetValue() );
				esp_light_sleep_start();
			}

			else // if( AEnabled )
				esp_light_sleep_start();

//			else
//				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_TIMER );
  

//			T_Wakeup_UpdateUnRegister::Call();
		}

	public:
		inline void SystemInit()
		{
/*
			if( ! Enabled().GetValue() )
				return;

//			esp_light_sleep_start();
			T_Wakeup_Init::Call( false );
*/
		}

	};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class ESP32GetArduinoAvailableRAM : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue( false );
		}

	protected:
		inline void ReadValue( bool AChangeOnly )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( ESP.getFreeHeap(), AChangeOnly );

		}

	public:
		inline void SystemStart()
		{
			ReadValue( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValue( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AnyHigh,
		typename T_Enabled,
		typename T_Pins_GetMask,
		typename T_Pins_Init,
		typename T_Pins_UpdateRegister
//		typename T_Pins_UpdateUnRegister
	> class TArduinoESP32ProcessorWakeUpByPinsOperation :
		public T_AnyHigh,
		public T_Enabled
	{
	public:
		_V_PROP_( AnyHigh )
		_V_PROP_( Enabled )

	public:
		inline void UpdateRegister( bool &AEnabled )
		{
			if( ! Enabled().GetValue() )
				return;

            bool ASubEnabled = false;
			T_Pins_UpdateRegister::Call( ASubEnabled );
            
            if( ! ASubEnabled )
				return;

			AEnabled |= true;
			uint64_t AMask = 0;
			T_Pins_GetMask::Call( AMask );

			esp_sleep_enable_ext1_wakeup( AMask, 
				( AnyHigh() ) ? ESP_EXT1_WAKEUP_ANY_HIGH :
#ifdef CONFIG_IDF_TARGET_ESP32
				ESP_EXT1_WAKEUP_ALL_LOW
#else // CONFIG_IDF_TARGET_ESP32
				ESP_EXT1_WAKEUP_ANY_LOW
#endif // CONFIG_IDF_TARGET_ESP32
			);

		}

/*
		inline void UpdateUnRegister()
		{
			if( Enabled() )
				esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_EXT1 );

		}
*/
	public:
		inline void Init( bool ADeepSleep )
		{
			if( ! Enabled().GetValue() )
				return;
//return;
//			esp_sleep_disable_wakeup_source( ESP_SLEEP_WAKEUP_EXT1 );
			T_Pins_Init::Call( ADeepSleep );

//			T_Pins_UpdateUnRegister::Call();

//			if( ! ADeepSleep )
//				rtc_gpio_hold_dis( gpio_num_t( i ));
/*
			uint64_t AMask = 0;
			T_Pins_GetMask::Call( AMask );

			for( int i = 0; i < 40; ++ i )
				if( AMask & ( 1 << i ))
				{
					rtc_gpio_hold_dis( gpio_num_t( i ));
					rtc_gpio_deinit( gpio_num_t( i ));
				}
*/
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif