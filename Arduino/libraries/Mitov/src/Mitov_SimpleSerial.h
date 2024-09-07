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

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace NullSerial_CallChains
	{
		class GetPeriod
		{
		public:
		  inline static constexpr uint32_t Count() { return 0; }
		  static void Call( int32_t AIndex, unsigned long & APeriod ) {}

		};
	} // SoftwareSerial_CallChains
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Enabled,
		typename T_OutputPin
	> class BasicSerialWrapper :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )

	public:
		typedef Stream & T_STREAM;

	protected:
		inline bool GetIsEnabled() { return Enabled(); }

	public:
		inline Stream &GetStream()
		{
			return C_SERIAL;
		}

	public:
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
				int AData = C_SERIAL.read();
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
				T_OutputPin::SetPinValue( Mitov::TDataBlock( i, ABuffer ));

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Enabled,
		typename T_OutputPin
	> class SimpleSerial : public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>
	{
		typedef BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin> inherited;

	public:
		void ResetSerial()
		{
			C_SERIAL.end();
			StartPort();
		}

	protected:
		inline bool SerialEnd() 
		{
			C_SERIAL.end();
			return false;
		}

		inline void StartPort()
		{
			C_SERIAL.begin();
		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( inherited::Enabled() )
				C_SERIAL.println( AValue );

		}

		void PrintChar( char AValue )
		{
			if( inherited::Enabled() )
				C_SERIAL.print( AValue );

		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( inherited::Enabled() )
				return C_SERIAL.write( AData, ASize );

			return 0;
		}

	    size_t Write(uint8_t byte)
		{
			if( inherited::Enabled() )
				return C_SERIAL.write( byte );

			return 0;
		}

		inline int AvailableForWrite()
		{
			return C_SERIAL.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
		}

		inline void SystemInit()
		{
			if( inherited::Enabled() )
				StartPort();

		}

	public:
        void RestartPort() // Used by setters
		{
			if( ! inherited::Enabled() )
				return;

			C_SERIAL.end();
			StartPort();
		}

	public:
		void UpdateEnabled()
		{
			if( inherited::Enabled() )
				StartPort();

			else
				SerialEnd();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Period
	> class TArduinoSerialBreak :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Period )

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL
	> class TBasicSerialEndImplementation
	{
	public:
		inline bool SerialEnd() 
		{
			C_SERIAL.end();
			return false;
		}

		inline void StartPort()
		{
			C_SERIAL.begin( 2000000 ); // Dummy speeed!
//			C_SERIAL.begin();
		}

		inline void UpdateSending( bool ASending ) {}
		inline bool constexpr GetHasBreak() { return false; }
		inline bool StartBreak() { return false; }
		inline void EndBreak() {}
		inline void SystemLoopBegin( unsigned long AStartTime ) {}

		inline size_t SendingWrite( uint8_t *AData, uint32_t ASize )
		{
			return C_SERIAL.write( AData, ASize );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
        uint32_t C_SPEED
	> class USBSerialDelayImplementation : 
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>
	{
	public:
		static inline constexpr uint32_t GetByteSpeed()
		{
			return C_SPEED / ( 1 + 8 + 1 );  // 115200
		}

		inline void StartPort()
		{
			C_SERIAL.begin( C_SPEED ); // Dummy speeed!
//			C_SERIAL.begin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		uint32_t C_INDEX,
		typename T_Period
	> class TArduinoSerialTimedBreakElement :
		public T_Enabled,
		public T_Period
	{
	public:
		_V_PROP_( Enabled )
        _V_PROP_( Period )

	public:
		inline void GetPeriod( unsigned long & APeriod )
		{
			APeriod = Period();
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.FSending().GetValue() )
				C_OWNER.StartTimedBreakElement( C_INDEX );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_IMPLEMENTATION,
		typename T_ConnectedOutputPin,
		typename T_WaitForConnection
	> class ConnectionUSBSerialPortImplementation : 
		public T_IMPLEMENTATION,
		public T_ConnectedOutputPin,
		public T_WaitForConnection
	{
	public:
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( WaitForConnection )

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::SystemInit();

			if( WaitForConnection() )
				while( ! C_SERIAL )
					;

		}

		inline void SystemLoopBegin()
		{
			if( T_ConnectedOutputPin::GetPinIsConnected() )
				T_ConnectedOutputPin::SetPinValue( bool( C_SERIAL ));

			T_IMPLEMENTATION::SystemLoopBegin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION,
		typename T_Elements_GetPeriod,
		typename T_Enabled,
		typename T_FElementIndex,
		typename T_FInBreak,
        typename T_SendBreak
	> class SerialDelayBreakImplementation :
		public T_IMPLEMENTATION,
		public T_Enabled,
		public T_FElementIndex,
		public T_FInBreak,
        public T_SendBreak
	{
	public:
		_V_PROP_( Enabled )
        _V_PROP_( SendBreak )

	protected:
		_V_PROP_( FElementIndex )
		_V_PROP_( FInBreak )

	protected:
		inline bool SerialEnd() 
		{
			T_IMPLEMENTATION::SerialEnd();
            if( SendBreak().Enabled().GetValue() )
            {
                pinMode( T_IMPLEMENTATION::TXPin(), OUTPUT );
				Digital.Write( T_IMPLEMENTATION::TXPin(), false );
                FInBreak() = true;
				return SendBreak().Period().Enabled();
            }            

			return false;
        }

		inline bool GetHasBreak() { return SendBreak().Enabled().GetValue(); }
		inline bool StartBreak() 
		{
			if( ! SendBreak().Enabled().GetValue() )
				return false;

            FInBreak() = true;

            T_IMPLEMENTATION::SerialEnd();
			pinMode( T_IMPLEMENTATION::TXPin(), OUTPUT );
			Digital.Write( T_IMPLEMENTATION::TXPin(), false );

			return SendBreak().Period().Enabled();
		}

		inline void EndBreak()
		{
			if( ! FInBreak().GetValue() )
                return;

            FInBreak() = false;
			T_IMPLEMENTATION::StartPort();
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
					T_IMPLEMENTATION::StartPort();

				else
					Digital.Write( T_IMPLEMENTATION::TXPin(), true );

				FElementIndex() = 0;
	            FInBreak() = false;
			}
		}

	public:
		inline void StartTimedBreakElement( uint32_t AIndex )
		{
			FElementIndex() = AIndex + 1;
		}

	public:
		inline SerialDelayBreakImplementation()
		{
			FElementIndex() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DELAY_IMPLEMENTATION,
		typename T_AfterSendingDelay,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingOutputPin
	> class BasicSendingSerialPort : 
        public BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin>,
		public T_DELAY_IMPLEMENTATION,
		public T_AfterSendingDelay,
		public T_FEndTime,
		public T_FSending,
		public T_SendingOutputPin
	{
		typedef BasicSerialWrapper<T_SERIAL_TYPE, C_SERIAL, T_Enabled, T_OutputPin> inherited;

	public:
		_V_PIN_( SendingOutputPin )

	public:
		_V_PROP_( AfterSendingDelay )

	public:
		_V_PROP_( FSending )

	protected:
		_V_PROP_( FEndTime )

	public:
		inline void UpdateBreak() // Use by Visual Bindings!
		{
//			T_DELAY_IMPLEMENTATION::UpdateBreak();
			if( ! inherited::Enabled() )
				if( T_DELAY_IMPLEMENTATION::StartBreak())
					FEndTime() = micros();

		}

	protected:
		inline void UpdateSending( bool ASending )
		{
			if( ASending )
				T_DELAY_IMPLEMENTATION::EndBreak();

			T_DELAY_IMPLEMENTATION::UpdateSending( ASending );
			T_SendingOutputPin::SetPinValue( ASending );
		}

		inline void SendingFlush()
		{
			if( ( ! T_FSending::GetIsConstant() ) || T_DELAY_IMPLEMENTATION::GetHasBreak() )
			{
				C_SERIAL.flush();
				if( T_DELAY_IMPLEMENTATION::StartBreak())
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
				FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );
//				Serial.println( 10000000 / GetByteSpeed() );
//				Serial.println( int32_t( FEndTime - ATime ));
			}

			else
			{
				UpdateSending( FSending() );
				uint32_t ATime = micros();
				if( uint32_t( FEndTime().GetValue() - ATime ) >= AfterSendingDelay() )
					FEndTime() = FEndTime().GetValue() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );

				else
					FEndTime() = ATime + AfterSendingDelay() + 1 * ( 10000000 / T_DELAY_IMPLEMENTATION::GetByteSpeed() );

//					Serial.println( 10000000 / GetByteSpeed() );
//					Serial.println( int32_t( FEndTime - ATime ));
			}

//			Serial.println( 10000000 / GetByteSpeed() );
//			Serial.println( int32_t( FEndTime - ATime ));
		}

	public:
		template <typename T> void Print( T AValue )
		{
			if( ! inherited::Enabled().GetValue() )
				return;

			SendingCount();
			C_SERIAL.println( AValue );
			SendingFlush();
		}

		void PrintChar( char AValue )
		{
			if( ! inherited::Enabled().GetValue() )
				return;

			SendingCount();
			C_SERIAL.print( AValue );
			SendingFlush();
		}

		size_t Write( uint8_t *AData, uint32_t ASize )
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

			SendingCount();
			size_t AResult = T_DELAY_IMPLEMENTATION::SendingWrite( AData, ASize );
//			size_t AResult = C_SERIAL.write( AData, ASize );
			SendingFlush();
			return AResult;
		}

	    size_t Write(uint8_t byte)
		{
			if( ! inherited::Enabled().GetValue() )
				return 0;

			SendingCount();
			size_t AResult = C_SERIAL.write( byte );
			SendingFlush();
			return AResult;
		}

		inline int AvailableForWrite()
		{
			return C_SERIAL.availableForWrite();
		}

	public:
		inline void SystemLoopBegin()
		{
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

			T_DELAY_IMPLEMENTATION::SystemLoopBegin( FEndTime().GetValue() );
		}

	public:
		inline void SystemInit()
		{
			if( inherited::Enabled() )
				T_DELAY_IMPLEMENTATION::StartPort();

		}

	public:
        void RestartPort() // Used by setters
		{
			if( ! inherited::Enabled() )
				return;

			C_SERIAL.end();
			T_DELAY_IMPLEMENTATION::StartPort();
		}

	public:
		void UpdateEnabled()
		{
			if( inherited::Enabled() )
				inherited::StartPort();

			else if( T_DELAY_IMPLEMENTATION::SerialEnd())
				FEndTime() = micros();

		}
/*
	public:
		inline SerialPort()
		{
			FSending() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingOutputPin,
		typename T_WaitForConnection
	> class USBSerialPort :  // ESP32_USBSerialPort
		public ConnectionUSBSerialPortImplementation <
			T_SERIAL_TYPE, C_SERIAL,
			BasicSendingSerialPort <
					T_SERIAL_TYPE, C_SERIAL,
					USBSerialDelayImplementation <
							T_SERIAL_TYPE, C_SERIAL,
							115200
						>,
					T_AfterSendingDelay,
					T_Enabled,
					T_FEndTime,
					T_FSending,
					T_OutputPin,
					T_SendingOutputPin
				>,
                T_ConnectedOutputPin,
                T_WaitForConnection
			>
	{
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingOutputPin
	> class SimpleUSBSerialPort :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				USBSerialDelayImplementation <
						T_SERIAL_TYPE, C_SERIAL,
						115200
					>,
				T_AfterSendingDelay,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_SendingOutputPin
			>
	{
	};	
//---------------------------------------------------------------------------
} // Mitov
