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
        typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
        typename T_TXPin
	> class BreakSerialPort : 
		public BasicSendingSerialPort <
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
