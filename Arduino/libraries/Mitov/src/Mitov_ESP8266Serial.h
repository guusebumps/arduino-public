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
#include <Mitov_StandardSerial.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_RXPin,
		typename T_ReceiveEnabled,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin,
		typename T_TransmitEnabled
	> class SerialESP8266Implementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_Parity,
		public T_RXPin,
		public T_ReceiveEnabled,
		public T_Speed,
		public T_StopBits,
		public T_TXPin,
		public T_TransmitEnabled
	{
	public:
		_V_PROP_( DataBits )
		_V_PROP_( Parity )
		_V_PROP_( Speed )
		_V_PROP_( StopBits )

		_V_PROP_( ReceiveEnabled )
		_V_PROP_( TransmitEnabled )

		_V_PROP_( RXPin )
		_V_PROP_( TXPin )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 );
		}

		void StartPort()
		{
			C_SERIAL.pins( TXPin(), RXPin() );

			int AIndex = ((int)Parity()) * 8 + ( StopBits() - 1 ) * 4 + ( DataBits() - 5);

			if( ReceiveEnabled().GetValue() && TransmitEnabled().GetValue() )
				C_SERIAL.begin( Speed(), CSerialInits[ AIndex ], SERIAL_FULL );

			else if( ReceiveEnabled() )
				C_SERIAL.begin( Speed(), CSerialInits[ AIndex ], SERIAL_RX_ONLY );

			else if( TransmitEnabled() )
				C_SERIAL.begin( Speed(), CSerialInits[ AIndex ], SERIAL_TX_ONLY );

//			if( UseAlternativePins() )
//				C_SERIAL.swap();

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
		typename T_ReceiveEnabled,
		typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin,
		typename T_TransmitEnabled
	> class SerialESP8266Port :
		public BasicSendingSerialPort<
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					SerialESP8266Implementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_DataBits,
							T_Parity,
							T_RXPin,
							T_ReceiveEnabled,
							T_Speed,
							T_StopBits,
							T_TXPin,
							T_TransmitEnabled
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
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class SerialESP8266PortSendOnlyImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_Parity,
		public T_Speed,
		public T_StopBits,
		public T_TXPin
	{
	public:
		_V_PROP_( DataBits )
		_V_PROP_( Parity )
		_V_PROP_( Speed )
		_V_PROP_( StopBits )

		_V_PROP_( TXPin )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 );
		}

		void StartPort()
		{
			int AIndex = ((int)Parity()) * 8 + ( StopBits() - 1 ) * 4 + ( DataBits() - 5);
			C_SERIAL.begin( Speed(), CSerialInits[ AIndex ] );
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
		typename T_Parity,
		typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class SerialESP8266PortSendOnly :
		public BasicSendingSerialPort<
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					SerialESP8266PortSendOnlyImplementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_DataBits,
							T_Parity,
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
				Mitov::GenericPin_NoImplementation<121>,
				T_SendingOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
} // Mitov
