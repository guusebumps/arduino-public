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
	static const uint32_t CUSARTSerialDataBits[] =
	{
		US_MR_CHRL_5_BIT,
		US_MR_CHRL_6_BIT,
		US_MR_CHRL_7_BIT,
		US_MR_CHRL_8_BIT
	};
//---------------------------------------------------------------------------
	static const uint32_t CUSARTSerialParity[] =
	{
		US_MR_PAR_NO,
		US_MR_PAR_EVEN,
		US_MR_PAR_ODD,
		US_MR_PAR_MARK,
		US_MR_PAR_SPACE
	};
//---------------------------------------------------------------------------
	static const uint32_t CUSARTSerialStopBits[] =
	{
		US_MR_NBSTOP_1_BIT,
		US_MR_NBSTOP_2_BIT
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits
	> class USARTImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_Parity,
		public T_Speed,
		public T_StopBits
	{
	public:
		_V_PROP_( Speed )
		_V_PROP_( Parity )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )

	protected:
		inline uint32_t GetByteSpeed()
		{
			return Speed().GetValue() / ( StopBits().GetValue() + DataBits().GetValue() + 1 );
		}

		void StartPort()
		{
//			int AIndex = ((int)Parity().GetValue() ) * 8 + ( StopBits().GetValue() - 1 ) * 4 + ( DataBits().GetValue() - 5 );
//			C_SERIAL.begin( Speed(), CUSARTSerialInits[ AIndex ] );
			uint8_t ABits = DataBits().GetValue() - 5;
			if( StopBits().GetValue() == 1 )
				if( ABits == 3 )
					ABits = 2;

			C_SERIAL.begin( Speed(), USARTClass::USARTModes( CUSARTSerialDataBits[ ABits ] | CUSARTSerialParity[ (int)Parity().GetValue() ] | CUSARTSerialStopBits[ StopBits().GetValue() - 1 ] ));
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits,
        typename T_TXPin
	> class USARTBreakImplementation :
		public USARTImplementation<
				T_SERIAL_TYPE, C_SERIAL,
				T_DataBits,
				T_Parity,
				T_Speed,
				T_StopBits
			>
	{
	public:
        _V_PROP_( TXPin )

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
        typename T_SendBreak,
		typename T_SendingOutputPin,
		typename T_Speed,
		typename T_StopBits,
        typename T_TXPin
	> class USARTSerialPort :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				SerialDelayBreakImplementation <
					USARTBreakImplementation <
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
	> class NoBreakUSARTSerialPort :
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				USARTImplementation <
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
} // Mitov
