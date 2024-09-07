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
#include <SoftwareSerial.h>
#include <Mitov_SimpleSerial.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template <
		int TX_PIN_NUMBER,
		int RX_PIN_NUMBER,
		typename T_AfterSendingDelay,
		typename T_Elements_GetPeriod,
		typename T_Enabled,
		typename T_FElementIndex,
		typename T_FEndTime,
		typename T_FInBreak,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed
	> class VisuinoSoftwareSerial :
		public T_AfterSendingDelay,
		public T_Enabled,
		public T_FElementIndex,
		public T_FEndTime,
		public T_FInBreak,
		public T_FSending,
		public T_OutputPin,
		public T_SendBreak,
		public T_SendingOutputPin,
		public T_Speed
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( SendingOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( AfterSendingDelay )

	public:
		_V_PROP_( Speed )
        _V_PROP_( SendBreak )

	protected:
		_V_PROP_( FSending )

	protected:
		_V_PROP_( FEndTime )

	protected:
		_V_PROP_( FElementIndex )
		_V_PROP_( FInBreak )

	protected:
		SoftwareSerial FSerial;

	public:
		typedef Stream & T_STREAM;

	public:
		inline Stream &GetStream()
		{
			return FSerial;
		}

	protected:
		inline void StartPort()
		{
			FSerial.begin( Speed() );
		}

		inline bool SerialEnd() 
		{
			FSerial.end();
			return false;
		}

		inline size_t SendingWrite( uint8_t *AData, uint32_t ASize )
		{
			return FSerial.write( AData, ASize );
		}

		inline constexpr uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( 1 + 8 + 1 );
		}
        
		inline bool GetHasBreak() { return SendBreak().Enabled().GetValue(); }
		inline bool StartBreak() 
		{
			if( ! SendBreak().Enabled().GetValue() )
				return false;

            FInBreak() = true;

//            T_IMPLEMENTATION::SerialEnd();
//			pinMode( T_IMPLEMENTATION::TXPin(), OUTPUT );
//			Digital.Write( T_IMPLEMENTATION::TXPin(), false );

            SerialEnd();
			pinMode( TX_PIN_NUMBER, OUTPUT );
			Digital.Write( TX_PIN_NUMBER, false );
            
			return SendBreak().Period().Enabled();
		}

		inline void EndBreak()
		{
			if( ! FInBreak().GetValue() )
                return;

            FInBreak() = false;
//			T_IMPLEMENTATION::StartPort();
			StartPort();
		}

		inline void SystemLoopBegin( unsigned long AStartTime )
		{ 
			if( ! FInBreak().GetValue() )
				return;

			unsigned long APeriod;
			if( FElementIndex() )
				T_Elements_GetPeriod::Call( FElementIndex().GetValue() - 1, APeriod );

			else
			{
				if( ! SendBreak().Period().Enabled().GetValue() )
					return;

				APeriod = SendBreak().Period().Value();
            }

			if( micros() - AStartTime > SendBreak().Period().Value() )
			{
				if( Enabled() )
					StartPort();
//					T_IMPLEMENTATION::StartPort();

				else
					Digital.Write( TX_PIN_NUMBER, true );

				FElementIndex() = 0;
	            FInBreak() = false;
			}
		}

	public:
/*
		inline void SystemLoopBeginOutput()
		{
			if( ! T_OutputPin::GetPinIsConnected() )
                return;
            
			if( ! Enabled() )
				return;

			uint8_t ABuffer[ 20 ];

			int i = 0;
			for( ; i < 20; ++i )
			{
				int AData = FSerial.read();
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( i, ABuffer ));

		}
*/
	public:
		inline void UpdateBreak() // Used by Visual Bindings!
		{
//			T_DELAY_IMPLEMENTATION::UpdateBreak();
//			if( ! inherited::Enabled() )
//				if( T_DELAY_IMPLEMENTATION::StartBreak())
			if( ! Enabled() )
				if( StartBreak())
					FEndTime() = micros();

		}

	protected:
		inline void UpdateSending( bool ASending )
		{
			if( ASending )
				EndBreak();
//				T_DELAY_IMPLEMENTATION::EndBreak();

//			T_PARENT::UpdateSending( ASending );
			T_SendingOutputPin::SetPinValue( ASending );
		}

		inline void SendingFlush()
		{
//			if( ( ! T_FSending::GetIsConstant() ) || T_DELAY_IMPLEMENTATION::GetHasBreak() )
			if( ( ! T_FSending::GetIsConstant() ) || GetHasBreak() )
			{
				FSerial.flush();
//				if( T_DELAY_IMPLEMENTATION::StartBreak())
				if( StartBreak())
					FEndTime() = micros();

			}
		}

		void SendingCount()
		{
			if( T_FSending::GetIsConstant() )
				return;

			if( ! FSending() )
			{
				FSending() = true;
				UpdateSending( FSending() );
				uint32_t ATime = micros();
//				FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );
				FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / GetByteSpeed() );
//				Serial.println( 10000000 / GetByteSpeed() );
//				Serial.println( int32_t( FEndTime - ATime ));
			}

			else
			{
				UpdateSending( FSending() );
				uint32_t ATime = micros();
				if( int32_t( FEndTime().GetValue() - ATime ) >= AfterSendingDelay() )
					FEndTime() = FEndTime().GetValue() + 1 * ( 10000000 / GetByteSpeed() );
//					FEndTime() = FEndTime().GetValue() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );

				else
					FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / GetByteSpeed() );
//					FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );

//					Serial.println( 10000000 / GetByteSpeed() );
//					Serial.println( int32_t( FEndTime - ATime ));
			}

//			Serial.println( 10000000 / GetByteSpeed() );
//			Serial.println( int32_t( FEndTime - ATime ));
		}

	public:
		template <typename T> void Print( T AValue )
		{
//			if( ! inherited::Enabled().GetValue() )
			if( ! Enabled().GetValue() )
				return;

			SendingCount();
			FSerial.println( AValue );
			SendingFlush();
		}

		void PrintChar( char AValue )
		{
//			if( ! inherited::Enabled().GetValue() )
			if( ! Enabled().GetValue() )
				return;

			SendingCount();
			FSerial.print( AValue );
			SendingFlush();
		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
//			if( ! inherited::Enabled().GetValue() )
			if( ! Enabled().GetValue() )
				return 0;

			SendingCount();
//			size_t AResult = T_DELAY_IMPLEMENTATION::SendingWrite( AData, ASize );
			size_t AResult = SendingWrite( AData, ASize );
//			size_t AResult = FSerial.write( AData, ASize );
			SendingFlush();
			return AResult;
		}

	    size_t Write(uint8_t byte)
		{
//			if( ! inherited::Enabled().GetValue() )
			if( ! Enabled().GetValue() )
				return 0;

			SendingCount();
			size_t AResult = FSerial.write( byte );
			SendingFlush();
			return AResult;
		}

		inline int AvailableForWrite()
		{
			return FSerial.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			if( T_OutputPin::GetPinIsConnected() )
			{
				uint8_t ABuffer[ 20 ];

				int i = 0;
				for( ; i < 20; ++i )
				{
					int AData = FSerial.read();
					if( AData < 0 )
						break;

					ABuffer[ i ] = AData;
				}

				if( i > 0 )
					T_OutputPin::SetPinValue( Mitov::TDataBlock( i, ABuffer ));
			}

			if( FSending() )
			{
				unsigned long ACurrentMicros = micros();
				if( int32_t( FEndTime().GetValue() - ACurrentMicros ) < 0 )
				{
//					Serial.print( "STOP: " );
//					Serial.println( int32_t( FEndTime - ACurrentMicros ) );
					FSending() = false;
					UpdateSending( FSending() );
				}
			}

//			T_DELAY_IMPLEMENTATION::SystemLoopBegin( FEndTime().GetValue() );
			SystemLoopBegin( FEndTime().GetValue() );
		}

		inline void SystemLoopBeginOutput() // For boards serial compatibility
		{
			SystemLoopBegin();
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartPort();

//			if( inherited::Enabled() )
//				T_DELAY_IMPLEMENTATION::StartPort();

		}

		inline void SystemStart()
		{
			T_SendingOutputPin::SetPinValue( true, false );
		}

	public:
        void RestartPort() // Used by setters
		{
//			if( ! inherited::Enabled() )
			if( ! Enabled() )
				return;

			FSerial.end();
//			T_DELAY_IMPLEMENTATION::StartPort();
			StartPort();
		}

	public:
		void UpdateEnabled()
		{
//			if( inherited::Enabled() )
//				inherited::StartPort();

			if( Enabled() )
				StartPort();

//			else if( T_DELAY_IMPLEMENTATION::SerialEnd())
			else if( SerialEnd())
				FEndTime() = micros();

		}

	public:
		inline VisuinoSoftwareSerial() :
			FSerial( RX_PIN_NUMBER, TX_PIN_NUMBER )
		{
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif