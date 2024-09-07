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
#include <Mitov_SimpleSerial.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Speed,
		typename T_TXPin
	> class SpeedOnlySerialDelayImplementation :
		public T_Speed,
		public T_TXPin
	{
	public:
		_V_PROP_( Speed )
		_V_PROP_( TXPin )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( 1 + 8 + 1 );
		}

		inline bool SerialEnd() 
		{
			C_SERIAL.end();
			return false;
		}

		void StartPort()
		{
			C_SERIAL.begin( Speed() );
		}

		inline void UpdateSending( bool ASending ) {}
		inline bool constexpr GetHasBreak() { return false; }
		inline bool StartBreak() { return false; }
		inline void EndBreak() {}

		inline size_t SendingWrite( uint8_t *AData, uint32_t ASize )
		{
			return C_SERIAL.write( AData, ASize );
		}

		inline void SystemLoopBegin( unsigned long AStartTime ) {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Speed
	> class SerialDelayNoTXPinSpeedImplementation :
			public SpeedOnlySerialDelayImplementation <
					T_SERIAL_TYPE, C_SERIAL,
					T_Speed,
                    Mitov::ConstantProperty<87, uint32_t, 200 >
                >
	{
	public:
		inline void UpdateSending( bool ASending ) {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingOutputPin,
		typename T_Speed
	> class SerialPortSpeed :
		public BasicSendingSerialPort<
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayNoTXPinSpeedImplementation <
						T_SERIAL_TYPE, C_SERIAL,
						T_Speed
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
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
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
		typename T_Speed,
        typename T_TXPin
	> class SerialPortSpeedBreak : 
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					SpeedOnlySerialDelayImplementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_Speed,
							T_TXPin
						>,
						T_Elements_GetPeriod,
						T_Enabled,
						T_FElementIndex,
						T_FInBreak,
						T_SendBreak
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
