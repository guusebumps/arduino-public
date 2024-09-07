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
/*
#if ! ( defined(VISUINO_CHIPKIT) || defined( VISUINO_INTEL_EDISON ) || defined( VISUINO_DIGISPARK_PRO ) || defined( VISUINO_DIGISPARK ) || defined( NO_SERIAL_FORMAT ) || defined( VISUINO_K210 ) )

#ifdef VISUINO_ESP8266
	static const	SerialConfig CSerialInits[] =
#else
	static const	long CSerialInits[] =
#endif
	{
		SERIAL_5N1,
		SERIAL_6N1,
		SERIAL_7N1,
		SERIAL_8N1,
		SERIAL_5N2,
		SERIAL_6N2,
		SERIAL_7N2,
		SERIAL_8N2,
		SERIAL_5E1,
		SERIAL_6E1,
		SERIAL_7E1,
		SERIAL_8E1,
		SERIAL_5E2,
		SERIAL_6E2,
		SERIAL_7E2,
		SERIAL_8E2,
		SERIAL_5O1,
		SERIAL_6O1,
		SERIAL_7O1,
		SERIAL_8O1,
		SERIAL_5O2,
		SERIAL_6O2,
		SERIAL_7O2,
		SERIAL_8O2
	};

#endif // VISUINO_CHIPKIT
*/
#ifdef VISUINO_ESP8266
	static const	SerialConfig CSerialInits[] =
#else
	static const	long CSerialInits[] =
#endif
	{
		SERIAL_5N1,
		SERIAL_6N1,
		SERIAL_7N1,
		SERIAL_8N1,
		SERIAL_5N2,
		SERIAL_6N2,
		SERIAL_7N2,
		SERIAL_8N2,
		SERIAL_5E1,
		SERIAL_6E1,
		SERIAL_7E1,
		SERIAL_8E1,
		SERIAL_5E2,
		SERIAL_6E2,
		SERIAL_7E2,
		SERIAL_8E2,
		SERIAL_5O1,
		SERIAL_6O1,
		SERIAL_7O1,
		SERIAL_8O1,
		SERIAL_5O2,
		SERIAL_6O2,
		SERIAL_7O2,
		SERIAL_8O2
	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits,
		typename T_TXPin
	> class StandardSerialDelayImplementation :
		public TBasicSerialEndImplementation<T_SERIAL_TYPE, C_SERIAL>,
		public T_DataBits,
		public T_Parity,
		public T_Speed,
		public T_StopBits,
		public T_TXPin
	{
	public:
		_V_PROP_( Speed )
		_V_PROP_( Parity )
		_V_PROP_( StopBits )
		_V_PROP_( DataBits )
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

#ifdef VISUINO_ESP32
			volatile bool ADummy = false; // Force the library to build properly!
			if( ADummy )
				C_SERIAL.println(); // Work around ESP32 library bug

			C_SERIAL.flush();
#endif // VISUINO_ESP32
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_DataBits,
		typename T_Parity,
		typename T_Speed,
		typename T_StopBits
	> class StandardSerialDelayNoTXPinImplementation :
			public StandardSerialDelayImplementation <
					T_SERIAL_TYPE, C_SERIAL,
					T_DataBits,
					T_Parity,
					T_Speed,
					T_StopBits,
                    Mitov::ConstantProperty<87, uint32_t, 200 >
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
	> class SerialPort : 
		public BasicSendingSerialPort <
				T_SERIAL_TYPE, C_SERIAL,
				StandardSerialDelayNoTXPinImplementation <
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
