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
#include <RTC.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!L
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduino_UNO_R4_RTCModule :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				RTC.begin();
//				RTCTime mytime(30, Month::JUNE, 2023, 13, 37, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
//				RTC.setTime(mytime);
			}

		}

	protected:
		void ReadTime( bool AChangeOnly )
		{
			if( Enabled() )
			{
				if( T_OutputPin::GetPinIsConnected() )
				{
					RTCTime ACurrenttime;
					RTC.getTime( ACurrenttime );

					TDateTime ADateTime;

//					Serial.println( ACurrenttime.getYear() );

					if( ADateTime.TryEncodeDateTime( ACurrenttime.getYear(), Month2int( ACurrenttime.getMonth() ), ACurrenttime.getDayOfMonth(), ACurrenttime.getHour(), ACurrenttime.getMinutes(), ACurrenttime.getSeconds(), 0 ))
						T_OutputPin::SetPinValue( ADateTime, AChangeOnly );

				}
			}
		}

	public:
		Mitov::TDateTime GetDateTime()
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );

			TDateTime ADateTime;

			ADateTime.TryEncodeDateTime( ACurrenttime.getYear(), Month2int( ACurrenttime.getMonth() ), ACurrenttime.getDayOfMonth(), ACurrenttime.getHour(), ACurrenttime.getMinutes(), ACurrenttime.getSeconds(), 0 );

			return ADateTime;
		}

		void SetDateTime( const Mitov::TDateTime &ADateTime )
		{
			struct tm ATimeinfo;

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;
			ADateTime.DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			RTCTime mytime( ADay, Month( AMonth - 1 ), AYear, AHour, AMinute, ASecond, DayOfWeek( AWeekDay - 1 ), SaveLight::SAVING_TIME_INACTIVE );

			RTC.setTime( mytime );
		}

		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		inline void SetSecond( uint8_t AValue )
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );
			ACurrenttime.setSecond( AValue );
			RTC.setTime( ACurrenttime );
		}

		inline void SetMinute( uint8_t AValue )
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );
			ACurrenttime.setMinute( AValue );
			RTC.setTime( ACurrenttime );
		}

		inline void SetHour( uint8_t AValue )
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );
			ACurrenttime.setHour( AValue );
			RTC.setTime( ACurrenttime );
		}

		inline void SetWeekDay( uint8_t AValue )
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );
			ACurrenttime.setDayOfWeek( DayOfWeek( AValue - 1 ) );
			RTC.setTime( ACurrenttime );
		}

		inline void SetYear( uint16_t AValue )
		{
			RTCTime ACurrenttime;
			RTC.getTime( ACurrenttime );
			ACurrenttime.setYear( AValue );
			RTC.setTime( ACurrenttime );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadTime( false );
		}

		inline void SetInputPin_o_Receive( void *_Data )
		{
			Mitov::TDateTime &ADateTime = *(Mitov::TDateTime *)_Data;
			SetDateTime( ADateTime );
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

		inline void SystemStart()
		{
			ReadTime( false );
		}

		inline void SystemLoopBegin()
		{
			ReadTime( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
