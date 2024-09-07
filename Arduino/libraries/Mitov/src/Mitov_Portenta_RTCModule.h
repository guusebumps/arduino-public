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
#include <mbed_rtc_time.h>
#include <mbed_mktime.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!L
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduino_Portenta_RTCModule :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadTime( bool AChangeOnly )
		{
			if( Enabled() )
			{
				if( T_OutputPin::GetPinIsConnected() )
				{
					time_t ANow = time( nullptr );
					struct tm ATimeinfo;
					gmtime_r( &ANow, &ATimeinfo );

					TDateTime ADateTime;

					if( ADateTime.TryEncodeDateTime( ATimeinfo.tm_year + 1900, ATimeinfo.tm_mon + 1, ATimeinfo.tm_mday, ATimeinfo.tm_hour, ATimeinfo.tm_min, ATimeinfo.tm_sec, 0 ))
						T_OutputPin::SetPinValue( ADateTime, AChangeOnly );

				}
			}
		}

	public:
		Mitov::TDateTime GetDateTime()
		{
			time_t ANow = time( nullptr );
			struct tm ATimeinfo;
			gmtime_r( &ANow, &ATimeinfo );

			TDateTime ADateTime;

			ADateTime.TryEncodeDateTime( ATimeinfo.tm_year + 1900, ATimeinfo.tm_mon + 1, ATimeinfo.tm_mday, ATimeinfo.tm_hour, ATimeinfo.tm_min, ATimeinfo.tm_sec, 0 );

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

			ATimeinfo.tm_year = AYear - 1900;
			ATimeinfo.tm_mon = AMonth - 1;
			ATimeinfo.tm_mday = ADay;
			ATimeinfo.tm_hour = AHour;
			ATimeinfo.tm_min = AMinute;
			ATimeinfo.tm_sec = ASecond;

			time_t ATime = mktime( &ATimeinfo );
			::set_time( ATime );
		}

		inline void SetMilliSecond( uint8_t AValue )
		{
		}

		inline void SetSecond( uint8_t AValue )
		{
			time_t ANow = time( nullptr );
			struct tm ATimeinfo;
			gmtime_r( &ANow, &ATimeinfo );

			ATimeinfo.tm_sec = AValue; 

			time_t ATime = mktime( &ATimeinfo );
			set_time( ATime );
		}

		inline void SetMinute( uint8_t AValue )
		{
			time_t ANow = time( nullptr );
			struct tm ATimeinfo;
			gmtime_r( &ANow, &ATimeinfo );

			ATimeinfo.tm_min = AValue; 

			time_t ATime = mktime( &ATimeinfo );
			set_time( ATime );
		}

		inline void SetHour( uint8_t AValue )
		{
			time_t ANow = time( nullptr );
			struct tm ATimeinfo;
			gmtime_r( &ANow, &ATimeinfo );

			ATimeinfo.tm_hour = AValue; 

			time_t ATime = mktime( &ATimeinfo );
			set_time( ATime );
		}

		inline void SetWeekDay( uint8_t AValue )
		{
		}

		inline void SetYear( uint16_t AValue )
		{
			time_t ANow = time( nullptr );
			struct tm ATimeinfo;
			gmtime_r( &ANow, &ATimeinfo );

			ATimeinfo.tm_year = AValue - 1900; 

			time_t ATime = mktime( &ATimeinfo );
			set_time( ATime );
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
