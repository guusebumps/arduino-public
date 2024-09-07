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
		typename T_RXPin,
		typename T_Speed,
		typename T_TXPin
	> class SerialSipeedK210Implementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_RXPin,
		public T_Speed,
		public T_TXPin
	{
	public:
		_V_PROP_( Speed )

		_V_PROP_( RXPin )
		_V_PROP_( TXPin )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( 1 + 8 + 1 );
		}

		void StartPort()
		{
			C_SERIAL.begin( Speed(), RXPin().GetValue(), TXPin().GetValue() );
		}

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
		typename T_RXPin,
		typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_TXPin
	> class SerialSipeedK210DefinedPinsSpeedPort :
		public BasicSendingSerialPort<
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					SerialSipeedK210Implementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_RXPin,
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
