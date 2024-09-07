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
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_IMPLEMENTATION,
		uint16_t C_TRANSMIT_PIN,
		typename T_FTransmit
	> class SerialDelay_RS485_BreakImplementation :
		public T_IMPLEMENTATION,
		public T_FTransmit
	{
	public:
		_V_PROP_( FTransmit )

	protected:
		inline void UpdateSending( bool ASending )
		{
			if( ! ASending )
				C_SERIAL.flush();

			Digital.Write( C_TRANSMIT_PIN, ASending || FTransmit().GetValue() );
			T_IMPLEMENTATION::UpdateSending( ASending );
		}

		void StartPort()
		{
			pinMode( C_TRANSMIT_PIN, OUTPUT );

			Digital.Write( C_TRANSMIT_PIN, false );

			T_IMPLEMENTATION::StartPort();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		uint16_t C_TRANSMIT_PIN,
		typename T_DataBits,
        typename T_Elements_GetPeriod,
		typename T_Enabled,
		typename T_FElementIndex,
		typename T_FEndTime,
		typename T_FInBreak,
		typename T_FSending,
		typename T_FTransmit,
		typename T_OutputPin,
		typename T_Parity,
        typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class Serial_RS485_SingleControlPin_Port :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelay_RS485_BreakImplementation <
					T_SERIAL_TYPE, C_SERIAL,
					SerialDelayBreakImplementation <
						StandardSerialDelayImplementation <
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
					C_TRANSMIT_PIN
				>,
				T_AfterSendingDelay,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_SendingOutputPin
			>
	{
		typedef BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelay_RS485_BreakImplementation <
					T_SERIAL_TYPE, C_SERIAL,
					SerialDelayBreakImplementation <
						StandardSerialDelayImplementation <
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
					C_TRANSMIT_PIN
				>,
				T_AfterSendingDelay,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_SendingOutputPin
			> inherited;

	public:
		void TransmitInputPin_o_Receive( void *_Data )
		{
			inherited::FTransmit() = *(bool *)_Data;
			if( ! inherited::GetIsEnabled() )
				return;

			inherited::UpdateSending( inherited::FSending() );
		}

	};
//---------------------------------------------------------------------------
} // Mitov
