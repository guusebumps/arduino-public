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
	static const	uint8_t CSTM32SerialInits[] =
	{
		SERIAL_7E1,
		SERIAL_8N1,
		SERIAL_7E2,
		SERIAL_8N2,
		SERIAL_7E1,
		SERIAL_8E1,
		SERIAL_7E2,
		SERIAL_8E2,
		SERIAL_7O1,
		SERIAL_8O1,
		SERIAL_7O2,
		SERIAL_8O2
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits
	> class SerialSTM32NoRemapPortImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_Parity,
		public T_Speed,
		public T_StopBits
	{
	public:
		_V_PROP_( Parity )
		_V_PROP_( Speed )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 );
		}

		void StartPort()
		{
			int AIndex = ((int)Parity() ) * 4 + ( StopBits() - 1 ) * 2 + ( DataBits() - 7 );
			C_SERIAL.begin( Speed(), CSTM32SerialInits[ AIndex ] );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits
	> class SerialSTM32NoRemapPort :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialSTM32NoRemapPortImplementation <
						T_SERIAL_TYPE, C_SERIAL,
						T_DataBits,
						T_Parity,
						T_Speed,
						T_StopBits
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
		typename T_DataBits,
		typename T_Parity,
		typename T_RXPin,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class SerialSTM32PortImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_RXPin,
		public T_Parity,
		public T_Speed,
		public T_StopBits,
		public T_TXPin
	{
	public:
		_V_PROP_( Parity )
		_V_PROP_( Speed )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )

		_V_PROP_( RXPin )
		_V_PROP_( TXPin )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 );
		}

		void StartPort()
		{
			int AIndex = ((int)Parity() ) * 4 + ( StopBits() - 1 ) * 2 + ( DataBits() - 7 );
			C_SERIAL.setRx( RXPin().GetValue() );
			C_SERIAL.setTx( TXPin().GetValue() );

			C_SERIAL.begin( Speed(), CSTM32SerialInits[ AIndex ] );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_DataBits,
		typename T_Elements_GetPeriod,
		typename T_Enabled,
		typename T_FElementIndex,
		typename T_FEndTime,
		typename T_FInBreak,
		typename T_FSending,
		typename T_OutputPin,
		typename T_Parity,
		typename T_RXPin,
		typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class SerialSTM32Port :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					SerialSTM32PortImplementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_DataBits,
							T_Parity,
							T_RXPin,
							T_Speed,
							T_StopBits,
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
