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
//#if ! defined( VISUINO_STMDUINO )
	static const	UARTClass::UARTModes CUARTSerialInits[] = 
	{
		SERIAL_8N1,
		SERIAL_8E1,
		SERIAL_8O1,
		SERIAL_8M1,
		SERIAL_8S1
	};
//#endif
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_Enabled,
		typename T_Parity,
		typename T_Speed
	> class UARTBreakImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_Enabled,
		public T_Parity,
		public T_Speed
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Speed )
		_V_PROP_( Parity )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / 11; //( StopBits().GetValue() + DataBits().GetValue() + 1 ); 
		}

		inline bool SerialEnd() 
		{
			C_SERIAL.end();
			return false;
		}

		void StartPort()
		{
			C_SERIAL.begin( Speed(), CUARTSerialInits[ int( Parity().GetValue()) ] );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_Parity,
		typename T_SendingOutputPin,
		typename T_Speed
	> class UARTSerialPort :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				UARTBreakImplementation <
						T_SERIAL_TYPE, C_SERIAL,
						T_Enabled,
						T_Parity,
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
} // Mitov
